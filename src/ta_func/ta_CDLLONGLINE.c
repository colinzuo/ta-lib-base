#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_CDLLONGLINE_Lookback( void )
{
    return max( TA_CANDLEAVGPERIOD(BodyLong), TA_CANDLEAVGPERIOD(ShadowShort) );
}

/*
 * TA_CDLLONGLINE - Long Line Candle
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLLONGLINE( int    startIdx,
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
    double BodyPeriodTotal, ShadowPeriodTotal;
    int i, outIdx, BodyTrailingIdx, ShadowTrailingIdx, lookbackTotal;

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

   lookbackTotal = TA_CDLLONGLINE_Lookback();

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
   BodyTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(BodyLong);
   ShadowPeriodTotal = 0;
   ShadowTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(ShadowShort);
   
   i = BodyTrailingIdx;
   while( i < startIdx ) {
        BodyPeriodTotal += TA_CANDLERANGE( BodyLong, i );
        i++;
   }
   i = ShadowTrailingIdx;
   while( i < startIdx ) {
        ShadowPeriodTotal += TA_CANDLERANGE( ShadowShort, i );
        i++;
   }

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - long real body
    * - short upper and lower shadow
    * The meaning of "long" and "short" is specified with TA_SetCandleSettings
    * outInteger is positive (1 to 100) when white (bullish), negative (-1 to -100) when black (bearish)
    */
   outIdx = 0;
   do
   {
        if( TA_REALBODY(i) > TA_CANDLEAVERAGE( BodyLong, BodyPeriodTotal, i ) &&
            TA_UPPERSHADOW(i) < TA_CANDLEAVERAGE( ShadowShort, ShadowPeriodTotal, i ) &&
            TA_LOWERSHADOW(i) < TA_CANDLEAVERAGE( ShadowShort, ShadowPeriodTotal, i ) )
            outInteger[outIdx++] = TA_CANDLECOLOR(i) * 100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        BodyPeriodTotal += TA_CANDLERANGE( BodyLong, i ) - TA_CANDLERANGE( BodyLong, BodyTrailingIdx );
        ShadowPeriodTotal += TA_CANDLERANGE( ShadowShort, i ) - TA_CANDLERANGE( ShadowShort, ShadowTrailingIdx );
        i++; 
        BodyTrailingIdx++;
        ShadowTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
