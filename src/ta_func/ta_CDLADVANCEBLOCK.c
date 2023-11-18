#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_CDLADVANCEBLOCK_Lookback( void )
{
    return max( max( max( TA_CANDLEAVGPERIOD(ShadowLong), TA_CANDLEAVGPERIOD(ShadowShort) ),
                     max( TA_CANDLEAVGPERIOD(Far), TA_CANDLEAVGPERIOD(Near) ) ),
                TA_CANDLEAVGPERIOD(BodyLong)
            ) + 2;
}

/*
 * TA_CDLADVANCEBLOCK - Advance Block
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLADVANCEBLOCK( int    startIdx,
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
	ARRAY_LOCAL(ShadowShortPeriodTotal,3);
	ARRAY_LOCAL(ShadowLongPeriodTotal,2);
	ARRAY_LOCAL(NearPeriodTotal,3);
	ARRAY_LOCAL(FarPeriodTotal,3);
    double BodyLongPeriodTotal;
    int i, outIdx, totIdx, BodyLongTrailingIdx, ShadowShortTrailingIdx, ShadowLongTrailingIdx, NearTrailingIdx, 
        FarTrailingIdx, lookbackTotal;

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

   lookbackTotal = TA_CDLADVANCEBLOCK_Lookback();

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
   ShadowShortPeriodTotal[2] = 0;
   ShadowShortPeriodTotal[1] = 0;
   ShadowShortPeriodTotal[0] = 0;
   ShadowShortTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(ShadowShort);
   ShadowLongPeriodTotal[1] = 0;
   ShadowLongPeriodTotal[0] = 0;
   ShadowLongTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(ShadowLong);
   NearPeriodTotal[2] = 0;
   NearPeriodTotal[1] = 0;
   NearPeriodTotal[0] = 0;
   NearTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(Near);
   FarPeriodTotal[2] = 0;
   FarPeriodTotal[1] = 0;
   FarPeriodTotal[0] = 0;
   FarTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(Far);
   BodyLongPeriodTotal = 0;
   BodyLongTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(BodyLong);
   
   i = ShadowShortTrailingIdx;
   while( i < startIdx ) {
        ShadowShortPeriodTotal[2] += TA_CANDLERANGE( ShadowShort, i-2 );
        ShadowShortPeriodTotal[1] += TA_CANDLERANGE( ShadowShort, i-1 );
        ShadowShortPeriodTotal[0] += TA_CANDLERANGE( ShadowShort, i );
        i++;
   }
   i = ShadowLongTrailingIdx;
   while( i < startIdx ) {
        ShadowLongPeriodTotal[1] += TA_CANDLERANGE( ShadowLong, i-1 );
        ShadowLongPeriodTotal[0] += TA_CANDLERANGE( ShadowLong, i );
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
   i = BodyLongTrailingIdx;
   while( i < startIdx ) {
        BodyLongPeriodTotal += TA_CANDLERANGE( BodyLong, i-2 );
        i++;
   }
   i = startIdx;

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - three white candlesticks with consecutively higher closes
    * - each candle opens within or near the previous white real body 
    * - first candle: long white with no or very short upper shadow (a short shadow is accepted too for more flexibility)
    * - second and third candles, or only third candle, show signs of weakening: progressively smaller white real bodies 
    * and/or relatively long upper shadows; see below for specific conditions
    * The meanings of "long body", "short shadow", "far" and "near" are specified with TA_SetCandleSettings;
    * outInteger is negative (-1 to -100): advance block is always bearish;
    * the user should consider that advance block is significant when it appears in uptrend, while this function 
    * does not consider it
    */
   outIdx = 0;
   do
   {
        if( TA_CANDLECOLOR(i-2) == 1 &&                                                     // 1st white
            TA_CANDLECOLOR(i-1) == 1 &&                                                     // 2nd white
            TA_CANDLECOLOR(i) == 1 &&                                                       // 3rd white
            inClose[i] > inClose[i-1] && inClose[i-1] > inClose[i-2] &&                     // consecutive higher closes
            inOpen[i-1] > inOpen[i-2] &&                                                    // 2nd opens within/near 1st real body
            inOpen[i-1] <= inClose[i-2] + TA_CANDLEAVERAGE( Near, NearPeriodTotal[2], i-2 ) &&
            inOpen[i] > inOpen[i-1] &&                                                      // 3rd opens within/near 2nd real body
            inOpen[i] <= inClose[i-1] + TA_CANDLEAVERAGE( Near, NearPeriodTotal[1], i-1 ) &&
            TA_REALBODY(i-2) > TA_CANDLEAVERAGE( BodyLong, BodyLongPeriodTotal, i-2 ) && // 1st: long real body
            TA_UPPERSHADOW(i-2) < TA_CANDLEAVERAGE( ShadowShort, ShadowShortPeriodTotal[2], i-2 ) &&
                                                                                            // 1st: short upper shadow
            (
                // ( 2 far smaller than 1 && 3 not longer than 2 )
                // advance blocked with the 2nd, 3rd must not carry on the advance
                (   
                    TA_REALBODY(i-1) < TA_REALBODY(i-2) - TA_CANDLEAVERAGE( Far, FarPeriodTotal[2], i-2 ) &&
                    TA_REALBODY(i) < TA_REALBODY(i-1) + TA_CANDLEAVERAGE( Near, NearPeriodTotal[1], i-1 )
                ) ||
                // 3 far smaller than 2
                // advance blocked with the 3rd
                (
                    TA_REALBODY(i) < TA_REALBODY(i-1) - TA_CANDLEAVERAGE( Far, FarPeriodTotal[1], i-1 ) 
                ) ||
                // ( 3 smaller than 2 && 2 smaller than 1 && (3 or 2 not short upper shadow) )
                // advance blocked with progressively smaller real bodies and some upper shadows
                (
                    TA_REALBODY(i) < TA_REALBODY(i-1) &&
                    TA_REALBODY(i-1) < TA_REALBODY(i-2) &&
                    ( 
                        TA_UPPERSHADOW(i) > TA_CANDLEAVERAGE( ShadowShort, ShadowShortPeriodTotal[0], i ) ||
                        TA_UPPERSHADOW(i-1) > TA_CANDLEAVERAGE( ShadowShort, ShadowShortPeriodTotal[1], i-1 )
                    )
                ) ||
                // ( 3 smaller than 2 && 3 long upper shadow )
                // advance blocked with 3rd candle's long upper shadow and smaller body
                (
                    TA_REALBODY(i) < TA_REALBODY(i-1) &&
                    TA_UPPERSHADOW(i) > TA_CANDLEAVERAGE( ShadowLong, ShadowLongPeriodTotal[0], i )
                )
            )
          )
            outInteger[outIdx++] = -100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        for (totIdx = 2; totIdx >= 0; --totIdx)
            ShadowShortPeriodTotal[totIdx] += TA_CANDLERANGE( ShadowShort, i-totIdx ) 
                                            - TA_CANDLERANGE( ShadowShort, ShadowShortTrailingIdx-totIdx );
        for (totIdx = 1; totIdx >= 0; --totIdx)
            ShadowLongPeriodTotal[totIdx] += TA_CANDLERANGE( ShadowLong, i-totIdx ) 
                                           - TA_CANDLERANGE( ShadowLong, ShadowLongTrailingIdx-totIdx );
        for (totIdx = 2; totIdx >= 1; --totIdx) {
            FarPeriodTotal[totIdx] += TA_CANDLERANGE( Far, i-totIdx ) 
                                    - TA_CANDLERANGE( Far, FarTrailingIdx-totIdx );
            NearPeriodTotal[totIdx] += TA_CANDLERANGE( Near, i-totIdx ) 
                                     - TA_CANDLERANGE( Near, NearTrailingIdx-totIdx );
        }
        BodyLongPeriodTotal += TA_CANDLERANGE( BodyLong, i-2 ) - TA_CANDLERANGE( BodyLong, BodyLongTrailingIdx-2 );
        i++; 
        ShadowShortTrailingIdx++;
        ShadowLongTrailingIdx++;
        NearTrailingIdx++;
        FarTrailingIdx++;
        BodyLongTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
