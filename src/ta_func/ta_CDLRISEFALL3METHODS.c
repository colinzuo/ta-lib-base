#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_CDLRISEFALL3METHODS_Lookback( void )
{
    return max( TA_CANDLEAVGPERIOD(BodyShort), TA_CANDLEAVGPERIOD(BodyLong) ) + 4;
}

/*
 * TA_CDLRISEFALL3METHODS - Rising/Falling Three Methods
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLRISEFALL3METHODS( int    startIdx,
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
    ARRAY_LOCAL(BodyPeriodTotal,5);
    int i, outIdx, totIdx, BodyShortTrailingIdx, BodyLongTrailingIdx, lookbackTotal;

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

   lookbackTotal = TA_CDLRISEFALL3METHODS_Lookback();

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
   BodyPeriodTotal[4] = 0;
   BodyPeriodTotal[3] = 0;
   BodyPeriodTotal[2] = 0;
   BodyPeriodTotal[1] = 0;
   BodyPeriodTotal[0] = 0;
   BodyShortTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(BodyShort);
   BodyLongTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(BodyLong);
   
   i = BodyShortTrailingIdx;
   while( i < startIdx ) {
        BodyPeriodTotal[3] += TA_CANDLERANGE( BodyShort, i-3 );
        BodyPeriodTotal[2] += TA_CANDLERANGE( BodyShort, i-2 );
        BodyPeriodTotal[1] += TA_CANDLERANGE( BodyShort, i-1 );
        i++;
   }
   i = BodyLongTrailingIdx;
   while( i < startIdx ) {
        BodyPeriodTotal[4] += TA_CANDLERANGE( BodyLong, i-4 );
        BodyPeriodTotal[0] += TA_CANDLERANGE( BodyLong, i );
        i++;
   }
   i = startIdx;

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - first candle: long white (black) candlestick
    * - then: group of falling (rising) small real body candlesticks (commonly black (white)) that hold within 
    *   the prior long candle's range: ideally they should be three but two or more than three are ok too
    * - final candle: long white (black) candle that opens above (below) the previous small candle's close 
    *   and closes above (below) the first long candle's close
    * The meaning of "short" and "long" is specified with TA_SetCandleSettings; here only patterns with 3 small candles
    * are considered;
    * outInteger is positive (1 to 100) or negative (-1 to -100)
    */
   outIdx = 0;
   do
   {
        if( // 1st long, then 3 small, 5th long
            TA_REALBODY(i-4) > TA_CANDLEAVERAGE( BodyLong, BodyPeriodTotal[4], i-4 ) &&
            TA_REALBODY(i-3) < TA_CANDLEAVERAGE( BodyShort, BodyPeriodTotal[3], i-3 ) &&
            TA_REALBODY(i-2) < TA_CANDLEAVERAGE( BodyShort, BodyPeriodTotal[2], i-2 ) &&
            TA_REALBODY(i-1) < TA_CANDLEAVERAGE( BodyShort, BodyPeriodTotal[1], i-1 ) &&
            TA_REALBODY(i)   > TA_CANDLEAVERAGE( BodyLong, BodyPeriodTotal[0], i ) &&
            // white, 3 black, white  ||  black, 3 white, black
            TA_CANDLECOLOR(i-4) == -TA_CANDLECOLOR(i-3) &&
            TA_CANDLECOLOR(i-3) ==  TA_CANDLECOLOR(i-2) &&
            TA_CANDLECOLOR(i-2) ==  TA_CANDLECOLOR(i-1) &&
            TA_CANDLECOLOR(i-1) == -TA_CANDLECOLOR(i) &&
            // 2nd to 4th hold within 1st: a part of the real body must be within 1st range
            min(inOpen[i-3], inClose[i-3]) < inHigh[i-4] && max(inOpen[i-3], inClose[i-3]) > inLow[i-4] &&
            min(inOpen[i-2], inClose[i-2]) < inHigh[i-4] && max(inOpen[i-2], inClose[i-2]) > inLow[i-4] &&
            min(inOpen[i-1], inClose[i-1]) < inHigh[i-4] && max(inOpen[i-1], inClose[i-1]) > inLow[i-4] &&
            // 2nd to 4th are falling (rising)
            inClose[i-2] * TA_CANDLECOLOR(i-4) < inClose[i-3] * TA_CANDLECOLOR(i-4) &&
            inClose[i-1] * TA_CANDLECOLOR(i-4) < inClose[i-2] * TA_CANDLECOLOR(i-4) &&
            // 5th opens above (below) the prior close
            inOpen[i] * TA_CANDLECOLOR(i-4) > inClose[i-1] * TA_CANDLECOLOR(i-4) &&
            // 5th closes above (below) the 1st close
            inClose[i] * TA_CANDLECOLOR(i-4) > inClose[i-4] * TA_CANDLECOLOR(i-4)
          )
            outInteger[outIdx++] = 100 * TA_CANDLECOLOR(i-4);
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        BodyPeriodTotal[4] += TA_CANDLERANGE( BodyLong, i-4 ) - TA_CANDLERANGE( BodyLong, BodyLongTrailingIdx-4 );
        for (totIdx = 3; totIdx >= 1; --totIdx)
            BodyPeriodTotal[totIdx] += TA_CANDLERANGE( BodyShort, i-totIdx ) 
                                     - TA_CANDLERANGE( BodyShort, BodyShortTrailingIdx-totIdx );
        BodyPeriodTotal[0] += TA_CANDLERANGE( BodyLong, i ) - TA_CANDLERANGE( BodyLong, BodyLongTrailingIdx );
        i++; 
        BodyShortTrailingIdx++;
        BodyLongTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
