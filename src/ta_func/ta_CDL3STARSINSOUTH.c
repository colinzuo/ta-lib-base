#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_CDL3STARSINSOUTH_Lookback( void )
{
    return max( max( TA_CANDLEAVGPERIOD(ShadowVeryShort), TA_CANDLEAVGPERIOD(ShadowLong) ),
                max( TA_CANDLEAVGPERIOD(BodyLong), TA_CANDLEAVGPERIOD(BodyShort) )
            ) + 2;
}

/*
 * TA_CDL3STARSINSOUTH - Three Stars In The South
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDL3STARSINSOUTH( int    startIdx,
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
    double BodyLongPeriodTotal, BodyShortPeriodTotal, ShadowLongPeriodTotal;
	ARRAY_LOCAL(ShadowVeryShortPeriodTotal,2);
    int i, outIdx, totIdx, BodyLongTrailingIdx, BodyShortTrailingIdx, ShadowLongTrailingIdx, ShadowVeryShortTrailingIdx,
        lookbackTotal;

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

   lookbackTotal = TA_CDL3STARSINSOUTH_Lookback();

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
   BodyLongPeriodTotal = 0;
   BodyLongTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(BodyLong);
   ShadowLongPeriodTotal = 0;
   ShadowLongTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(ShadowLong);
   ShadowVeryShortPeriodTotal[1] = 0;
   ShadowVeryShortPeriodTotal[0] = 0;
   ShadowVeryShortTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(ShadowVeryShort);
   BodyShortPeriodTotal = 0;
   BodyShortTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(BodyShort);
   
   i = BodyLongTrailingIdx;
   while( i < startIdx ) {
        BodyLongPeriodTotal += TA_CANDLERANGE( BodyLong, i-2 );
        i++;
   }
   i = ShadowLongTrailingIdx;
   while( i < startIdx ) {
        ShadowLongPeriodTotal += TA_CANDLERANGE( ShadowLong, i-2 );
        i++;
   }
   i = ShadowVeryShortTrailingIdx;
   while( i < startIdx ) {
        ShadowVeryShortPeriodTotal[1] += TA_CANDLERANGE( ShadowVeryShort, i-1 );
        ShadowVeryShortPeriodTotal[0] += TA_CANDLERANGE( ShadowVeryShort, i );
        i++;
   }
   i = BodyShortTrailingIdx;
   while( i < startIdx ) {
        BodyShortPeriodTotal += TA_CANDLERANGE( BodyShort, i );
        i++;
   }
   i = startIdx;

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - first candle: long black candle with long lower shadow
    * - second candle: smaller black candle that opens higher than prior close but within prior candle's range 
    *   and trades lower than prior close but not lower than prior low and closes off of its low (it has a shadow)
    * - third candle: small black marubozu (or candle with very short shadows) engulfed by prior candle's range
    * The meanings of "long body", "short body", "very short shadow" are specified with TA_SetCandleSettings;
    * outInteger is positive (1 to 100): 3 stars in the south is always bullish;
    * the user should consider that 3 stars in the south is significant when it appears in downtrend, while this function 
    * does not consider it
    */
   outIdx = 0;
   do
   {
        if( TA_CANDLECOLOR(i-2) == -1 &&                                    // 1st black
            TA_CANDLECOLOR(i-1) == -1 &&                                    // 2nd black
            TA_CANDLECOLOR(i) == -1 &&                                      // 3rd black
                                                                            // 1st: long
            TA_REALBODY(i-2) > TA_CANDLEAVERAGE( BodyLong, BodyLongPeriodTotal, i-2 ) &&
                                                                            //      with long lower shadow
            TA_LOWERSHADOW(i-2) > TA_CANDLEAVERAGE( ShadowLong, ShadowLongPeriodTotal, i-2 ) &&
            TA_REALBODY(i-1) < TA_REALBODY(i-2) &&                          // 2nd: smaller candle
            inOpen[i-1] > inClose[i-2] && inOpen[i-1] <= inHigh[i-2] &&     //      that opens higher but within 1st range
            inLow[i-1] < inClose[i-2] &&                                    //      and trades lower than 1st close
            inLow[i-1] >= inLow[i-2] &&                                     //      but not lower than 1st low
                                                                            //      and has a lower shadow
            TA_LOWERSHADOW(i-1) > TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal[1], i-1 ) &&
                                                                            // 3rd: small marubozu
            TA_REALBODY(i) < TA_CANDLEAVERAGE( BodyShort, BodyShortPeriodTotal, i ) &&
            TA_LOWERSHADOW(i) < TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal[0], i ) &&
            TA_UPPERSHADOW(i) < TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal[0], i ) &&
            inLow[i] > inLow[i-1] && inHigh[i] < inHigh[i-1]                //      engulfed by prior candle's range
          )
            outInteger[outIdx++] = 100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        BodyLongPeriodTotal += TA_CANDLERANGE( BodyLong, i-2 ) 
                             - TA_CANDLERANGE( BodyLong, BodyLongTrailingIdx-2 );
        ShadowLongPeriodTotal += TA_CANDLERANGE( ShadowLong, i-2 ) 
                               - TA_CANDLERANGE( ShadowLong, ShadowLongTrailingIdx-2 );
        for (totIdx = 1; totIdx >= 0; --totIdx)
            ShadowVeryShortPeriodTotal[totIdx] += TA_CANDLERANGE( ShadowVeryShort, i-totIdx ) 
                                                - TA_CANDLERANGE( ShadowVeryShort, ShadowVeryShortTrailingIdx-totIdx );
        BodyShortPeriodTotal += TA_CANDLERANGE( BodyShort, i ) 
                              - TA_CANDLERANGE( BodyShort, BodyShortTrailingIdx );
        i++; 
        BodyLongTrailingIdx++;
        ShadowLongTrailingIdx++;
        ShadowVeryShortTrailingIdx++;
        BodyShortTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
