#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_CDLMATHOLD_Lookback( double        optInPenetration )  /* From 0 to TA_REAL_MAX */
{
   /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   if( optInPenetration == TA_REAL_DEFAULT )
      optInPenetration = 5.000000e-1;
   else if( (optInPenetration < 0.000000e+0) || (optInPenetration > 3.000000e+37) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */
   (void)optInPenetration;
   return max( TA_CANDLEAVGPERIOD(BodyShort), TA_CANDLEAVGPERIOD(BodyLong) ) + 4;
}

/*
 * TA_CDLMATHOLD - Mat Hold
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
TA_RetCode TA_CDLMATHOLD( int    startIdx,
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
    ARRAY_LOCAL(BodyPeriodTotal,5);
    int i, outIdx, totIdx, BodyShortTrailingIdx, BodyLongTrailingIdx, lookbackTotal;

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
      optInPenetration = 5.000000e-1;
   else if( (optInPenetration < 0.000000e+0) || (optInPenetration > 3.000000e+37) )
      return TA_BAD_PARAM;

   if( !outInteger )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Identify the minimum number of price bar needed
    * to calculate at least one output.
    */

   lookbackTotal = TA_CDLMATHOLD_Lookback(optInPenetration);

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
   BodyPeriodTotal[4] = 0;
   BodyPeriodTotal[3] = 0;
   BodyPeriodTotal[2] = 0;
   BodyPeriodTotal[1] = 0;
   BodyPeriodTotal[0] = 0;
   BodyShortTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(BodyShort);
   BodyLongTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(BodyLong);
   
   i = BodyShortTrailingIdx;
   while( i < startIdx ) {
        BodyPeriodTotal[3] += TA_CANDLERANGE( BodyShort, i-3 );
        BodyPeriodTotal[2] += TA_CANDLERANGE( BodyShort, i-2 );
        BodyPeriodTotal[1] += TA_CANDLERANGE( BodyShort, i-1 );
        i++;
   }
   i = BodyLongTrailingIdx;
   while( i < startIdx ) {
        BodyPeriodTotal[4] += TA_CANDLERANGE( BodyLong, i-4 );
        i++;
   }
   i = startIdx;

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - first candle: long white candle
    * - upside gap between the first and the second bodies
    * - second candle: small black candle
    * - third and fourth candles: falling small real body candlesticks (commonly black) that hold within the long 
    *   white candle's body and are higher than the reaction days of the rising three methods
    * - fifth candle: white candle that opens above the previous small candle's close and closes higher than the 
    *   high of the highest reaction day
    * The meaning of "short" and "long" is specified with TA_SetCandleSettings; 
    * "hold within" means "a part of the real body must be within";
    * optInPenetration is the maximum percentage of the first white body the reaction days can penetrate (it is 
    * to specify how much the reaction days should be "higher than the reaction days of the rising three methods")
    * outInteger is positive (1 to 100): mat hold is always bullish
    */
   outIdx = 0;
   do
   {
        if( // 1st long, then 3 small
            TA_REALBODY(i-4) > TA_CANDLEAVERAGE( BodyLong, BodyPeriodTotal[4], i-4 ) &&
            TA_REALBODY(i-3) < TA_CANDLEAVERAGE( BodyShort, BodyPeriodTotal[3], i-3 ) &&
            TA_REALBODY(i-2) < TA_CANDLEAVERAGE( BodyShort, BodyPeriodTotal[2], i-2 ) &&
            TA_REALBODY(i-1) < TA_CANDLEAVERAGE( BodyShort, BodyPeriodTotal[1], i-1 ) &&
            // white, black, 2 black or white, white
            TA_CANDLECOLOR(i-4) == 1 &&
            TA_CANDLECOLOR(i-3) == -1 &&
            TA_CANDLECOLOR(i) == 1 &&
            // upside gap 1st to 2nd
            TA_REALBODYGAPUP(i-3,i-4) &&
            // 3rd to 4th hold within 1st: a part of the real body must be within 1st real body
            min(inOpen[i-2], inClose[i-2]) < inClose[i-4] &&
            min(inOpen[i-1], inClose[i-1]) < inClose[i-4] &&
            // reaction days penetrate first body less than optInPenetration percent
            min(inOpen[i-2], inClose[i-2]) > inClose[i-4] - TA_REALBODY(i-4) * optInPenetration &&
            min(inOpen[i-1], inClose[i-1]) > inClose[i-4] - TA_REALBODY(i-4) * optInPenetration &&
            // 2nd to 4th are falling
            max(inClose[i-2], inOpen[i-2]) < inOpen[i-3] && 
            max(inClose[i-1], inOpen[i-1]) < max(inClose[i-2], inOpen[i-2]) &&
            // 5th opens above the prior close
            inOpen[i] > inClose[i-1] &&
            // 5th closes above the highest high of the reaction days
            inClose[i] > max(max(inHigh[i-3], inHigh[i-2]), inHigh[i-1])
          )
            outInteger[outIdx++] = 100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        BodyPeriodTotal[4] += TA_CANDLERANGE( BodyLong, i-4 ) - TA_CANDLERANGE( BodyLong, BodyLongTrailingIdx-4 );
        for (totIdx = 3; totIdx >= 1; --totIdx)
            BodyPeriodTotal[totIdx] += TA_CANDLERANGE( BodyShort, i-totIdx ) 
                                     - TA_CANDLERANGE( BodyShort, BodyShortTrailingIdx-totIdx );
        i++; 
        BodyShortTrailingIdx++;
        BodyLongTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
