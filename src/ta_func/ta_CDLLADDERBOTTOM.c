#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_CDLLADDERBOTTOM_Lookback( void )
{
    return TA_CANDLEAVGPERIOD(ShadowVeryShort) + 4;
}

/*
 * TA_CDLLADDERBOTTOM - Ladder Bottom
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLLADDERBOTTOM( int    startIdx,
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
    double ShadowVeryShortPeriodTotal;
    int i, outIdx, ShadowVeryShortTrailingIdx, lookbackTotal;

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

   lookbackTotal = TA_CDLLADDERBOTTOM_Lookback();

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
   ShadowVeryShortPeriodTotal = 0;
   ShadowVeryShortTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(ShadowVeryShort);
   
   i = ShadowVeryShortTrailingIdx;
   while( i < startIdx ) {
        ShadowVeryShortPeriodTotal += TA_CANDLERANGE( ShadowVeryShort, i-1 );
        i++;
   }
   i = startIdx;

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - three black candlesticks with consecutively lower opens and closes
    * - fourth candle: black candle with an upper shadow (it's supposed to be not very short)
    * - fifth candle: white candle that opens above prior candle's body and closes above prior candle's high
    * The meaning of "very short" is specified with TA_SetCandleSettings
    * outInteger is positive (1 to 100): ladder bottom is always bullish; 
    * the user should consider that ladder bottom is significant when it appears in a downtrend, 
    * while this function does not consider it
    */
   outIdx = 0;
   do
   {
        if(                                                             
            TA_CANDLECOLOR(i-4) == -1 && TA_CANDLECOLOR(i-3) == -1 && TA_CANDLECOLOR(i-2) == -1 &&  // 3 black candlesticks
            inOpen[i-4] > inOpen[i-3] && inOpen[i-3] > inOpen[i-2] &&           // with consecutively lower opens
            inClose[i-4] > inClose[i-3] && inClose[i-3] > inClose[i-2] &&       // and closes
            TA_CANDLECOLOR(i-1) == -1 &&                                        // 4th: black with an upper shadow
            TA_UPPERSHADOW(i-1) > TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal, i-1 ) &&
            TA_CANDLECOLOR(i) == 1 &&                                           // 5th: white
            inOpen[i] > inOpen[i-1] &&                                          // that opens above prior candle's body
            inClose[i] > inHigh[i-1]                                            // and closes above prior candle's high
          )
            outInteger[outIdx++] = 100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        ShadowVeryShortPeriodTotal += TA_CANDLERANGE( ShadowVeryShort, i-1 ) 
                                    - TA_CANDLERANGE( ShadowVeryShort, ShadowVeryShortTrailingIdx-1 );
        i++; 
        ShadowVeryShortTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
