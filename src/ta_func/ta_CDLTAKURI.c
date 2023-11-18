#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_CDLTAKURI_Lookback( void )
{
    return max( max( TA_CANDLEAVGPERIOD(BodyDoji), TA_CANDLEAVGPERIOD(ShadowVeryShort) ),
                TA_CANDLEAVGPERIOD(ShadowVeryLong)
            );
}

/*
 * TA_CDLTAKURI - Takuri (Dragonfly Doji with very long lower shadow)
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLTAKURI( int    startIdx,
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
    double BodyDojiPeriodTotal, ShadowVeryShortPeriodTotal, ShadowVeryLongPeriodTotal;
    int i, outIdx, BodyDojiTrailingIdx, ShadowVeryShortTrailingIdx, ShadowVeryLongTrailingIdx, lookbackTotal;

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

   lookbackTotal = TA_CDLTAKURI_Lookback();

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
   BodyDojiPeriodTotal = 0;
   BodyDojiTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(BodyDoji);
   ShadowVeryShortPeriodTotal = 0;
   ShadowVeryShortTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(ShadowVeryShort);
   ShadowVeryLongPeriodTotal = 0;
   ShadowVeryLongTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(ShadowVeryLong);
   
   i = BodyDojiTrailingIdx;
   while( i < startIdx ) {
        BodyDojiPeriodTotal += TA_CANDLERANGE( BodyDoji, i );
        i++;
   }
   i = ShadowVeryShortTrailingIdx;
   while( i < startIdx ) {
        ShadowVeryShortPeriodTotal += TA_CANDLERANGE( ShadowVeryShort, i );
        i++;
   }
   i = ShadowVeryLongTrailingIdx;
   while( i < startIdx ) {
        ShadowVeryLongPeriodTotal += TA_CANDLERANGE( ShadowVeryLong, i );
        i++;
   }

   /* Proceed with the calculation for the requested range.
    *
    * Must have:
    * - doji body
    * - open and close at the high of the day = no or very short upper shadow
    * - very long lower shadow
    * The meaning of "doji", "very short" and "very long" is specified with TA_SetCandleSettings
    * outInteger is always positive (1 to 100) but this does not mean it is bullish: takuri must be considered
    * relatively to the trend
    */
   outIdx = 0;
   do
   {
        if( TA_REALBODY(i) <= TA_CANDLEAVERAGE( BodyDoji, BodyDojiPeriodTotal, i ) &&
            TA_UPPERSHADOW(i) < TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal, i ) &&
            TA_LOWERSHADOW(i) > TA_CANDLEAVERAGE( ShadowVeryLong, ShadowVeryLongPeriodTotal, i )
          )
            outInteger[outIdx++] = 100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        BodyDojiPeriodTotal += TA_CANDLERANGE( BodyDoji, i ) - TA_CANDLERANGE( BodyDoji, BodyDojiTrailingIdx );
        ShadowVeryShortPeriodTotal += TA_CANDLERANGE( ShadowVeryShort, i ) 
                                    - TA_CANDLERANGE( ShadowVeryShort, ShadowVeryShortTrailingIdx );
        ShadowVeryLongPeriodTotal += TA_CANDLERANGE( ShadowVeryLong, i ) 
                                   - TA_CANDLERANGE( ShadowVeryLong, ShadowVeryLongTrailingIdx );
        i++; 
        BodyDojiTrailingIdx++;
        ShadowVeryShortTrailingIdx++;
        ShadowVeryLongTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
