/* TA-LIB Copyright (c) 1999-2007, Mario Fortier
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 *
 * - Neither name of author nor the names of its contributors
 *   may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* List of contributors:
 *
 *  Initial  Name/description
 *  -------------------------------------------------------------------
 *  AC       Angelo Ciceri
 *
 *
 * Change history:
 *
 *  MMDDYY BY   Description
 *  -------------------------------------------------------------------
 *  103004 AC   Creation           
 *
 */

/**** START GENCODE SECTION 1 - DO NOT DELETE THIS LINE ****/
/* All code within this section is automatically
 * generated by gen_code. Any modification will be lost
 * next time gen_code is run.
 */
/* Generated */ 
/* Generated */ #include <string.h>
/* Generated */ #include <math.h>
/* Generated */ #include "ta_func.h"
/* Generated */ 
/* Generated */ #ifndef TA_UTILITY_H
/* Generated */    #include "ta_utility.h"
/* Generated */ #endif
/* Generated */ 
/* Generated */ #ifndef TA_MEMORY_H
/* Generated */    #include "ta_memory.h"
/* Generated */ #endif
/* Generated */ 
/* Generated */ #define TA_PREFIX(x) TA_##x
/* Generated */ #define INPUT_TYPE   double
/* Generated */ 
/* Generated */ int TA_CDLINVERTEDHAMMER_Lookback( void )
/* Generated */ 
/**** END GENCODE SECTION 1 - DO NOT DELETE THIS LINE ****/
{
   /* insert local variable here */

/**** START GENCODE SECTION 2 - DO NOT DELETE THIS LINE ****/
/* Generated */ /* No parameters to validate. */
/**** END GENCODE SECTION 2 - DO NOT DELETE THIS LINE ****/

   /* insert lookback code here. */
    return max( max( TA_CANDLEAVGPERIOD(BodyShort), TA_CANDLEAVGPERIOD(ShadowLong) ),
                TA_CANDLEAVGPERIOD(ShadowVeryShort)
            ) + 1;
}

/**** START GENCODE SECTION 3 - DO NOT DELETE THIS LINE ****/
/*
 * TA_CDLINVERTEDHAMMER - Inverted Hammer
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
/* Generated */ 
/* Generated */ TA_RetCode TA_CDLINVERTEDHAMMER( int    startIdx,
/* Generated */                                  int    endIdx,
/* Generated */                                  const double inOpen[],
/* Generated */                                  const double inHigh[],
/* Generated */                                  const double inLow[],
/* Generated */                                  const double inClose[],
/* Generated */                                  int          *outBegIdx,
/* Generated */                                  int          *outNBElement,
/* Generated */                                  int           outInteger[] )
/**** END GENCODE SECTION 3 - DO NOT DELETE THIS LINE ****/
{
   /* Insert local variables here. */
    double BodyPeriodTotal, ShadowLongPeriodTotal, ShadowVeryShortPeriodTotal;
    int i, outIdx, BodyTrailingIdx, ShadowLongTrailingIdx, ShadowVeryShortTrailingIdx, lookbackTotal;

/**** START GENCODE SECTION 4 - DO NOT DELETE THIS LINE ****/
/* Generated */ 
/* Generated */ #ifndef TA_FUNC_NO_RANGE_CHECK
/* Generated */ 
/* Generated */    /* Validate the requested output range. */
/* Generated */    if( startIdx < 0 )
/* Generated */       return ENUM_VALUE(RetCode,TA_OUT_OF_RANGE_START_INDEX,OutOfRangeStartIndex);
/* Generated */    if( (endIdx < 0) || (endIdx < startIdx))
/* Generated */       return ENUM_VALUE(RetCode,TA_OUT_OF_RANGE_END_INDEX,OutOfRangeEndIndex);
/* Generated */ 
/* Generated */    /* Verify required price component. */
/* Generated */    if(!inOpen||!inHigh||!inLow||!inClose)
/* Generated */       return ENUM_VALUE(RetCode,TA_BAD_PARAM,BadParam);
/* Generated */ 
/* Generated */    /* Generated */    if( !outInteger )
/* Generated */       return ENUM_VALUE(RetCode,TA_BAD_PARAM,BadParam);
/* Generated */ 
/* Generated */ #endif /* TA_FUNC_NO_RANGE_CHECK */
/* Generated */ 
/**** END GENCODE SECTION 4 - DO NOT DELETE THIS LINE ****/

   /* Identify the minimum number of price bar needed
    * to calculate at least one output.
    */

   lookbackTotal = LOOKBACK_CALL(CDLINVERTEDHAMMER)();

   /* Move up the start index if there is not
    * enough initial data.
    */
   if( startIdx < lookbackTotal )
      startIdx = lookbackTotal;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
   {
      VALUE_HANDLE_DEREF_TO_ZERO(outBegIdx);
      VALUE_HANDLE_DEREF_TO_ZERO(outNBElement);
      return ENUM_VALUE(RetCode,TA_SUCCESS,Success);
   }

   /* Do the calculation using tight loops. */
   /* Add-up the initial period, except for the last value. */
   BodyPeriodTotal = 0;
   BodyTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(BodyShort);
   ShadowLongPeriodTotal = 0;
   ShadowLongTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(ShadowLong);
   ShadowVeryShortPeriodTotal = 0;
   ShadowVeryShortTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(ShadowVeryShort);
   
   i = BodyTrailingIdx;
   while( i < startIdx ) {
        BodyPeriodTotal += TA_CANDLERANGE( BodyShort, i );
        i++;
   }
   i = ShadowLongTrailingIdx;
   while( i < startIdx ) {
        ShadowLongPeriodTotal += TA_CANDLERANGE( ShadowLong, i );
        i++;
   }
   i = ShadowVeryShortTrailingIdx;
   while( i < startIdx ) {
        ShadowVeryShortPeriodTotal += TA_CANDLERANGE( ShadowVeryShort, i );
        i++;
   }

   /* Proceed with the calculation for the requested range.
    * Must have:
    * - small real body
    * - long upper shadow
    * - no, or very short, lower shadow
    * - gap down
    * The meaning of "short", "very short" and "long" is specified with TA_SetCandleSettings;
    * outInteger is positive (1 to 100): inverted hammer is always bullish;
    * the user should consider that an inverted hammer must appear in a downtrend, while this function does not consider it
    */
   outIdx = 0;
   do
   {
        if( TA_REALBODY(i) < TA_CANDLEAVERAGE( BodyShort, BodyPeriodTotal, i ) &&                        // small rb
            TA_UPPERSHADOW(i) > TA_CANDLEAVERAGE( ShadowLong, ShadowLongPeriodTotal, i ) &&              // long upper shadow
            TA_LOWERSHADOW(i) < TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal, i ) &&    // very short lower shadow
            TA_REALBODYGAPDOWN(i, i-1) )                                                                    // gap down
            outInteger[outIdx++] = 100;
        else
            outInteger[outIdx++] = 0;
        /* add the current range and subtract the first range: this is done after the pattern recognition 
         * when avgPeriod is not 0, that means "compare with the previous candles" (it excludes the current candle)
         */
        BodyPeriodTotal += TA_CANDLERANGE( BodyShort, i ) 
            - TA_CANDLERANGE( BodyShort, BodyTrailingIdx );
        ShadowLongPeriodTotal += TA_CANDLERANGE( ShadowLong, i ) 
            - TA_CANDLERANGE( ShadowLong, ShadowLongTrailingIdx );
        ShadowVeryShortPeriodTotal += TA_CANDLERANGE( ShadowVeryShort, i ) 
            - TA_CANDLERANGE( ShadowVeryShort, ShadowVeryShortTrailingIdx );
        i++; 
        BodyTrailingIdx++;
        ShadowLongTrailingIdx++;
        ShadowVeryShortTrailingIdx++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   VALUE_HANDLE_DEREF(outNBElement) = outIdx;
   VALUE_HANDLE_DEREF(outBegIdx)    = startIdx;

   return ENUM_VALUE(RetCode,TA_SUCCESS,Success);
}

/**** START GENCODE SECTION 5 - DO NOT DELETE THIS LINE ****/
/* Generated */ 
/* Generated */ #define  USE_SINGLE_PRECISION_INPUT
/* Generated */ #if !defined( _MANAGED ) && !defined( _JAVA )
/* Generated */    #undef   TA_PREFIX
/* Generated */    #define  TA_PREFIX(x) TA_S_##x
/* Generated */ #endif
/* Generated */ #undef   INPUT_TYPE
/* Generated */ #define  INPUT_TYPE float
/* Generated */ TA_RetCode TA_S_CDLINVERTEDHAMMER( int    startIdx,
/* Generated */                                    int    endIdx,
/* Generated */                                    const float  inOpen[],
/* Generated */                                    const float  inHigh[],
/* Generated */                                    const float  inLow[],
/* Generated */                                    const float  inClose[],
/* Generated */                                    int          *outBegIdx,
/* Generated */                                    int          *outNBElement,
/* Generated */                                    int           outInteger[] )
/* Generated */ {
/* Generated */     double BodyPeriodTotal, ShadowLongPeriodTotal, ShadowVeryShortPeriodTotal;
/* Generated */     int i, outIdx, BodyTrailingIdx, ShadowLongTrailingIdx, ShadowVeryShortTrailingIdx, lookbackTotal;
/* Generated */  #ifndef TA_FUNC_NO_RANGE_CHECK
/* Generated */     if( startIdx < 0 )
/* Generated */        return ENUM_VALUE(RetCode,TA_OUT_OF_RANGE_START_INDEX,OutOfRangeStartIndex);
/* Generated */     if( (endIdx < 0) || (endIdx < startIdx))
/* Generated */        return ENUM_VALUE(RetCode,TA_OUT_OF_RANGE_END_INDEX,OutOfRangeEndIndex);
/* Generated */     #if !defined(_JAVA)
/* Generated */     if(!inOpen||!inHigh||!inLow||!inClose)
/* Generated */        return ENUM_VALUE(RetCode,TA_BAD_PARAM,BadParam);
/* Generated */     #endif 
/* Generated */     #if !defined(_JAVA)
/* Generated */     if( !outInteger )
/* Generated */        return ENUM_VALUE(RetCode,TA_BAD_PARAM,BadParam);
/* Generated */     #endif 
/* Generated */  #endif 
/* Generated */    lookbackTotal = LOOKBACK_CALL(CDLINVERTEDHAMMER)();
/* Generated */    if( startIdx < lookbackTotal )
/* Generated */       startIdx = lookbackTotal;
/* Generated */    if( startIdx > endIdx )
/* Generated */    {
/* Generated */       VALUE_HANDLE_DEREF_TO_ZERO(outBegIdx);
/* Generated */       VALUE_HANDLE_DEREF_TO_ZERO(outNBElement);
/* Generated */       return ENUM_VALUE(RetCode,TA_SUCCESS,Success);
/* Generated */    }
/* Generated */    BodyPeriodTotal = 0;
/* Generated */    BodyTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(BodyShort);
/* Generated */    ShadowLongPeriodTotal = 0;
/* Generated */    ShadowLongTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(ShadowLong);
/* Generated */    ShadowVeryShortPeriodTotal = 0;
/* Generated */    ShadowVeryShortTrailingIdx = startIdx - TA_CANDLEAVGPERIOD(ShadowVeryShort);
/* Generated */    i = BodyTrailingIdx;
/* Generated */    while( i < startIdx ) {
/* Generated */         BodyPeriodTotal += TA_CANDLERANGE( BodyShort, i );
/* Generated */         i++;
/* Generated */    }
/* Generated */    i = ShadowLongTrailingIdx;
/* Generated */    while( i < startIdx ) {
/* Generated */         ShadowLongPeriodTotal += TA_CANDLERANGE( ShadowLong, i );
/* Generated */         i++;
/* Generated */    }
/* Generated */    i = ShadowVeryShortTrailingIdx;
/* Generated */    while( i < startIdx ) {
/* Generated */         ShadowVeryShortPeriodTotal += TA_CANDLERANGE( ShadowVeryShort, i );
/* Generated */         i++;
/* Generated */    }
/* Generated */    outIdx = 0;
/* Generated */    do
/* Generated */    {
/* Generated */         if( TA_REALBODY(i) < TA_CANDLEAVERAGE( BodyShort, BodyPeriodTotal, i ) &&                        // small rb
/* Generated */             TA_UPPERSHADOW(i) > TA_CANDLEAVERAGE( ShadowLong, ShadowLongPeriodTotal, i ) &&              // long upper shadow
/* Generated */             TA_LOWERSHADOW(i) < TA_CANDLEAVERAGE( ShadowVeryShort, ShadowVeryShortPeriodTotal, i ) &&    // very short lower shadow
/* Generated */             TA_REALBODYGAPDOWN(i, i-1) )                                                                    // gap down
/* Generated */             outInteger[outIdx++] = 100;
/* Generated */         else
/* Generated */             outInteger[outIdx++] = 0;
/* Generated */         BodyPeriodTotal += TA_CANDLERANGE( BodyShort, i ) 
/* Generated */             - TA_CANDLERANGE( BodyShort, BodyTrailingIdx );
/* Generated */         ShadowLongPeriodTotal += TA_CANDLERANGE( ShadowLong, i ) 
/* Generated */             - TA_CANDLERANGE( ShadowLong, ShadowLongTrailingIdx );
/* Generated */         ShadowVeryShortPeriodTotal += TA_CANDLERANGE( ShadowVeryShort, i ) 
/* Generated */             - TA_CANDLERANGE( ShadowVeryShort, ShadowVeryShortTrailingIdx );
/* Generated */         i++; 
/* Generated */         BodyTrailingIdx++;
/* Generated */         ShadowLongTrailingIdx++;
/* Generated */         ShadowVeryShortTrailingIdx++;
/* Generated */    } while( i <= endIdx );
/* Generated */    VALUE_HANDLE_DEREF(outNBElement) = outIdx;
/* Generated */    VALUE_HANDLE_DEREF(outBegIdx)    = startIdx;
/* Generated */    return ENUM_VALUE(RetCode,TA_SUCCESS,Success);
/* Generated */ }
/* Generated */ 
/* Generated */ #if defined( _MANAGED )
/* Generated */ }}} // Close namespace TicTacTec.TA.Lib
/* Generated */ #endif
/**** END GENCODE SECTION 5 - DO NOT DELETE THIS LINE ****/

