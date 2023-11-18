#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_CDL3LINESTRIKE_Lookback( void )
{
    return TA_CANDLEAVGPERIOD(Near) + 3;
}

/*
 * TA_CDL3LINESTRIKE - Three-Line Strike 
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDL3LINESTRIKE( int    startIdx,
                              int    endIdx,
                              const double inOpen[],
                              const double inHigh[],
                              const double inLow[],
                              const double inClose[],
                              int          *outBegIdx,
                              int          *outNBElement,
                              int           outInteger[] )
{
   /* Insert local variables here. */
    ARRAY_LOCAL(NearPeriodTotal,4);
    int i, outIdx, totIdx, NearTrailingIdx, lookbackTotal;

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   /* Verify required price component. */
   if(!inOpen||!inHigh||!inLow||!inClose)
      return TA_BAD_PARAM;

      if( !outInteger )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Identify the minimum number of price bar needed
    * to calculate at least one output.
    */

   lookbackTotal = TA_CDL3LINESTRIKE_Lookback();

   /* Move up the start index if there is not
    * enough initial data.
    */
   if( startIdx < lookbackTotal )
      startIdx = lookbackTotal;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
   {
      *outBegIdx = 0;
      *outNBElement = 0;
      return TA_SUCCESS;
   }

   /* Do the calculation using tight loops. */
   /* Add-up the initial period, except for the last value. */
   NearPeriodTotal[3] = 0;
   NearPeriodTotal[2] = 0;
   NearTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(Near);
   
   i = NearTrailingIdx;
   while( i < startIdx ) {
        NearPeriodTotal[3] += TA_CANDLERANGE( Near, i-3 );
        NearPeriodTotal[2] += TA_CANDLERANGE( Near, i-2 );
        i++;
   }
   i = startIdx;

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - three white soldiers (three black crows): three white (black) candlesticks with consecutively higher (lower) closes,
    * each opening within or near the previous real body
    * - fourth candle: black (white) candle that opens above (below) prior candle's close and closes below (above) 
    * the first candle's open
    * The meaning of "near" is specified with TA_SetCandleSettings;
    * outInteger is positive (1 to 100) when bullish or negative (-1 to -100) when bearish;
    * the user should consider that 3-line strike is significant when it appears in a trend in the same direction of
    * the first three candles, while this function does not consider it
    */
   outIdx = 0;
   do
   {
        if( TA_CANDLECOLOR(i-3) == TA_CANDLECOLOR(i-2) &&                                   // three with same color
            TA_CANDLECOLOR(i-2) == TA_CANDLECOLOR(i-1) &&
            TA_CANDLECOLOR(i) == -TA_CANDLECOLOR(i-1) &&                                    // 4th opposite color
                                                                                            // 2nd opens within/near 1st rb
            inOpen[i-2] >= min( inOpen[i-3], inClose[i-3] ) - TA_CANDLEAVERAGE( Near, NearPeriodTotal[3], i-3 ) &&
            inOpen[i-2] <= max( inOpen[i-3], inClose[i-3] ) + TA_CANDLEAVERAGE( Near, NearPeriodTotal[3], i-3 ) &&
                                                                                            // 3rd opens within/near 2nd rb
            inOpen[i-1] >= min( inOpen[i-2], inClose[i-2] ) - TA_CANDLEAVERAGE( Near, NearPeriodTotal[2], i-2 ) &&
            inOpen[i-1] <= max( inOpen[i-2], inClose[i-2] ) + TA_CANDLEAVERAGE( Near, NearPeriodTotal[2], i-2 ) &&
            (
                (   // if three white
                    TA_CANDLECOLOR(i-1) == 1 &&
                    inClose[i-1] > inClose[i-2] && inClose[i-2] > inClose[i-3] &&           // consecutive higher closes
                    inOpen[i] > inClose[i-1] &&                                             // 4th opens above prior close
                    inClose[i] < inOpen[i-3]                                                // 4th closes below 1st open
                ) ||
                (   // if three black
                    TA_CANDLECOLOR(i-1) == -1 &&
                    inClose[i-1] < inClose[i-2] && inClose[i-2] < inClose[i-3] &&           // consecutive lower closes
                    inOpen[i] < inClose[i-1] &&                                             // 4th opens below prior close
                    inClose[i] > inOpen[i-3]                                                // 4th closes above 1st open
                )
            )
          )
            outInteger[outIdx++] = TA_CANDLECOLOR(i-1) * 100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        for (totIdx = 3; totIdx >= 2; --totIdx)
            NearPeriodTotal[totIdx] += TA_CANDLERANGE( Near, i-totIdx ) 
                                     - TA_CANDLERANGE( Near, NearTrailingIdx-totIdx );
        i++; 
        NearTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
