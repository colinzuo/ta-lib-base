/* Provides common mathematical or analysis functions.
 *
 * These functions are all PRIVATE to ta-lib and should
 * never be called directly by the user of the TA-LIB.
 */

#pragma once

#include "ta_common.h"
#include "ta_memory.h"
#include "ta_global.h"

/* Calculate a Simple Moving Average.
 * This is an internal version, parameter are assumed validated.
 * (startIdx and endIdx cannot be -1).
 */
TA_RetCode TA_INT_SMA( int           startIdx,
                       int           endIdx,
                       const double *inReal,
                       int           optInTimePeriod, 
                       int          *outBegIdx,
                       int          *outNBElement,
                       double       *outReal );

/* Calculate an Exponential Moving Average.
 * This is an internal version, parameter are assumed validated.
 * (startIdx and endIdx cannot be -1).
 */
TA_RetCode TA_INT_EMA( int           startIdx,
                       int           endIdx,
                       const double *inReal,
                       int           optInTimePeriod, 
                       double        optInK_1,
                       int          *outBegIdx,
                       int          *outNBElement,
                       double       *outReal );

/* Calculate a MACD
 * This is an internal version, parameter are assumed validated.
 * (startIdx and endIdx cannot be -1).
 */
TA_RetCode TA_INT_MACD( int           startIdx,
                        int           endIdx,
                        const double  inReal[],
                        int           optInFastPeriod, /* 0 is fix 12 */
                        int           optInSlowPeriod, /* 0 is fix 26 */
                        int           optInSignalPeriod_2, 
                        int          *outBegIdx,
                        int          *outNBElement,
                        double        outRealMACD_0[],
                        double        outRealMACDSignal_1[],
                        double        outRealMACDHist_2[] );

/* Internal Price Oscillator function.
 *
 * A buffer must be provided for intermediate processing
 * 'tempBuffer' must be of at least (endIdx-startIdx+1)
 */
TA_RetCode TA_INT_PO( int           startIdx,
                      int           endIdx,
                      const double *inReal,
                      int           optInFastPeriod, 
                      int           optInSlowPeriod, 
                      TA_MAType     optInMethod_2,
                      int          *outBegIdx,
                      int          *outNBElement,
                      double       *outReal,
                      double       *tempBuffer,
                      int  doPercentageOutput );

/* Internal variance function. */
TA_RetCode TA_INT_VAR( int           startIdx,
                       int           endIdx,
                       const double *inReal,
                       int           optInTimePeriod,                       
                       int          *outBegIdx,
                       int          *outNBElement,
                       double       *outReal );

/* A function to calculate a standard deviation.
 *
 * This function allows speed optimization when the
 * moving average series is already calculated.
 */
void TA_INT_stddev_using_precalc_ma( const double *inReal,
                                     const double *inMovAvg,
                                     int           inMovAvgBegIdx,
                                     int           inMovAvgNbElement,
                                     int           timePeriod,
                                     double       *output );

/* Provides an equivalent to standard "math.h" functions. */
#define std_floor floor
#define std_ceil  ceil
#define std_fabs  fabs
#define std_atan  atan
#define std_cos   cos
#define std_sin   sin
#define std_sqrt  sqrt
#define std_tanh  tanh
#define std_tan   tan
#define std_sinh  sinh
#define std_log10 log10
#define std_log   log
#define std_exp   exp
#define std_cosh  cosh   
#define std_asin  asin
#define std_acos  acos

/* Rounding macro for doubles. Works only with positive numbers. */
#define round_pos(x) (std_floor((x)+0.5))

/* Rounding macro for doubles. Works only with negative numbers. */
#define round_neg(x) (std_ceil((x)-0.5))

/* Rounding with a precision of 2 digit after the dot */
#define round_pos_2(x) ((std_floor((x*100.0)+0.5))/100.0)
#define round_neg_2(x) ((std_ceil((x*100.0)-0.5))/100.0)

/* In the context of TA-Lib, floating point are often 
 * compared within an acceptable error range.
 *
 * As an example,a TA oscillator ranging from 0 to 100 can
 * fairly be considered equal if their difference is less 
 * than 0.000001.
 *
 * Ranging around zero also allows to work around limit 
 * cases where floating point minimal step (EPSILON) causes 
 * unexpected cummulative effect (ending with "negative zero" 
 * being one example).
 *
 * FLT_EPSILON == 1.192092896e-07 for float type on intel with msvc. 
 * DBL_EPSILON == 2.2204460492503131e-016 for the double type.
 *
 * Warning: These macro are not intended as "general purpose" floating
 * point comparison. TA_REAL_EQ is not even transitive. The "ep" parameter
 * must be carefully choosen to work in the domain of the tested values.  
 * Do a search on Google for a more generalize algo.
 */
#define TA_REAL_EQ(x,v,ep)   (((v-ep)<x)&&(x<(v+ep)))
#define TA_IS_ZERO(v)        (((-0.00000001)<v)&&(v<0.00000001))
#define TA_IS_ZERO_OR_NEG(v) (v<0.00000001)

/* The following macros are being used to do
 * the Hilbert Transform logic as documented
 * in John Ehlers books "Rocket Science For Traders".
 */
#define HILBERT_VARIABLES(varName) \
   ARRAY_LOCAL(varName##_Odd,3); \
   ARRAY_LOCAL(varName##_Even, 3); \
   double varName; \
   double prev_##varName##_Odd; \
   double prev_##varName##_Even; \
   double prev_##varName##_input_Odd; \
   double prev_##varName##_input_Even

#define INIT_HILBERT_VARIABLES(varName) { \
      varName##_Odd [0] = 0.0; \
      varName##_Odd [1] = 0.0; \
      varName##_Odd [2] = 0.0; \
      varName##_Even[0] = 0.0; \
      varName##_Even[1] = 0.0; \
      varName##_Even[2] = 0.0; \
      varName = 0.0; \
      prev_##varName##_Odd        = 0.0; \
      prev_##varName##_Even       = 0.0; \
      prev_##varName##_input_Odd  = 0.0; \
      prev_##varName##_input_Even = 0.0; \
      }

#define DO_HILBERT_TRANSFORM(varName,input,OddOrEvenId) {\
         hilbertTempReal = a * input; \
         varName = -varName##_##OddOrEvenId[hilbertIdx]; \
         varName##_##OddOrEvenId[hilbertIdx] = hilbertTempReal; \
         varName += hilbertTempReal; \
         varName -= prev_##varName##_##OddOrEvenId; \
         prev_##varName##_##OddOrEvenId = b * prev_##varName##_input_##OddOrEvenId; \
         varName += prev_##varName##_##OddOrEvenId; \
         prev_##varName##_input_##OddOrEvenId = input; \
         varName *= adjustedPrevPeriod; \
         }

#define DO_HILBERT_ODD(varName,input)  DO_HILBERT_TRANSFORM(varName,input,Odd)
#define DO_HILBERT_EVEN(varName,input) DO_HILBERT_TRANSFORM(varName,input,Even)

/* Convert a period into the equivalent k:
 *
 *    k = 2 / (period + 1)
 *
 * Useful to calculate the 'k' for TA_INT_EMA().
 */
#define PER_TO_K( per ) ((double)2.0 / ((double)(per + 1)))

/* Math Constants and Functions */
#define PI 3.14159265358979323846

#ifndef min
   #define min(a, b)  (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
   #define max(a, b)  (((a) > (b)) ? (a) : (b))
#endif 

/* Candlestick macros (used by candlestick functions, where the parameters are always called inClose, inOpen, etc.
 * Don't use i++ or func(i) with these macros !
 */

#define TA_REALBODY(IDX)        ( std_fabs( inClose[IDX] - inOpen[IDX] ) )
#define TA_UPPERSHADOW(IDX)     ( inHigh[IDX] - ( inClose[IDX] >= inOpen[IDX] ? inClose[IDX] : inOpen[IDX] ) )
#define TA_LOWERSHADOW(IDX)     ( ( inClose[IDX] >= inOpen[IDX] ? inOpen[IDX] : inClose[IDX] ) - inLow[IDX] )
#define TA_HIGHLOWRANGE(IDX)    ( inHigh[IDX] - inLow[IDX] )
#define TA_CANDLECOLOR(IDX)     ( inClose[IDX] >= inOpen[IDX] ? 1 : -1 )

#define TA_CANDLERANGETYPE(SET) (TA_Globals->candleSettings[TA_##SET].rangeType)
#define TA_CANDLEAVGPERIOD(SET) (TA_Globals->candleSettings[TA_##SET].avgPeriod)
#define TA_CANDLEFACTOR(SET)    (TA_Globals->candleSettings[TA_##SET].factor)

#define TA_CANDLERANGE(SET,IDX) \
    ( TA_CANDLERANGETYPE(SET) == TA_RangeType_RealBody ? TA_REALBODY(IDX) : \
    ( TA_CANDLERANGETYPE(SET) == TA_RangeType_HighLow  ? TA_HIGHLOWRANGE(IDX) : \
    ( TA_CANDLERANGETYPE(SET) == TA_RangeType_Shadows  ? TA_UPPERSHADOW(IDX) + TA_LOWERSHADOW(IDX) : \
      0 ) ) )
#define TA_CANDLEAVERAGE(SET,SUM,IDX) \
    ( TA_CANDLEFACTOR(SET) \
        * ( TA_CANDLEAVGPERIOD(SET) != 0.0? SUM / TA_CANDLEAVGPERIOD(SET) : TA_CANDLERANGE(SET,IDX) ) \
        / ( TA_CANDLERANGETYPE(SET) == TA_RangeType_Shadows ? 2.0 : 1.0 ) \
    )
#define TA_REALBODYGAPUP(IDX2,IDX1)     ( min(inOpen[IDX2],inClose[IDX2]) > max(inOpen[IDX1],inClose[IDX1]) )
#define TA_REALBODYGAPDOWN(IDX2,IDX1)   ( max(inOpen[IDX2],inClose[IDX2]) < min(inOpen[IDX1],inClose[IDX1]) )
#define TA_CANDLEGAPUP(IDX2,IDX1)       ( inLow[IDX2] > inHigh[IDX1] )
#define TA_CANDLEGAPDOWN(IDX2,IDX1)     ( inHigh[IDX2] < inLow[IDX1] )
