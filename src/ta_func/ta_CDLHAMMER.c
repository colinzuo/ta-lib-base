#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_CDLHAMMER_Lookback( void )
{
   /* insert local variable here */


/* No parameters to validate. */


   /* insert lookback code here. */
    return max( max( max( TA_CANDLEAVGPERIOD(BodyShort), TA_CANDLEAVGPERIOD(ShadowLong) ),
                     TA_CANDLEAVGPERIOD(ShadowVeryShort) ), 
                TA_CANDLEAVGPERIOD(Near)
            ) + 1;
}

/*
 * TA_CDLHAMMER - Hammer
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLHAMMER( int    startIdx,
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
    double BodyPeriodTotal, ShadowLongPeriodTotal, ShadowVeryShortPeriodTotal, NearPeriodTotal;
    int i, outIdx, BodyTrailingIdx, ShadowLongTrailingIdx, ShadowVeryShortTrailingIdx, NearTrailingIdx, lookbackTotal;

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

   lookbackTotal = TA_CDLHAMMER_Lookback();

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
   BodyPeriodTotal = 0;
   BodyTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(BodyShort);
   ShadowLongPeriodTotal = 0;
   ShadowLongTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(ShadowLong);
   ShadowVeryShortPeriodTotal = 0;
   ShadowVeryShortTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(ShadowVeryShort);
   NearPeriodTotal = 0;
   NearTrailingIdx = startIdx -1 - TA_CANDLEAVGPERIOD(Near);
   
   i = BodyTrailingIdx;
   while( i < startIdx ) {
        BodyPeriodTotal += TA_CANDLERANGE( BodyShort, i );
        i++;
   }
   i = ShadowLongTrailingIdx;
   while( i < startIdx ) {
        ShadowLongPeriodTotal += TA_CANDLERANGE( ShadowLong, i );
        i++;
   }
   i = ShadowVeryShortTrailingIdx;
   while( i < startIdx ) {
        ShadowVeryShortPeriodTotal += TA_CANDLERANGE( ShadowVeryShort, i );
        i++;
   }
   i = NearTrailingIdx;
   while( i < startIdx-1 ) {
        NearPeriodTotal += TA_CANDLERANGE( Near, i );
        i++;
   }
   i = startIdx;

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - small real body
    * - long lower shadow
    * - no, or very short, upper shadow
    * - body below or near the lows of the previous candle
    * The meaning of "short", "long" and "near the lows" is specified with TA_SetCandleSettings;
    * outInteger is positive (1 to 100): hammer is always bullish;
    * the user should consider that a hammer must appear in a downtrend, while this function does not consider it
    */
   outIdx = 0;
   do
   {
        if( TA_REALBODY(i) < TA_CANDLEAVERAGE( BodyShort, BodyPeriodTotal, i ) &&                        // small rb
            TA_LOWERSHADOW(i) > TA_CANDLEAVERAGE( ShadowLong, ShadowLongPeriodTotal, i ) &&              // long lower shadow
            TA_UPPERSHADOW(i) < TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal, i ) &&    // very short upper shadow
            min( inClose[i], inOpen[i] ) <= inLow[i-1] + TA_CANDLEAVERAGE( Near, NearPeriodTotal, i-1 )  // rb near the prior candle's lows
          )
            outInteger[outIdx++] = 100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        BodyPeriodTotal += TA_CANDLERANGE( BodyShort, i ) 
            - TA_CANDLERANGE( BodyShort, BodyTrailingIdx );
        ShadowLongPeriodTotal += TA_CANDLERANGE( ShadowLong, i ) 
            - TA_CANDLERANGE( ShadowLong, ShadowLongTrailingIdx );
        ShadowVeryShortPeriodTotal += TA_CANDLERANGE( ShadowVeryShort, i ) 
            - TA_CANDLERANGE( ShadowVeryShort, ShadowVeryShortTrailingIdx );
        NearPeriodTotal += TA_CANDLERANGE( Near, i-1 ) 
            - TA_CANDLERANGE( Near, NearTrailingIdx );
        i++; 
        BodyTrailingIdx++;
        ShadowLongTrailingIdx++;
        ShadowVeryShortTrailingIdx++;
        NearTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
