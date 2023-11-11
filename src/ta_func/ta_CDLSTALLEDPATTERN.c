#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_CDLSTALLEDPATTERN_Lookback( void )
{
    return max( max( TA_CANDLEAVGPERIOD(BodyLong), TA_CANDLEAVGPERIOD(BodyShort) ),
                max( TA_CANDLEAVGPERIOD(ShadowVeryShort), TA_CANDLEAVGPERIOD(Near) )
            ) + 2;
}

/*
 * TA_CDLSTALLEDPATTERN - Stalled Pattern
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLSTALLEDPATTERN( int    startIdx,
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
    ARRAY_LOCAL(BodyLongPeriodTotal,3);
	ARRAY_LOCAL(NearPeriodTotal,3);
	double BodyShortPeriodTotal, ShadowVeryShortPeriodTotal;
    int i, outIdx, totIdx, BodyLongTrailingIdx, BodyShortTrailingIdx, ShadowVeryShortTrailingIdx, NearTrailingIdx, 
        lookbackTotal;

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

   lookbackTotal = TA_CDLSTALLEDPATTERN_Lookback();

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
   BodyLongPeriodTotal[2] = 0;
   BodyLongPeriodTotal[1] = 0;
   BodyLongPeriodTotal[0] = 0;
   BodyLongTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(BodyLong);
   BodyShortPeriodTotal = 0;
   BodyShortTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(BodyShort);
   ShadowVeryShortPeriodTotal = 0;
   ShadowVeryShortTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(ShadowVeryShort);
   NearPeriodTotal[2] = 0;
   NearPeriodTotal[1] = 0;
   NearPeriodTotal[0] = 0;
   NearTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(Near);
   
   i = BodyLongTrailingIdx;
   while( i < startIdx ) {
        BodyLongPeriodTotal[2] += TA_CANDLERANGE( BodyLong, i-2 );
        BodyLongPeriodTotal[1] += TA_CANDLERANGE( BodyLong, i-1 );
        i++;
   }
   i = BodyShortTrailingIdx;
   while( i < startIdx ) {
        BodyShortPeriodTotal += TA_CANDLERANGE( BodyShort, i );
        i++;
   }
   i = ShadowVeryShortTrailingIdx;
   while( i < startIdx ) {
        ShadowVeryShortPeriodTotal += TA_CANDLERANGE( ShadowVeryShort, i-1 );
        i++;
   }
   i = NearTrailingIdx;
   while( i < startIdx ) {
        NearPeriodTotal[2] += TA_CANDLERANGE( Near, i-2 );
        NearPeriodTotal[1] += TA_CANDLERANGE( Near, i-1 );
        i++;
   }
   i = startIdx;

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - three white candlesticks with consecutively higher closes
    * - first candle: long white
    * - second candle: long white with no or very short upper shadow opening within or near the previous white real body
    * and closing higher than the prior candle
    * - third candle: small white that gaps away or "rides on the shoulder" of the prior long real body (= it's at 
    * the upper end of the prior real body)
    * The meanings of "long", "very short", "short", "near" are specified with TA_SetCandleSettings;
    * outInteger is negative (-1 to -100): stalled pattern is always bearish;
    * the user should consider that stalled pattern is significant when it appears in uptrend, while this function 
    * does not consider it
    */
   outIdx = 0;
   do
   {
        if( TA_CANDLECOLOR(i-2) == 1 &&                                             // 1st white
            TA_CANDLECOLOR(i-1) == 1 &&                                             // 2nd white
            TA_CANDLECOLOR(i) == 1 &&                                               // 3rd white
            inClose[i] > inClose[i-1] && inClose[i-1] > inClose[i-2] &&             // consecutive higher closes
            TA_REALBODY(i-2) > TA_CANDLEAVERAGE( BodyLong, BodyLongPeriodTotal[2], i-2 ) &&  // 1st: long real body
            TA_REALBODY(i-1) > TA_CANDLEAVERAGE( BodyLong, BodyLongPeriodTotal[1], i-1 ) &&  // 2nd: long real body
                                                                                    // very short upper shadow 
            TA_UPPERSHADOW(i-1) < TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal, i-1 ) &&
                                                                                    // opens within/near 1st real body
            inOpen[i-1] > inOpen[i-2] &&                                                    
            inOpen[i-1] <= inClose[i-2] + TA_CANDLEAVERAGE( Near, NearPeriodTotal[2], i-2 ) &&
            TA_REALBODY(i) < TA_CANDLEAVERAGE( BodyShort, BodyShortPeriodTotal, i ) &&       // 3rd: small real body
                                                                                    // rides on the shoulder of 2nd real body
            inOpen[i] >= inClose[i-1] - TA_REALBODY(i) - TA_CANDLEAVERAGE( Near, NearPeriodTotal[1], i-1 )
          )
            outInteger[outIdx++] = -100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        for (totIdx = 2; totIdx >= 1; --totIdx) {
            BodyLongPeriodTotal[totIdx] += TA_CANDLERANGE( BodyLong, i-totIdx ) 
                                         - TA_CANDLERANGE( BodyLong, BodyLongTrailingIdx-totIdx );
            NearPeriodTotal[totIdx] += TA_CANDLERANGE( Near, i-totIdx ) 
                                     - TA_CANDLERANGE( Near, NearTrailingIdx-totIdx );
        }
        BodyShortPeriodTotal += TA_CANDLERANGE( BodyShort, i ) - TA_CANDLERANGE( BodyShort, BodyShortTrailingIdx );
        ShadowVeryShortPeriodTotal += TA_CANDLERANGE( ShadowVeryShort, i-1 ) 
                                    - TA_CANDLERANGE( ShadowVeryShort, ShadowVeryShortTrailingIdx-1 );
        i++; 
        BodyLongTrailingIdx++;
        BodyShortTrailingIdx++;
        ShadowVeryShortTrailingIdx++;
        NearTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
