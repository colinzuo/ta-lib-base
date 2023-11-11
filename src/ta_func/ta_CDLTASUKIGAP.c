#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_CDLTASUKIGAP_Lookback( void )
{
    return TA_CANDLEAVGPERIOD(Near) + 2;
}

/*
 * TA_CDLTASUKIGAP - Tasuki Gap
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLTASUKIGAP( int    startIdx,
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
    double NearPeriodTotal;
    int i, outIdx, NearTrailingIdx, lookbackTotal;

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

   lookbackTotal = TA_CDLTASUKIGAP_Lookback();

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
   NearPeriodTotal = 0;
   NearTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(Near);

   i = NearTrailingIdx;
   while( i < startIdx ) {
        NearPeriodTotal += TA_CANDLERANGE( Near, i-1 );
        i++;
   }
   i = startIdx;

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - upside (downside) gap
    * - first candle after the window: white (black) candlestick
    * - second candle: black (white) candlestick that opens within the previous real body and closes under (above)
    *   the previous real body inside the gap
    * - the size of two real bodies should be near the same
    * The meaning of "near" is specified with TA_SetCandleSettings
    * outInteger is positive (1 to 100) when bullish or negative (-1 to -100) when bearish;
    * the user should consider that tasuki gap is significant when it appears in a trend, while this function does 
    * not consider it
    */
   outIdx = 0;
   do
   {
        if( 
            ( 
                TA_REALBODYGAPUP(i-1,i-2) &&                                // upside gap
                TA_CANDLECOLOR(i-1) == 1 &&                                 // 1st: white
                TA_CANDLECOLOR(i) == -1 &&                                  // 2nd: black
                inOpen[i] < inClose[i-1] && inOpen[i] > inOpen[i-1] &&      //      that opens within the white rb
                inClose[i] < inOpen[i-1] &&                                 //      and closes under the white rb
                inClose[i] > max(inClose[i-2], inOpen[i-2]) &&              //      inside the gap
                                                                            // size of 2 rb near the same
                std_fabs(TA_REALBODY(i-1) - TA_REALBODY(i)) < TA_CANDLEAVERAGE( Near, NearPeriodTotal, i-1 )
            ) ||
            (
                TA_REALBODYGAPDOWN(i-1,i-2) &&                              // downside gap
                TA_CANDLECOLOR(i-1) == -1 &&                                // 1st: black
                TA_CANDLECOLOR(i) == 1 &&                                   // 2nd: white
                inOpen[i] < inOpen[i-1] && inOpen[i] > inClose[i-1] &&      //      that opens within the black rb
                inClose[i] > inOpen[i-1] &&                                 //      and closes above the black rb
                inClose[i] < min(inClose[i-2], inOpen[i-2]) &&              //      inside the gap
                                                                            // size of 2 rb near the same
                std_fabs(TA_REALBODY(i-1) - TA_REALBODY(i)) < TA_CANDLEAVERAGE( Near, NearPeriodTotal, i-1 )
            )
        )
            outInteger[outIdx++] = TA_CANDLECOLOR(i-1) * 100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        NearPeriodTotal += TA_CANDLERANGE( Near, i-1 ) - TA_CANDLERANGE( Near, NearTrailingIdx-1 );
        i++; 
        NearTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
