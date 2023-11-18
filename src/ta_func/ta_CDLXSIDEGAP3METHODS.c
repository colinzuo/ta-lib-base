#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_CDLXSIDEGAP3METHODS_Lookback( void )
{
    return 2;
}

/*
 * TA_CDLXSIDEGAP3METHODS - Upside/Downside Gap Three Methods
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLXSIDEGAP3METHODS( int    startIdx,
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
    int i, outIdx, lookbackTotal;

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

   lookbackTotal = TA_CDLXSIDEGAP3METHODS_Lookback();

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
   i = startIdx;

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - first candle: white (black) candle
    * - second candle: white (black) candle
    * - upside (downside) gap between the first and the second real bodies
    * - third candle: black (white) candle that opens within the second real body and closes within the first real body
    * outInteger is positive (1 to 100) when bullish or negative (-1 to -100) when bearish;
    * the user should consider that up/downside gap 3 methods is significant when it appears in a trend, while this 
    * function does not consider it
    */
   outIdx = 0;
   do
   {
        if( TA_CANDLECOLOR(i-2) == TA_CANDLECOLOR(i-1) &&                   // 1st and 2nd of same color
            TA_CANDLECOLOR(i-1) == -TA_CANDLECOLOR(i) &&                    // 3rd opposite color
            inOpen[i] < max(inClose[i-1], inOpen[i-1]) &&                   // 3rd opens within 2nd rb
            inOpen[i] > min(inClose[i-1], inOpen[i-1]) &&
            inClose[i] < max(inClose[i-2], inOpen[i-2]) &&                  // 3rd closes within 1st rb
            inClose[i] > min(inClose[i-2], inOpen[i-2]) &&
            ( ( 
                TA_CANDLECOLOR(i-2) == 1 &&                                 // when 1st is white
                TA_REALBODYGAPUP(i-1,i-2)                                   // upside gap
              ) ||
              (
                TA_CANDLECOLOR(i-2) == -1 &&                                // when 1st is black
                TA_REALBODYGAPDOWN(i-1,i-2)                                 // downside gap
              )
            )
        )
            outInteger[outIdx++] = TA_CANDLECOLOR(i-2) * 100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        i++; 
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
