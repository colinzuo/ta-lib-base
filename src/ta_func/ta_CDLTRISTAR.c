#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_CDLTRISTAR_Lookback( void )
{
    return TA_CANDLEAVGPERIOD(BodyDoji) + 2;
}

/*
 * TA_CDLTRISTAR - Tristar Pattern
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLTRISTAR( int    startIdx,
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
    double BodyPeriodTotal;
    int i, outIdx, BodyTrailingIdx, lookbackTotal;

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

   lookbackTotal = TA_CDLTRISTAR_Lookback();

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
   BodyTrailingIdx = startIdx -2 - TA_CANDLEAVGPERIOD(BodyDoji);
   
   i = BodyTrailingIdx;
   while( i < startIdx-2 ) {
        BodyPeriodTotal += TA_CANDLERANGE( BodyDoji, i );
        i++;
   }

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - 3 consecutive doji days
    * - the second doji is a star
    * The meaning of "doji" is specified with TA_SetCandleSettings
    * outInteger is positive (1 to 100) when bullish or negative (-1 to -100) when bearish
    */
   i = startIdx;
   outIdx = 0;
   do
   {
        if( TA_REALBODY(i-2) <= TA_CANDLEAVERAGE( BodyDoji, BodyPeriodTotal, i-2 ) &&    // 1st: doji
            TA_REALBODY(i-1) <= TA_CANDLEAVERAGE( BodyDoji, BodyPeriodTotal, i-2 ) &&    // 2nd: doji
            TA_REALBODY(i) <= TA_CANDLEAVERAGE( BodyDoji, BodyPeriodTotal, i-2 ) ) {     // 3rd: doji
            outInteger[outIdx] = 0;
            if ( TA_REALBODYGAPUP(i-1,i-2)                                                  // 2nd gaps up
                 &&
                 max(inOpen[i],inClose[i]) < max(inOpen[i-1],inClose[i-1])                  // 3rd is not higher than 2nd
               )
                outInteger[outIdx] = -100;
            if ( TA_REALBODYGAPDOWN(i-1,i-2)                                                // 2nd gaps down
                 &&
                 min(inOpen[i],inClose[i]) > min(inOpen[i-1],inClose[i-1])                  // 3rd is not lower than 2nd
               )
                outInteger[outIdx] = +100;
            outIdx++;
        }
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        BodyPeriodTotal += TA_CANDLERANGE( BodyDoji, i-2 ) - TA_CANDLERANGE( BodyDoji, BodyTrailingIdx );
        i++;
        BodyTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
