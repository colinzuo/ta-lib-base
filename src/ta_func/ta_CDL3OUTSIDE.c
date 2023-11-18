#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_CDL3OUTSIDE_Lookback( void )
{
   return 3;
}

/*
 * TA_CDL3OUTSIDE - Three Outside Up/Down
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDL3OUTSIDE( int    startIdx,
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

   lookbackTotal = TA_CDL3OUTSIDE_Lookback();

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
    * - third: candle that closes higher (lower) than the second candle
    * outInteger is positive (1 to 100) for the three outside up or negative (-1 to -100) for the three outside down;
    * the user should consider that a three outside up must appear in a downtrend and three outside down must appear
    * in an uptrend, while this function does not consider it
    */
   outIdx = 0;
   do
   {
        if( ( TA_CANDLECOLOR(i-1) == 1 && TA_CANDLECOLOR(i-2) == -1 &&          // white engulfs black
              inClose[i-1] > inOpen[i-2] && inOpen[i-1] < inClose[i-2] &&
              inClose[i] > inClose[i-1]                                         // third candle higher
            )
            ||
            ( TA_CANDLECOLOR(i-1) == -1 && TA_CANDLECOLOR(i-2) == 1 &&          // black engulfs white
              inOpen[i-1] > inClose[i-2] && inClose[i-1] < inOpen[i-2] &&
              inClose[i] < inClose[i-1]                                         // third candle lower
            )
          )
            outInteger[outIdx++] = TA_CANDLECOLOR(i-1) * 100;
        else
            outInteger[outIdx++] = 0;
        i++; 
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
