#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_BBANDS_Lookback( int           optInTimePeriod, /* From 2 to 100000 */
                      double        optInNbDevUp, /* From TA_REAL_MIN to TA_REAL_MAX */
                      double        optInNbDevDn, /* From TA_REAL_MIN to TA_REAL_MAX */
                      TA_MAType     optInMAType ) 
{
   /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 5;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return -1;

   if( optInNbDevUp == TA_REAL_DEFAULT )
      optInNbDevUp = 2.000000e+0;
   else if( (optInNbDevUp < -3.000000e+37) || (optInNbDevUp > 3.000000e+37) )
      return -1;

   if( optInNbDevDn == TA_REAL_DEFAULT )
      optInNbDevDn = 2.000000e+0;
   else if( (optInNbDevDn < -3.000000e+37) || (optInNbDevDn > 3.000000e+37) )
      return -1;

   if( (int)optInMAType == TA_INTEGER_DEFAULT )
      optInMAType = (TA_MAType)0;
   else if( ((int)optInMAType < 0) || ((int)optInMAType > 8) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */
   (void)optInNbDevUp;
   (void)optInNbDevDn;

   /* The lookback is driven by the middle band moving average. */
   return TA_MA_Lookback( optInTimePeriod, optInMAType );                          
}

/*
 * TA_BBANDS - Bollinger Bands
 * 
 * Input  = double
 * Output = double, double, double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * optInNbDevUp:(From TA_REAL_MIN to TA_REAL_MAX)
 *    Deviation multiplier for upper band
 * 
 * optInNbDevDn:(From TA_REAL_MIN to TA_REAL_MAX)
 *    Deviation multiplier for lower band
 * 
 * optInMAType:
 *    Type of Moving Average
 * 
 * 
 */
TA_RetCode TA_BBANDS( int    startIdx,
                      int    endIdx,
                      const double inReal[],
                      int           optInTimePeriod, /* From 2 to 100000 */
                      double        optInNbDevUp, /* From TA_REAL_MIN to TA_REAL_MAX */
                      double        optInNbDevDn, /* From TA_REAL_MIN to TA_REAL_MAX */
                      TA_MAType     optInMAType,
                      int          *outBegIdx,
                      int          *outNBElement,
                      double        outRealUpperBand[],
                      double        outRealMiddleBand[],
                      double        outRealLowerBand[] )
{
   /* Insert local variables here. */
   TA_RetCode retCode;
   int i;
   double tempReal, tempReal2;
   ARRAY_REF(tempBuffer1);
   ARRAY_REF(tempBuffer2);

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   if( !inReal ) return TA_BAD_PARAM;
      /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 5;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return TA_BAD_PARAM;

   if( optInNbDevUp == TA_REAL_DEFAULT )
      optInNbDevUp = 2.000000e+0;
   else if( (optInNbDevUp < -3.000000e+37) || (optInNbDevUp > 3.000000e+37) )
      return TA_BAD_PARAM;

   if( optInNbDevDn == TA_REAL_DEFAULT )
      optInNbDevDn = 2.000000e+0;
   else if( (optInNbDevDn < -3.000000e+37) || (optInNbDevDn > 3.000000e+37) )
      return TA_BAD_PARAM;

   if( (int)optInMAType == TA_INTEGER_DEFAULT )
      optInMAType = (TA_MAType)0;
   else if( ((int)optInMAType < 0) || ((int)optInMAType > 8) )
      return TA_BAD_PARAM;

   if( !outRealUpperBand )
      return TA_BAD_PARAM;

   if( !outRealMiddleBand )
      return TA_BAD_PARAM;

   if( !outRealLowerBand )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* Identify TWO temporary buffer among the outputs.
    *
    * These temporary buffers allows to perform the
    * calculation without any memory allocation.
    *
    * Whenever possible, make the tempBuffer1 be the
    * middle band output. This will save one copy operation.
    */
   #if defined(USE_SINGLE_PRECISION_INPUT) || defined( USE_SUBARRAY )
      tempBuffer1 = outRealMiddleBand;
      tempBuffer2 = outRealLowerBand;
   #else
      if( inReal == outRealUpperBand )
      {
         tempBuffer1 = outRealMiddleBand;
         tempBuffer2 = outRealLowerBand;
      }
      else if( inReal == outRealLowerBand )
      {
         tempBuffer1 = outRealMiddleBand;
         tempBuffer2 = outRealUpperBand;
      }
      else if( inReal == outRealMiddleBand )
      {
         tempBuffer1 = outRealLowerBand;
         tempBuffer2 = outRealUpperBand;
      }
      else
      {
         tempBuffer1 = outRealMiddleBand;
         tempBuffer2 = outRealUpperBand;
      }
      /* Check that the caller is not doing tricky things. 
       * (like using the input buffer in two output!)
       */
      if( (tempBuffer1 == inReal) || (tempBuffer2 == inReal) )
         return TA_BAD_PARAM;
   #endif


   /* Calculate the middle band, which is a moving average.
    * The other two bands will simply add/substract the
    * standard deviation from this middle band.
    */
   retCode = TA_PREFIX(MA)( startIdx, endIdx, inReal,
                                optInTimePeriod, optInMAType,
                                outBegIdx, outNBElement, tempBuffer1 );

   if( (retCode != TA_SUCCESS ) || ((int)(*outNBElement) == 0) )
   {
      *outNBElement = 0;
      return retCode;
   }

   /* Calculate the standard deviation into tempBuffer2. */
   if( optInMAType == TA_MAType_SMA )
   {
      /* A small speed optimization by re-using the
       * already calculated SMA.
       */
       TA_PREFIX(INT_stddev_using_precalc_ma)( inReal, tempBuffer1, 
                                                   (int)(*outBegIdx), (int)(*outNBElement),
                                                   optInTimePeriod, tempBuffer2 );
   }
   else
   {
      /* Calculate the Standard Deviation */
      retCode = TA_PREFIX(STDDEV)( (int)(*outBegIdx), endIdx, inReal,
                                       optInTimePeriod, 1.0,
                                       outBegIdx, outNBElement, tempBuffer2 );

      if( retCode != TA_SUCCESS )
      {
         *outNBElement = 0;
         return retCode;
      }
   }

   /* Copy the MA calculation into the middle band ouput, unless
    * the calculation was done into it already!
    */
   #if !defined(USE_SINGLE_PRECISION_INPUT)
      if( tempBuffer1 != outRealMiddleBand )
      {
         ARRAY_COPY( outRealMiddleBand, tempBuffer1, (*outNBElement) );
      }
   #endif

   /* Now do a tight loop to calculate the upper/lower band at
    * the same time. 
    *
    * All the following 5 loops are doing the same, except there
    * is an attempt to speed optimize by eliminating uneeded
    * multiplication.
    */
   if( optInNbDevUp == optInNbDevDn )
   {
      if(  optInNbDevUp == 1.0 )
      {
         /* No standard deviation multiplier needed. */
         for( i=0; i < (int)(*outNBElement); i++ )
         {
            tempReal  = tempBuffer2[i];
            tempReal2 = outRealMiddleBand[i];
            outRealUpperBand[i] = tempReal2 + tempReal;
            outRealLowerBand[i] = tempReal2 - tempReal;
         }
      }
      else
      {
         /* Upper/lower band use the same standard deviation multiplier. */
         for( i=0; i < (int)(*outNBElement); i++ )
         {
            tempReal  = tempBuffer2[i] * optInNbDevUp;
            tempReal2 = outRealMiddleBand[i];
            outRealUpperBand[i] = tempReal2 + tempReal;
            outRealLowerBand[i] = tempReal2 - tempReal;
         }
      }
   }
   else if( optInNbDevUp == 1.0 )
   {
      /* Only lower band has a standard deviation multiplier. */
      for( i=0; i < (int)(*outNBElement); i++ )
      {
         tempReal  = tempBuffer2[i];
         tempReal2 = outRealMiddleBand[i];
         outRealUpperBand[i] = tempReal2 + tempReal;
         outRealLowerBand[i] = tempReal2 - (tempReal * optInNbDevDn);
      }
   }
   else if( optInNbDevDn == 1.0 )
   {
      /* Only upper band has a standard deviation multiplier. */
      for( i=0; i < (int)(*outNBElement); i++ )
      {
         tempReal  = tempBuffer2[i];
         tempReal2 = outRealMiddleBand[i];
         outRealLowerBand[i] = tempReal2 - tempReal;
         outRealUpperBand[i] = tempReal2 + (tempReal * optInNbDevUp);
      }
   }
   else
   {
      /* Upper/lower band have distinctive standard deviation multiplier. */
      for( i=0; i < (int)(*outNBElement); i++ )
      {
         tempReal  = tempBuffer2[i];
         tempReal2 = outRealMiddleBand[i];
         outRealUpperBand[i] = tempReal2 + (tempReal * optInNbDevUp);
         outRealLowerBand[i] = tempReal2 - (tempReal * optInNbDevDn);
      }
   }
   
   return TA_SUCCESS;
}
