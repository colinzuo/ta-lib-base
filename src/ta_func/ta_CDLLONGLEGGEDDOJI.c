#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_CDLLONGLEGGEDDOJI_Lookback( void )
{
    return max( TA_CANDLEAVGPERIOD(BodyDoji), TA_CANDLEAVGPERIOD(ShadowLong) );
}

/*
 * TA_CDLLONGLEGGEDDOJI - Long Legged Doji
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLLONGLEGGEDDOJI( int    startIdx,
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
    double BodyDojiPeriodTotal, ShadowLongPeriodTotal;
    int i, outIdx, BodyDojiTrailingIdx, ShadowLongTrailingIdx, lookbackTotal;

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

   lookbackTotal = TA_CDLLONGLEGGEDDOJI_Lookback();

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
   ShadowLongPeriodTotal = 0;
   ShadowLongTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(ShadowLong);
   
   i = BodyDojiTrailingIdx;
   while( i < startIdx ) {
        BodyDojiPeriodTotal += TA_CANDLERANGE( BodyDoji, i );
        i++;
   }
   i = ShadowLongTrailingIdx;
   while( i < startIdx ) {
        ShadowLongPeriodTotal += TA_CANDLERANGE( ShadowLong, i );
        i++;
   }

   /* Proceed with the calculation for the requested range.
    *
    * Must have:
    * - doji body
    * - one or two long shadows
    * The meaning of "doji" is specified with TA_SetCandleSettings
    * outInteger is always positive (1 to 100) but this does not mean it is bullish: long legged doji shows uncertainty
    */
   outIdx = 0;
   do
   {
        if( TA_REALBODY(i) <= TA_CANDLEAVERAGE( BodyDoji, BodyDojiPeriodTotal, i ) &&
            ( TA_LOWERSHADOW(i) > TA_CANDLEAVERAGE( ShadowLong, ShadowLongPeriodTotal, i ) 
              ||
              TA_UPPERSHADOW(i) > TA_CANDLEAVERAGE( ShadowLong, ShadowLongPeriodTotal, i )
            )
          )
            outInteger[outIdx++] = 100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        BodyDojiPeriodTotal += TA_CANDLERANGE( BodyDoji, i ) - TA_CANDLERANGE( BodyDoji, BodyDojiTrailingIdx );
        ShadowLongPeriodTotal += TA_CANDLERANGE( ShadowLong, i ) - TA_CANDLERANGE( ShadowLong, ShadowLongTrailingIdx );
        i++; 
        BodyDojiTrailingIdx++;
        ShadowLongTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
