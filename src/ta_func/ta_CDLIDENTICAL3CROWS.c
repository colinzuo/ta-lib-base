#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_CDLIDENTICAL3CROWS_Lookback( void )
{
    return max( TA_CANDLEAVGPERIOD(ShadowVeryShort), TA_CANDLEAVGPERIOD(Equal)
            ) + 2;
}

/*
 * TA_CDLIDENTICAL3CROWS - Identical Three Crows
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLIDENTICAL3CROWS( int    startIdx,
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
    ARRAY_LOCAL(ShadowVeryShortPeriodTotal,3);
	ARRAY_LOCAL(EqualPeriodTotal,3);
    int i, outIdx, totIdx, ShadowVeryShortTrailingIdx, EqualTrailingIdx, lookbackTotal;

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

   lookbackTotal = TA_CDLIDENTICAL3CROWS_Lookback();

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
   ShadowVeryShortPeriodTotal[2] = 0;
   ShadowVeryShortPeriodTotal[1] = 0;
   ShadowVeryShortPeriodTotal[0] = 0;
   ShadowVeryShortTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(ShadowVeryShort);
   EqualPeriodTotal[2] = 0;
   EqualPeriodTotal[1] = 0;
   EqualPeriodTotal[0] = 0;
   EqualTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(Equal);
   
   i = ShadowVeryShortTrailingIdx;
   while( i < startIdx ) {
        ShadowVeryShortPeriodTotal[2] += TA_CANDLERANGE( ShadowVeryShort, i-2 );
        ShadowVeryShortPeriodTotal[1] += TA_CANDLERANGE( ShadowVeryShort, i-1 );
        ShadowVeryShortPeriodTotal[0] += TA_CANDLERANGE( ShadowVeryShort, i );
        i++;
   }
   i = EqualTrailingIdx;
   while( i < startIdx ) {
        EqualPeriodTotal[2] += TA_CANDLERANGE( Equal, i-2 );
        EqualPeriodTotal[1] += TA_CANDLERANGE( Equal, i-1 );
        i++;
   }
   i = startIdx;

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - three consecutive and declining black candlesticks
    * - each candle must have no or very short lower shadow
    * - each candle after the first must open at or very close to the prior candle's close
    * The meaning of "very short" is specified with TA_SetCandleSettings;
    * the meaning of "very close" is specified with TA_SetCandleSettings (Equal);
    * outInteger is negative (-1 to -100): identical three crows is always bearish; 
    * the user should consider that identical 3 crows is significant when it appears after a mature advance or at high levels, 
    * while this function does not consider it
    */
   outIdx = 0;
   do
   {
        if( TA_CANDLECOLOR(i-2) == -1 &&                                    // 1st black
                                                                            // very short lower shadow
            TA_LOWERSHADOW(i-2) < TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal[2], i-2 ) &&     
            TA_CANDLECOLOR(i-1) == -1 &&                                    // 2nd black
                                                                            // very short lower shadow
            TA_LOWERSHADOW(i-1) < TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal[1], i-1 ) &&     
            TA_CANDLECOLOR(i) == -1 &&                                      // 3rd black
                                                                            // very short lower shadow
            TA_LOWERSHADOW(i) < TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal[0], i ) &&         
            inClose[i-2] > inClose[i-1] &&                                  // three declining
            inClose[i-1] > inClose[i] &&
                                                                            // 2nd black opens very close to 1st close
            inOpen[i-1] <= inClose[i-2] + TA_CANDLEAVERAGE( Equal, EqualPeriodTotal[2], i-2 ) && 
            inOpen[i-1] >= inClose[i-2] - TA_CANDLEAVERAGE( Equal, EqualPeriodTotal[2], i-2 ) &&
                                                                            // 3rd black opens very close to 2nd close 
            inOpen[i] <= inClose[i-1] + TA_CANDLEAVERAGE( Equal, EqualPeriodTotal[1], i-1 ) &&   
            inOpen[i] >= inClose[i-1] - TA_CANDLEAVERAGE( Equal, EqualPeriodTotal[1], i-1 )
          )
            outInteger[outIdx++] = -100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        for (totIdx = 2; totIdx >= 0; --totIdx)
            ShadowVeryShortPeriodTotal[totIdx] += TA_CANDLERANGE( ShadowVeryShort, i-totIdx ) 
                                                - TA_CANDLERANGE( ShadowVeryShort, ShadowVeryShortTrailingIdx-totIdx );
        for (totIdx = 2; totIdx >= 1; --totIdx)
            EqualPeriodTotal[totIdx] += TA_CANDLERANGE( Equal, i-totIdx ) 
                                      - TA_CANDLERANGE( Equal, EqualTrailingIdx-totIdx );
        i++; 
        ShadowVeryShortTrailingIdx++;
        EqualTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
