#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_CDLCONCEALBABYSWALL_Lookback( void )
{
    return TA_CANDLEAVGPERIOD(ShadowVeryShort) + 3;
}

/*
 * TA_CDLCONCEALBABYSWALL - Concealing Baby Swallow
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLCONCEALBABYSWALL( int    startIdx,
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
    ARRAY_LOCAL(ShadowVeryShortPeriodTotal,4);
    int i, outIdx, totIdx, ShadowVeryShortTrailingIdx, lookbackTotal;

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

   lookbackTotal = TA_CDLCONCEALBABYSWALL_Lookback();

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
   ShadowVeryShortPeriodTotal[3] = 0;
   ShadowVeryShortPeriodTotal[2] = 0;
   ShadowVeryShortPeriodTotal[1] = 0;
   ShadowVeryShortTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(ShadowVeryShort);
   
   i = ShadowVeryShortTrailingIdx;
   while( i < startIdx ) {
        ShadowVeryShortPeriodTotal[3] += TA_CANDLERANGE( ShadowVeryShort, i-3 );
        ShadowVeryShortPeriodTotal[2] += TA_CANDLERANGE( ShadowVeryShort, i-2 );
        ShadowVeryShortPeriodTotal[1] += TA_CANDLERANGE( ShadowVeryShort, i-1 );
        i++;
   }
   i = startIdx;

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - first candle: black marubozu (very short shadows)
    * - second candle: black marubozu (very short shadows)
    * - third candle: black candle that opens gapping down but has an upper shadow that extends into the prior body
    * - fourth candle: black candle that completely engulfs the third candle, including the shadows
    * The meanings of "very short shadow" are specified with TA_SetCandleSettings;
    * outInteger is positive (1 to 100): concealing baby swallow is always bullish;
    * the user should consider that concealing baby swallow is significant when it appears in downtrend, while 
    * this function does not consider it
    */
   outIdx = 0;
   do
   {
        if( TA_CANDLECOLOR(i-3) == -1 &&                                    // 1st black
            TA_CANDLECOLOR(i-2) == -1 &&                                    // 2nd black
            TA_CANDLECOLOR(i-1) == -1 &&                                    // 3rd black
            TA_CANDLECOLOR(i) == -1 &&                                      // 4th black
                                                                            // 1st: marubozu
            TA_LOWERSHADOW(i-3) < TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal[3], i-3 ) &&
            TA_UPPERSHADOW(i-3) < TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal[3], i-3 ) &&
                                                                            // 2nd: marubozu
            TA_LOWERSHADOW(i-2) < TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal[2], i-2 ) &&
            TA_UPPERSHADOW(i-2) < TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal[2], i-2 ) &&
            TA_REALBODYGAPDOWN(i-1,i-2) &&                                  // 3rd: opens gapping down
                                                                            //      and HAS an upper shadow
            TA_UPPERSHADOW(i-1) > TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal[1], i-1 ) &&
            inHigh[i-1] > inClose[i-2] &&                                   //      that extends into the prior body
            inHigh[i] > inHigh[i-1] && inLow[i] < inLow[i-1]                // 4th: engulfs the 3rd including the shadows
          )
            outInteger[outIdx++] = 100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        for (totIdx = 3; totIdx >= 1; --totIdx)
            ShadowVeryShortPeriodTotal[totIdx] += TA_CANDLERANGE( ShadowVeryShort, i-totIdx ) 
                                                - TA_CANDLERANGE( ShadowVeryShort, ShadowVeryShortTrailingIdx-totIdx );
        i++; 
        ShadowVeryShortTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
