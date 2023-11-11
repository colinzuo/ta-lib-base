#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_CDLHIKKAKEMOD_Lookback( void )
{
    return max( 1, TA_CANDLEAVGPERIOD(Near) ) + 5;
}

/*
 * TA_CDLHIKKAKEMOD - Modified Hikkake Pattern
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLHIKKAKEMOD( int    startIdx,
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
    double NearPeriodTotal;
    int i, outIdx, NearTrailingIdx, lookbackTotal, patternIdx, patternResult;
    
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

   lookbackTotal = TA_CDLHIKKAKEMOD_Lookback();

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
   NearPeriodTotal = 0;
   NearTrailingIdx = startIdx - 3 - TA_CANDLEAVGPERIOD(Near);
   i = NearTrailingIdx;
   while( i < startIdx - 3 ) {
        NearPeriodTotal += TA_CANDLERANGE( Near, i-2 );
        i++;
   }

   patternIdx = 0;
   patternResult = 0;

   i = startIdx - 3;
   while( i < startIdx ) {
        /* copy here the pattern recognition code below */
        if( inHigh[i-2] < inHigh[i-3] && inLow[i-2] > inLow[i-3] &&             // 2nd: lower high and higher low than 1st
            inHigh[i-1] < inHigh[i-2] && inLow[i-1] > inLow[i-2] &&             // 3rd: lower high and higher low than 2nd
            ( ( inHigh[i] < inHigh[i-1] && inLow[i] < inLow[i-1] &&             // (bull) 4th: lower high and lower low
                inClose[i-2] <= inLow[i-2] + TA_CANDLEAVERAGE( Near, NearPeriodTotal, i-2 )  
                                                                                // (bull) 2nd: close near the low
              )
              ||
              ( inHigh[i] > inHigh[i-1] && inLow[i] > inLow[i-1] &&             // (bear) 4th: higher high and higher low
                inClose[i-2] >= inHigh[i-2] - TA_CANDLEAVERAGE( Near, NearPeriodTotal, i-2 )
                                                                                // (bull) 2nd: close near the top
              )
            )
        ) {
            patternResult = 100 * ( inHigh[i] < inHigh[i-1] ? 1 : -1 );
            patternIdx = i;
        } else
            /* search for confirmation if modified hikkake was no more than 3 bars ago */
            if( i <= patternIdx+3 &&
                ( ( patternResult > 0 && inClose[i] > inHigh[patternIdx-1] )    // close higher than the high of 3rd
                  ||
                  ( patternResult < 0 && inClose[i] < inLow[patternIdx-1] )     // close lower than the low of 3rd
                )
            ) 
                patternIdx = 0;
        NearPeriodTotal += TA_CANDLERANGE( Near, i-2 ) - TA_CANDLERANGE( Near, NearTrailingIdx-2 );
        NearTrailingIdx++;
        i++; 
   }

   i = startIdx;

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - first candle
    * - second candle: candle with range less than first candle and close near the bottom (near the top)
    * - third candle: lower high and higher low than 2nd
    * - fourth candle: lower high and lower low (higher high and higher low) than 3rd
    * outInteger[hikkake bar] is positive (1 to 100) or negative (-1 to -100) meaning bullish or bearish hikkake
    * Confirmation could come in the next 3 days with:
    * - a day that closes higher than the high (lower than the low) of the 3rd candle
    * outInteger[confirmationbar] is equal to 100 + the bullish hikkake result or -100 - the bearish hikkake result
    * Note: if confirmation and a new hikkake come at the same bar, only the new hikkake is reported (the new hikkake
    * overwrites the confirmation of the old hikkake);
    * the user should consider that modified hikkake is a reversal pattern, while hikkake could be both a reversal 
    * or a continuation pattern, so bullish (bearish) modified hikkake is significant when appearing in a downtrend 
    * (uptrend)
    */
   outIdx = 0;
   do
   {
        if( inHigh[i-2] < inHigh[i-3] && inLow[i-2] > inLow[i-3] &&             // 2nd: lower high and higher low than 1st
            inHigh[i-1] < inHigh[i-2] && inLow[i-1] > inLow[i-2] &&             // 3rd: lower high and higher low than 2nd
            ( ( inHigh[i] < inHigh[i-1] && inLow[i] < inLow[i-1] &&             // (bull) 4th: lower high and lower low
                inClose[i-2] <= inLow[i-2] + TA_CANDLEAVERAGE( Near, NearPeriodTotal, i-2 )  
                                                                                // (bull) 2nd: close near the low
              )
              ||
              ( inHigh[i] > inHigh[i-1] && inLow[i] > inLow[i-1] &&             // (bear) 4th: higher high and higher low
                inClose[i-2] >= inHigh[i-2] - TA_CANDLEAVERAGE( Near, NearPeriodTotal, i-2 )
                                                                                // (bull) 2nd: close near the top
              )
            )
        ) {
            patternResult = 100 * ( inHigh[i] < inHigh[i-1] ? 1 : -1 );
            patternIdx = i;
            outInteger[outIdx++] = patternResult;
        } else
            /* search for confirmation if modified hikkake was no more than 3 bars ago */
            if( i <= patternIdx+3 &&
                ( ( patternResult > 0 && inClose[i] > inHigh[patternIdx-1] )    // close higher than the high of 3rd
                  ||
                  ( patternResult < 0 && inClose[i] < inLow[patternIdx-1] )     // close lower than the low of 3rd
                )
            ) {
                outInteger[outIdx++] = patternResult + 100 * ( patternResult > 0 ? 1 : -1 );
                patternIdx = 0;
            } else
                outInteger[outIdx++] = 0;
        NearPeriodTotal += TA_CANDLERANGE( Near, i-2 ) - TA_CANDLERANGE( Near, NearTrailingIdx-2 );
        NearTrailingIdx++;
        i++; 
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
