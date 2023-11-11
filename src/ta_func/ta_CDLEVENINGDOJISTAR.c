#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_CDLEVENINGDOJISTAR_Lookback( double        optInPenetration )  /* From 0 to TA_REAL_MAX */
{
   /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   if( optInPenetration == TA_REAL_DEFAULT )
      optInPenetration = 3.000000e-1;
   else if( (optInPenetration < 0.000000e+0) || (optInPenetration > 3.000000e+37) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */
    (void)optInPenetration;

    return max( max( TA_CANDLEAVGPERIOD(BodyDoji), TA_CANDLEAVGPERIOD(BodyLong) ), 
                TA_CANDLEAVGPERIOD(BodyShort) 
            ) + 2;
}

/*
 * TA_CDLEVENINGDOJISTAR - Evening Doji Star
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 * Optional Parameters
 * -------------------
 * optInPenetration:(From 0 to TA_REAL_MAX)
 *    Percentage of penetration of a candle within another candle
 * 
 * 
 */
TA_RetCode TA_CDLEVENINGDOJISTAR( int    startIdx,
                                  int    endIdx,
                                  const double inOpen[],
                                  const double inHigh[],
                                  const double inLow[],
                                  const double inClose[],
                                  double        optInPenetration, /* From 0 to TA_REAL_MAX */
                                  int          *outBegIdx,
                                  int          *outNBElement,
                                  int           outInteger[] )
{
   /* Insert local variables here. */
    double BodyDojiPeriodTotal, BodyLongPeriodTotal, BodyShortPeriodTotal;
    int i, outIdx, BodyDojiTrailingIdx, BodyLongTrailingIdx, BodyShortTrailingIdx, lookbackTotal;

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   /* Verify required price component. */
   if(!inOpen||!inHigh||!inLow||!inClose)
      return TA_BAD_PARAM;

      if( optInPenetration == TA_REAL_DEFAULT )
      optInPenetration = 3.000000e-1;
   else if( (optInPenetration < 0.000000e+0) || (optInPenetration > 3.000000e+37) )
      return TA_BAD_PARAM;

   if( !outInteger )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Identify the minimum number of price bar needed
    * to calculate at least one output.
    */

   lookbackTotal = TA_CDLEVENINGDOJISTAR_Lookback(optInPenetration);

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
   BodyDojiPeriodTotal = 0;
   BodyShortPeriodTotal = 0;
   BodyLongTrailingIdx = startIdx -2 - TA_CANDLEAVGPERIOD(BodyLong);
   BodyDojiTrailingIdx = startIdx -1 - TA_CANDLEAVGPERIOD(BodyDoji);
   BodyShortTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(BodyShort);
   
   i = BodyLongTrailingIdx;
   while( i < startIdx-2 ) {
        BodyLongPeriodTotal += TA_CANDLERANGE( BodyLong, i );
        i++;
   }
   i = BodyDojiTrailingIdx;
   while( i < startIdx-1 ) {
        BodyDojiPeriodTotal += TA_CANDLERANGE( BodyDoji, i );
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
    * - first candle: long white real body
    * - second candle: doji gapping up
    * - third candle: black real body that moves well within the first candle's real body
    * The meaning of "doji" and "long" is specified with TA_SetCandleSettings
    * The meaning of "moves well within" is specified with optInPenetration and "moves" should mean the real body should
    * not be short ("short" is specified with TA_SetCandleSettings) - Greg Morris wants it to be long, someone else want
    * it to be relatively long
    * outInteger is negative (-1 to -100): evening star is always bearish; 
    * the user should consider that an evening star is significant when it appears in an uptrend, 
    * while this function does not consider the trend
    */
   outIdx = 0;
   do
   {
        if( TA_REALBODY(i-2) > TA_CANDLEAVERAGE( BodyLong, BodyLongPeriodTotal, i-2 ) &&         // 1st: long
            TA_CANDLECOLOR(i-2) == 1 &&                                                             //           white
            TA_REALBODY(i-1) <= TA_CANDLEAVERAGE( BodyDoji, BodyDojiPeriodTotal, i-1 ) &&        // 2nd: doji
            TA_REALBODYGAPUP(i-1,i-2) &&                                                            //           gapping up
            TA_REALBODY(i) > TA_CANDLEAVERAGE( BodyShort, BodyShortPeriodTotal, i ) &&           // 3rd: longer than short
            TA_CANDLECOLOR(i) == -1 &&                                                              //          black real body
            inClose[i] < inClose[i-2] - TA_REALBODY(i-2) * optInPenetration                         //               closing well within 1st rb
          )
            outInteger[outIdx++] = -100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        BodyLongPeriodTotal += TA_CANDLERANGE( BodyLong, i-2 ) - TA_CANDLERANGE( BodyLong, BodyLongTrailingIdx );
        BodyDojiPeriodTotal += TA_CANDLERANGE( BodyDoji, i-1 ) - TA_CANDLERANGE( BodyDoji, BodyDojiTrailingIdx );
        BodyShortPeriodTotal += TA_CANDLERANGE( BodyShort, i ) - TA_CANDLERANGE( BodyShort, BodyShortTrailingIdx );
        i++; 
        BodyLongTrailingIdx++;
        BodyDojiTrailingIdx++;
        BodyShortTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
