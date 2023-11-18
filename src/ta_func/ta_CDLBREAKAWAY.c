#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_CDLBREAKAWAY_Lookback( void )
{
    return TA_CANDLEAVGPERIOD(BodyLong) + 4;
}

/*
 * TA_CDLBREAKAWAY - Breakaway
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLBREAKAWAY( int    startIdx,
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
    double BodyLongPeriodTotal;
    int i, outIdx, BodyLongTrailingIdx, lookbackTotal;

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

   lookbackTotal = TA_CDLBREAKAWAY_Lookback();

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
   BodyLongTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(BodyLong);

   i = BodyLongTrailingIdx;
   while( i < startIdx ) {
        BodyLongPeriodTotal += TA_CANDLERANGE( BodyLong, i-4 );
        i++;
   }
   i = startIdx;

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - first candle: long black (white)
    * - second candle: black (white) day whose body gaps down (up)
    * - third candle: black or white day with lower (higher) high and lower (higher) low than prior candle's
    * - fourth candle: black (white) day with lower (higher) high and lower (higher) low than prior candle's
    * - fifth candle: white (black) day that closes inside the gap, erasing the prior 3 days
    * The meaning of "long" is specified with TA_SetCandleSettings
    * outInteger is positive (1 to 100) when bullish or negative (-1 to -100) when bearish;
    * the user should consider that breakaway is significant in a trend opposite to the last candle, while this 
    * function does not consider it
    */
   outIdx = 0;
   do
   {
        if( TA_REALBODY(i-4) > TA_CANDLEAVERAGE( BodyLong, BodyLongPeriodTotal, i-4 ) &&     // 1st long
            TA_CANDLECOLOR(i-4) == TA_CANDLECOLOR(i-3) &&                   // 1st, 2nd, 4th same color, 5th opposite
            TA_CANDLECOLOR(i-3) == TA_CANDLECOLOR(i-1) &&
            TA_CANDLECOLOR(i-1) == -TA_CANDLECOLOR(i) &&
            (
              ( TA_CANDLECOLOR(i-4) == -1 &&                                // when 1st is black:
                TA_REALBODYGAPDOWN(i-3,i-4) &&                              // 2nd gaps down
                inHigh[i-2] < inHigh[i-3] && inLow[i-2] < inLow[i-3] &&     // 3rd has lower high and low than 2nd
                inHigh[i-1] < inHigh[i-2] && inLow[i-1] < inLow[i-2] &&     // 4th has lower high and low than 3rd
                inClose[i] > inOpen[i-3] && inClose[i] < inClose[i-4]       // 5th closes inside the gap
              ) 
              ||
              ( TA_CANDLECOLOR(i-4) == 1 &&                                 // when 1st is white:
                TA_REALBODYGAPUP(i-3,i-4) &&                                // 2nd gaps up
                inHigh[i-2] > inHigh[i-3] && inLow[i-2] > inLow[i-3] &&     // 3rd has higher high and low than 2nd
                inHigh[i-1] > inHigh[i-2] && inLow[i-1] > inLow[i-2] &&     // 4th has higher high and low than 3rd
                inClose[i] < inOpen[i-3] && inClose[i] > inClose[i-4]       // 5th closes inside the gap
              ) 
            )
          )
            outInteger[outIdx++] = TA_CANDLECOLOR(i) * 100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        BodyLongPeriodTotal += TA_CANDLERANGE( BodyLong, i-4 ) 
                             - TA_CANDLERANGE( BodyLong, BodyLongTrailingIdx-4 );
        i++;
        BodyLongTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
