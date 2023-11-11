#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_STOCHF_Lookback( int           optInFastK_Period, /* From 1 to 100000 */
                      int           optInFastD_Period, /* From 1 to 100000 */
                      TA_MAType     optInFastD_MAType ) 
{
   /* insert local variable here */
   int retValue;

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInFastK_Period. */
   if( (int)optInFastK_Period == TA_INTEGER_DEFAULT )
      optInFastK_Period = 5;
   else if( ((int)optInFastK_Period < 1) || ((int)optInFastK_Period > 100000) )
      return -1;

   /* min/max are checked for optInFastD_Period. */
   if( (int)optInFastD_Period == TA_INTEGER_DEFAULT )
      optInFastD_Period = 3;
   else if( ((int)optInFastD_Period < 1) || ((int)optInFastD_Period > 100000) )
      return -1;

   if( (int)optInFastD_MAType == TA_INTEGER_DEFAULT )
      optInFastD_MAType = (TA_MAType)0;
   else if( ((int)optInFastD_MAType < 0) || ((int)optInFastD_MAType > 8) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */
   
   /* Account for the initial data needed for Fast-K. */
   retValue = (optInFastK_Period - 1);
         
   /* Add the smoothing being done for Fast-D */
   retValue += TA_MA_Lookback( optInFastD_Period, optInFastD_MAType );

   return retValue;
}

/*
 * TA_STOCHF - Stochastic Fast
 * 
 * Input  = High, Low, Close
 * Output = double, double
 * 
 * Optional Parameters
 * -------------------
 * optInFastK_Period:(From 1 to 100000)
 *    Time period for building the Fast-K line
 * 
 * optInFastD_Period:(From 1 to 100000)
 *    Smoothing for making the Fast-D line. Usually set to 3
 * 
 * optInFastD_MAType:
 *    Type of Moving Average for Fast-D
 * 
 * 
 */
TA_RetCode TA_STOCHF( int    startIdx,
                      int    endIdx,
                      const double inHigh[],
                      const double inLow[],
                      const double inClose[],
                      int           optInFastK_Period, /* From 1 to 100000 */
                      int           optInFastD_Period, /* From 1 to 100000 */
                      TA_MAType     optInFastD_MAType,
                      int          *outBegIdx,
                      int          *outNBElement,
                      double        outFastK[],
                      double        outFastD[] )
{
	/* insert local variable here */
   TA_RetCode retCode;
   double lowest, highest, tmp, diff;
   ARRAY_REF( tempBuffer );
   int outIdx, lowestIdx, highestIdx;
   int lookbackTotal, lookbackK, lookbackFastD;
   int trailingIdx, today, i;

   #if !defined( _MANAGED ) && !defined(USE_SINGLE_PRECISION_INPUT) && !defined( _JAVA )
   int bufferIsAllocated;
   #endif

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   /* Verify required price component. */
   if(!inHigh||!inLow||!inClose)
      return TA_BAD_PARAM;

      /* min/max are checked for optInFastK_Period. */
   if( (int)optInFastK_Period == TA_INTEGER_DEFAULT )
      optInFastK_Period = 5;
   else if( ((int)optInFastK_Period < 1) || ((int)optInFastK_Period > 100000) )
      return TA_BAD_PARAM;

   /* min/max are checked for optInFastD_Period. */
   if( (int)optInFastD_Period == TA_INTEGER_DEFAULT )
      optInFastD_Period = 3;
   else if( ((int)optInFastD_Period < 1) || ((int)optInFastD_Period > 100000) )
      return TA_BAD_PARAM;

   if( (int)optInFastD_MAType == TA_INTEGER_DEFAULT )
      optInFastD_MAType = (TA_MAType)0;
   else if( ((int)optInFastD_MAType < 0) || ((int)optInFastD_MAType > 8) )
      return TA_BAD_PARAM;

   if( !outFastK )
      return TA_BAD_PARAM;

   if( !outFastD )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* With stochastic, there is a total of 4 different lines that
    * are defined: FASTK, FASTD, SLOWK and SLOWD.
    *
    * The D is the signal line usually drawn over its
    * corresponding K function.
    *
    *                    (Today's Close - LowestLow)
    *  FASTK(Kperiod) =  --------------------------- * 100
    *                     (HighestHigh - LowestLow)
    *   
    *  FASTD(FastDperiod, MA type) = MA Smoothed FASTK over FastDperiod
    * 
    *  SLOWK(SlowKperiod, MA type) = MA Smoothed FASTK over SlowKperiod
    *
    *  SLOWD(SlowDperiod, MA Type) = MA Smoothed SLOWK over SlowDperiod
    *
    * The HighestHigh and LowestLow are the extreme values among the
    * last 'Kperiod'.
    *  
    * SLOWK and FASTD are equivalent when using the same period.
    *
    * The following shows how these four lines are made available in TA-LIB:
    *
    *  TA_STOCH  : Returns the SLOWK and SLOWD
    *  TA_STOCHF : Returns the FASTK and FASTD
    *
    * The TA_STOCH function correspond to the more widely implemented version
    * found in many software/charting package. The TA_STOCHF is more rarely
    * used because its higher volatility cause often whipsaws.
    */

   /* Identify the lookback needed. */
   lookbackK      = optInFastK_Period-1;
   lookbackFastD  = TA_MA_Lookback( optInFastD_Period, optInFastD_MAType );
   lookbackTotal  = lookbackK + lookbackFastD;

   /* Move up the start index if there is not
    * enough initial data.
    */
   if( startIdx < lookbackTotal )
      startIdx = lookbackTotal;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
   {
      /* Succeed... but no data in the output. */
      *outBegIdx = 0;
      *outNBElement = 0;
      return TA_SUCCESS;
   }

   /* Do the K calculation:
    *
    *    Kt = 100 x ((Ct-Lt)/(Ht-Lt))
    *
    * Kt is today stochastic
    * Ct is today closing price.
    * Lt is the lowest price of the last K Period (including today)
    * Ht is the highest price of the last K Period (including today)
    */

   /* Proceed with the calculation for the requested range.
    * Note that this algorithm allows the input and
    * output to be the same buffer.
    */
   outIdx = 0;

   /* Calculate just enough K for ending up with the caller 
    * requested range. (The range of k must consider all
    * the lookback involve with the smoothing).
    */
   trailingIdx = startIdx-lookbackTotal;
   today       = trailingIdx+lookbackK;
   lowestIdx   = highestIdx = -1;
   diff = highest = lowest  = 0.0;

   /* Allocate a temporary buffer large enough to
    * store the K.
    *
    * If the output is the same as the input, great
    * we just save ourself one memory allocation.
    */
   #if !defined( _MANAGED ) && !defined(USE_SINGLE_PRECISION_INPUT) && !defined( _JAVA )
      bufferIsAllocated = 0;
   #endif

   #if defined(USE_SINGLE_PRECISION_INPUT) || defined( USE_SUBARRAY )
      /* Always alloc, since output is of different type and
       * its allocated size is not guarantee to be as large as
       * the input.
       */
      ARRAY_ALLOC( tempBuffer, endIdx-today+1 );
   #else
      if( (outFastK == inHigh) || 
          (outFastK == inLow)  || 
          (outFastK == inClose) )
      {
         tempBuffer = outFastK;
      }
      else if( (outFastD == inHigh) ||
               (outFastD == inLow)  ||
               (outFastD == inClose) )
      {
         tempBuffer = outFastD;
      }
      else
      {
         #if !defined( _MANAGED ) && !defined( _JAVA )
            bufferIsAllocated = 1;
         #endif
         ARRAY_ALLOC(tempBuffer, endIdx-today+1 );
      }
   #endif

   /* Do the K calculation */
   while( today <= endIdx )
   {
      /* Set the lowest low */
      tmp = inLow[today];
      if( lowestIdx < trailingIdx )
      {
         lowestIdx = trailingIdx;
         lowest = inLow[lowestIdx];
         i = lowestIdx;
         while( ++i<=today )
         {
            tmp = inLow[i];
            if( tmp < lowest )
            {
               lowestIdx = i;
               lowest = tmp;
            }
         }
         diff = (highest - lowest)/100.0;
      }
      else if( tmp <= lowest )
      {
         lowestIdx = today;
         lowest = tmp;
         diff = (highest - lowest)/100.0;
      }

      /* Set the highest high */
      tmp = inHigh[today];
      if( highestIdx < trailingIdx )
      {
         highestIdx = trailingIdx;
         highest = inHigh[highestIdx];
         i = highestIdx;
         while( ++i<=today )
         {
            tmp = inHigh[i];
            if( tmp > highest )
            {
               highestIdx = i;
               highest = tmp;
            }
         }
         diff = (highest - lowest)/100.0;
      }
      else if( tmp >= highest )
      {
         highestIdx = today;
         highest = tmp;
         diff = (highest - lowest)/100.0;
      }

      /* Calculate stochastic. */
      if( diff != 0.0 )
        tempBuffer[outIdx++] = (inClose[today]-lowest)/diff;
      else
        tempBuffer[outIdx++] = 0.0;

      trailingIdx++;
      today++; 
   }

   /* Fast-K calculation completed. This K calculation is returned
    * to the caller. It is smoothed to become Fast-D.
    */
   retCode = TA_MA( 0, outIdx-1,
                                       tempBuffer, optInFastD_Period,
                                       optInFastD_MAType, 
                                       outBegIdx, outNBElement, outFastD );


   if( (retCode != TA_SUCCESS ) || ((int)(*outNBElement)) == 0 )
   {
      #if defined(USE_SINGLE_PRECISION_INPUT)
         ARRAY_FREE( tempBuffer ); 
      #else
         ARRAY_FREE_COND( bufferIsAllocated, tempBuffer ); 
      #endif
      /* Something wrong happen? No further data? */
      *outBegIdx = 0;
      *outNBElement = 0;
      return retCode; 
   }

   /* Copy tempBuffer into the caller buffer. 
    * (Calculation could not be done directly in the
    *  caller buffer because more input data then the
    *  requested range was needed for doing %D).
    */
   ARRAY_MEMMOVE( outFastK, 0, tempBuffer, lookbackFastD, (int)(*outNBElement) );

   /* Don't need K anymore, free it if it was allocated here. */
   #if defined(USE_SINGLE_PRECISION_INPUT)
      ARRAY_FREE( tempBuffer ); 
   #else
      ARRAY_FREE_COND( bufferIsAllocated, tempBuffer ); 
   #endif

   if( retCode != TA_SUCCESS )
   {
      /* Something wrong happen while processing %D? */
      *outBegIdx = 0;
      *outNBElement = 0;
      return retCode;
   }

   /* Note: Keep the outBegIdx relative to the
    *       caller input before returning.
    */
   *outBegIdx = startIdx;

   return TA_SUCCESS;
}
