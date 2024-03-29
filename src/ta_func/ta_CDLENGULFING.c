#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_CDLENGULFING_Lookback( void )
{
    return 2;
}

/*
 * TA_CDLENGULFING - Engulfing Pattern
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLENGULFING( int    startIdx,
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

   lookbackTotal = TA_CDLENGULFING_Lookback();

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
    * - first: black (white) real body
    * - second: white (black) real body that engulfs the prior real body
    * outInteger is positive (1 to 100) when bullish or negative (-1 to -100) when bearish;
    * the user should consider that an engulfing must appear in a downtrend if bullish or in an uptrend if bearish,
    * while this function does not consider it
    */
   outIdx = 0;
   do
   {
        if( ( TA_CANDLECOLOR(i) == 1 && TA_CANDLECOLOR(i-1) == -1 &&            // white engulfs black
              inClose[i] > inOpen[i-1] && inOpen[i] < inClose[i-1]
            )
            ||
            ( TA_CANDLECOLOR(i) == -1 && TA_CANDLECOLOR(i-1) == 1 &&            // black engulfs white
              inOpen[i] > inClose[i-1] && inClose[i] < inOpen[i-1]
            )
          )
            outInteger[outIdx++] = TA_CANDLECOLOR(i) * 100;
        else
            outInteger[outIdx++] = 0;
        i++; 
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
