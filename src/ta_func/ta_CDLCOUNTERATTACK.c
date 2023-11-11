#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_CDLCOUNTERATTACK_Lookback( void )
{
    return max( TA_CANDLEAVGPERIOD(Equal), TA_CANDLEAVGPERIOD(BodyLong)
            ) + 1;
}

/*
 * TA_CDLCOUNTERATTACK - Counterattack
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLCOUNTERATTACK( int    startIdx,
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
    double EqualPeriodTotal;
	ARRAY_LOCAL(BodyLongPeriodTotal,2);
    int i, outIdx, totIdx, EqualTrailingIdx, BodyLongTrailingIdx, lookbackTotal;

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

   lookbackTotal = TA_CDLCOUNTERATTACK_Lookback();

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
   BodyLongPeriodTotal[1] = 0;
   BodyLongPeriodTotal[0] = 0;
   BodyLongTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(BodyLong);

   i = EqualTrailingIdx;
   while( i < startIdx ) {
        EqualPeriodTotal += TA_CANDLERANGE( Equal, i-1 );
        i++;
   }
   i = BodyLongTrailingIdx;
   while( i < startIdx ) {
        BodyLongPeriodTotal[1] += TA_CANDLERANGE( BodyLong, i-1 );
        BodyLongPeriodTotal[0] += TA_CANDLERANGE( BodyLong, i );
        i++;
   }
   i = startIdx;

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - first candle: long black (white)
    * - second candle: long white (black) with close equal to the prior close
    * The meaning of "equal" and "long" is specified with TA_SetCandleSettings
    * outInteger is positive (1 to 100) when bullish or negative (-1 to -100) when bearish;
    * the user should consider that counterattack is significant in a trend, while this function does not consider it
    */
   outIdx = 0;
   do
   {
        if( TA_CANDLECOLOR(i-1) == -TA_CANDLECOLOR(i) &&                                        // opposite candles
            TA_REALBODY(i-1) > TA_CANDLEAVERAGE( BodyLong, BodyLongPeriodTotal[1], i-1 ) &&     // 1st long
            TA_REALBODY(i) > TA_CANDLEAVERAGE( BodyLong, BodyLongPeriodTotal[0], i ) &&         // 2nd long
            inClose[i] <= inClose[i-1] + TA_CANDLEAVERAGE( Equal, EqualPeriodTotal, i-1 ) && // equal closes
            inClose[i] >= inClose[i-1] - TA_CANDLEAVERAGE( Equal, EqualPeriodTotal, i-1 )
          )
            outInteger[outIdx++] = TA_CANDLECOLOR(i) * 100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        EqualPeriodTotal += TA_CANDLERANGE( Equal, i-1 ) - TA_CANDLERANGE( Equal, EqualTrailingIdx-1 );
        for (totIdx = 1; totIdx >= 0; --totIdx)
            BodyLongPeriodTotal[totIdx] += TA_CANDLERANGE( BodyLong, i-totIdx ) 
                                         - TA_CANDLERANGE( BodyLong, BodyLongTrailingIdx-totIdx );
        i++;
        EqualTrailingIdx++;
        BodyLongTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
