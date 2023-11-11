#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_CDLINVERTEDHAMMER_Lookback( void )
{
    return max( max( TA_CANDLEAVGPERIOD(BodyShort), TA_CANDLEAVGPERIOD(ShadowLong) ),
                TA_CANDLEAVGPERIOD(ShadowVeryShort)
            ) + 1;
}

/*
 * TA_CDLINVERTEDHAMMER - Inverted Hammer
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLINVERTEDHAMMER( int    startIdx,
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
    double BodyPeriodTotal, ShadowLongPeriodTotal, ShadowVeryShortPeriodTotal;
    int i, outIdx, BodyTrailingIdx, ShadowLongTrailingIdx, ShadowVeryShortTrailingIdx, lookbackTotal;

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

   lookbackTotal = TA_CDLINVERTEDHAMMER_Lookback();

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

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - small real body
    * - long upper shadow
    * - no, or very short, lower shadow
    * - gap down
    * The meaning of "short", "very short" and "long" is specified with TA_SetCandleSettings;
    * outInteger is positive (1 to 100): inverted hammer is always bullish;
    * the user should consider that an inverted hammer must appear in a downtrend, while this function does not consider it
    */
   outIdx = 0;
   do
   {
        if( TA_REALBODY(i) < TA_CANDLEAVERAGE( BodyShort, BodyPeriodTotal, i ) &&                        // small rb
            TA_UPPERSHADOW(i) > TA_CANDLEAVERAGE( ShadowLong, ShadowLongPeriodTotal, i ) &&              // long upper shadow
            TA_LOWERSHADOW(i) < TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal, i ) &&    // very short lower shadow
            TA_REALBODYGAPDOWN(i, i-1) )                                                                    // gap down
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
        i++; 
        BodyTrailingIdx++;
        ShadowLongTrailingIdx++;
        ShadowVeryShortTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
