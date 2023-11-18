#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_CDL3WHITESOLDIERS_Lookback( void )
{
    return max( max( TA_CANDLEAVGPERIOD(ShadowVeryShort), TA_CANDLEAVGPERIOD(BodyShort) ),
                max( TA_CANDLEAVGPERIOD(Far), TA_CANDLEAVGPERIOD(Near) )
            ) + 2;
}

/*
 * TA_CDL3WHITESOLDIERS - Three Advancing White Soldiers
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDL3WHITESOLDIERS( int    startIdx,
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
	ARRAY_LOCAL(NearPeriodTotal,3);
    ARRAY_LOCAL(FarPeriodTotal,3);
	double BodyShortPeriodTotal;
    int i, outIdx, totIdx, ShadowVeryShortTrailingIdx, NearTrailingIdx, FarTrailingIdx, BodyShortTrailingIdx, lookbackTotal;

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

   lookbackTotal = TA_CDL3WHITESOLDIERS_Lookback();

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
   NearPeriodTotal[2] = 0;
   NearPeriodTotal[1] = 0;
   NearPeriodTotal[0] = 0;
   NearTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(Near);
   FarPeriodTotal[2] = 0;
   FarPeriodTotal[1] = 0;
   FarPeriodTotal[0] = 0;
   FarTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(Far);
   BodyShortPeriodTotal = 0;
   BodyShortTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(BodyShort);
   
   i = ShadowVeryShortTrailingIdx;
   while( i < startIdx ) {
        ShadowVeryShortPeriodTotal[2] += TA_CANDLERANGE( ShadowVeryShort, i-2 );
        ShadowVeryShortPeriodTotal[1] += TA_CANDLERANGE( ShadowVeryShort, i-1 );
        ShadowVeryShortPeriodTotal[0] += TA_CANDLERANGE( ShadowVeryShort, i );
        i++;
   }
   i = NearTrailingIdx;
   while( i < startIdx ) {
        NearPeriodTotal[2] += TA_CANDLERANGE( Near, i-2 );
        NearPeriodTotal[1] += TA_CANDLERANGE( Near, i-1 );
        i++;
   }
   i = FarTrailingIdx;
   while( i < startIdx ) {
        FarPeriodTotal[2] += TA_CANDLERANGE( Far, i-2 );
        FarPeriodTotal[1] += TA_CANDLERANGE( Far, i-1 );
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
    * - three white candlesticks with consecutively higher closes
    * - Greg Morris wants them to be long, Steve Nison doesn't; anyway they should not be short
    * - each candle opens within or near the previous white real body 
    * - each candle must have no or very short upper shadow
    * - to differentiate this pattern from advance block, each candle must not be far shorter than the prior candle
    * The meanings of "not short", "very short shadow", "far" and "near" are specified with TA_SetCandleSettings;
    * here the 3 candles must be not short, if you want them to be long use TA_SetCandleSettings on BodyShort;
    * outInteger is positive (1 to 100): advancing 3 white soldiers is always bullish;
    * the user should consider that 3 white soldiers is significant when it appears in downtrend, while this function 
    * does not consider it
    */
   outIdx = 0;
   do
   {
        if( TA_CANDLECOLOR(i-2) == 1 &&                                                     // 1st white
            TA_UPPERSHADOW(i-2) < TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal[2], i-2 ) &&     
                                                                                            // very short upper shadow
            TA_CANDLECOLOR(i-1) == 1 &&                                                     // 2nd white
            TA_UPPERSHADOW(i-1) < TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal[1], i-1 ) &&     
                                                                                            // very short upper shadow
            TA_CANDLECOLOR(i) == 1 &&                                                       // 3rd white
            TA_UPPERSHADOW(i) < TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal[0], i ) &&         
                                                                                            // very short upper shadow
            inClose[i] > inClose[i-1] && inClose[i-1] > inClose[i-2] &&                     // consecutive higher closes
            inOpen[i-1] > inOpen[i-2] &&                                                    // 2nd opens within/near 1st real body
            inOpen[i-1] <= inClose[i-2] + TA_CANDLEAVERAGE( Near, NearPeriodTotal[2], i-2 ) &&
            inOpen[i] > inOpen[i-1] &&                                                      // 3rd opens within/near 2nd real body
            inOpen[i] <= inClose[i-1] + TA_CANDLEAVERAGE( Near, NearPeriodTotal[1], i-1 ) &&
            TA_REALBODY(i-1) > TA_REALBODY(i-2) - TA_CANDLEAVERAGE( Far, FarPeriodTotal[2], i-2 ) &&     
                                                                                            // 2nd not far shorter than 1st
            TA_REALBODY(i) > TA_REALBODY(i-1) - TA_CANDLEAVERAGE( Far, FarPeriodTotal[1], i-1 ) &&       
                                                                                            // 3rd not far shorter than 2nd
            TA_REALBODY(i) > TA_CANDLEAVERAGE( BodyShort, BodyShortPeriodTotal, i )      // not short real body
          )
            outInteger[outIdx++] = 100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        for (totIdx = 2; totIdx >= 0; --totIdx)
            ShadowVeryShortPeriodTotal[totIdx] += TA_CANDLERANGE( ShadowVeryShort, i-totIdx ) 
                                                - TA_CANDLERANGE( ShadowVeryShort, ShadowVeryShortTrailingIdx-totIdx );
        for (totIdx = 2; totIdx >= 1; --totIdx) {
            FarPeriodTotal[totIdx] += TA_CANDLERANGE( Far, i-totIdx ) 
                                    - TA_CANDLERANGE( Far, FarTrailingIdx-totIdx );
            NearPeriodTotal[totIdx] += TA_CANDLERANGE( Near, i-totIdx ) 
                                     - TA_CANDLERANGE( Near, NearTrailingIdx-totIdx );
        }
        BodyShortPeriodTotal += TA_CANDLERANGE( BodyShort, i ) - TA_CANDLERANGE( BodyShort, BodyShortTrailingIdx );
        i++; 
        ShadowVeryShortTrailingIdx++;
        NearTrailingIdx++;
        FarTrailingIdx++;
        BodyShortTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
