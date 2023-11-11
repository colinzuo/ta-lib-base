#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_CDLTHRUSTING_Lookback( void )
{
    return max( TA_CANDLEAVGPERIOD(Equal), TA_CANDLEAVGPERIOD(BodyLong)
            ) + 1;
}

/*
 * TA_CDLTHRUSTING - Thrusting Pattern
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLTHRUSTING( int    startIdx,
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
    double EqualPeriodTotal, BodyLongPeriodTotal;
    int i, outIdx, EqualTrailingIdx, BodyLongTrailingIdx, lookbackTotal;

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

   lookbackTotal = TA_CDLTHRUSTING_Lookback();

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
   EqualPeriodTotal = 0;
   EqualTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(Equal);
   BodyLongPeriodTotal = 0;
   BodyLongTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(BodyLong);

   i = EqualTrailingIdx;
   while( i < startIdx ) {
        EqualPeriodTotal += TA_CANDLERANGE( Equal, i-1 );
        i++;
   }
   i = BodyLongTrailingIdx;
   while( i < startIdx ) {
        BodyLongPeriodTotal += TA_CANDLERANGE( BodyLong, i-1 );
        i++;
   }
   i = startIdx;

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - first candle: long black candle
    * - second candle: white candle with open below previous day low and close into previous day body under the midpoint;
    * to differentiate it from in-neck the close should not be equal to the black candle's close
    * The meaning of "equal" is specified with TA_SetCandleSettings
    * outInteger is negative (-1 to -100): thrusting pattern is always bearish
    * the user should consider that the thrusting pattern is significant when it appears in a downtrend and it could be 
    * even bullish "when coming in an uptrend or occurring twice within several days" (Steve Nison says), while this 
    * function does not consider the trend
    */
   outIdx = 0;
   do
   {
        if( TA_CANDLECOLOR(i-1) == -1 &&                                                        // 1st: black
            TA_REALBODY(i-1) > TA_CANDLEAVERAGE( BodyLong, BodyLongPeriodTotal, i-1 ) &&     //  long
            TA_CANDLECOLOR(i) == 1 &&                                                           // 2nd: white
            inOpen[i] < inLow[i-1] &&                                                           //  open below prior low
            inClose[i] > inClose[i-1] + TA_CANDLEAVERAGE( Equal, EqualPeriodTotal, i-1 ) &&  //  close into prior body
            inClose[i] <= inClose[i-1] + TA_REALBODY(i-1) * 0.5                                 //   under the midpoint
          )
            outInteger[outIdx++] = -100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        EqualPeriodTotal += TA_CANDLERANGE( Equal, i-1 ) - TA_CANDLERANGE( Equal, EqualTrailingIdx-1 );
        BodyLongPeriodTotal += TA_CANDLERANGE( BodyLong, i-1 ) 
                             - TA_CANDLERANGE( BodyLong, BodyLongTrailingIdx-1 );
        i++;
        EqualTrailingIdx++;
        BodyLongTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
