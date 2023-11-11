#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_CDLUPSIDEGAP2CROWS_Lookback( void )
{
    return max( TA_CANDLEAVGPERIOD(BodyShort), TA_CANDLEAVGPERIOD(BodyLong) ) + 2;
}

/*
 * TA_CDLUPSIDEGAP2CROWS - Upside Gap Two Crows
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLUPSIDEGAP2CROWS( int    startIdx,
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
    double BodyShortPeriodTotal, BodyLongPeriodTotal;
    int i, outIdx, BodyShortTrailingIdx, BodyLongTrailingIdx, lookbackTotal;

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

   lookbackTotal = TA_CDLUPSIDEGAP2CROWS_Lookback();

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
   BodyShortPeriodTotal = 0;
   BodyLongTrailingIdx = startIdx -2 - TA_CANDLEAVGPERIOD(BodyLong);
   BodyShortTrailingIdx = startIdx -1 - TA_CANDLEAVGPERIOD(BodyShort);
   
   i = BodyLongTrailingIdx;
   while( i < startIdx-2 ) {
        BodyLongPeriodTotal += TA_CANDLERANGE( BodyLong, i );
        i++;
   }
   i = BodyShortTrailingIdx;
   while( i < startIdx-1 ) {
        BodyShortPeriodTotal += TA_CANDLERANGE( BodyShort, i );
        i++;
   }
   i = startIdx;

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - first candle: white candle, usually long
    * - second candle: small black real body
    * - gap between the first and the second candle's real bodies
    * - third candle: black candle with a real body that engulfs the preceding candle 
    *   and closes above the white candle's close
    * The meaning of "short" and "long" is specified with TA_SetCandleSettings
    * outInteger is negative (-1 to -100): upside gap two crows is always bearish; 
    * the user should consider that an upside gap two crows is significant when it appears in an uptrend, 
    * while this function does not consider the trend
    */
   outIdx = 0;
   do
   {
        if( TA_CANDLECOLOR(i-2) == 1 &&                                                             // 1st: white
            TA_REALBODY(i-2) > TA_CANDLEAVERAGE( BodyLong, BodyLongPeriodTotal, i-2 ) &&         //      long
            TA_CANDLECOLOR(i-1) == -1 &&                                                            // 2nd: black
            TA_REALBODY(i-1) <= TA_CANDLEAVERAGE( BodyShort, BodyShortPeriodTotal, i-1 ) &&      //      short
            TA_REALBODYGAPUP(i-1,i-2) &&                                                            //      gapping up
            TA_CANDLECOLOR(i) == -1 &&                                                              // 3rd: black
            inOpen[i] > inOpen[i-1] && inClose[i] < inClose[i-1] &&                                 // 3rd: engulfing prior rb
            inClose[i] > inClose[i-2]                                                               //      closing above 1st
          )
            outInteger[outIdx++] = -100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        BodyLongPeriodTotal += TA_CANDLERANGE( BodyLong, i-2 ) - TA_CANDLERANGE( BodyLong, BodyLongTrailingIdx );
        BodyShortPeriodTotal += TA_CANDLERANGE( BodyShort, i-1 ) - TA_CANDLERANGE( BodyShort, BodyShortTrailingIdx );
        i++; 
        BodyLongTrailingIdx++;
        BodyShortTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
