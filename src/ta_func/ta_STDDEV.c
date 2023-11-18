#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_STDDEV_Lookback( int           optInTimePeriod, /* From 2 to 100000 */
                      double        optInNbDev )  /* From TA_REAL_MIN to TA_REAL_MAX */
{
   /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 5;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return -1;

   if( optInNbDev == TA_REAL_DEFAULT )
      optInNbDev = 1.000000e+0;
   else if( (optInNbDev < -3.000000e+37) || (optInNbDev > 3.000000e+37) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */

   /* Lookback is driven by the variance. */
   return TA_VAR_Lookback( optInTimePeriod, optInNbDev );
}

/*
 * TA_STDDEV - Standard Deviation
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * optInNbDev:(From TA_REAL_MIN to TA_REAL_MAX)
 *    Nb of deviations
 * 
 * 
 */
TA_RetCode TA_STDDEV( int    startIdx,
                      int    endIdx,
                      const double inReal[],
                      int           optInTimePeriod, /* From 2 to 100000 */
                      double        optInNbDev, /* From TA_REAL_MIN to TA_REAL_MAX */
                      int          *outBegIdx,
                      int          *outNBElement,
                      double        outReal[] )
{
   /* Insert local variables here. */
   int i;
   TA_RetCode retCode;
   double tempReal;

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

   if( optInNbDev == TA_REAL_DEFAULT )
      optInNbDev = 1.000000e+0;
   else if( (optInNbDev < -3.000000e+37) || (optInNbDev > 3.000000e+37) )
      return TA_BAD_PARAM;

   if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* Calculate the variance. */
   retCode = TA_INT_VAR( startIdx, endIdx,
                                     inReal, optInTimePeriod,
                                     outBegIdx, outNBElement, outReal );

   if( retCode != TA_SUCCESS )
      return retCode;

   /* Calculate the square root of each variance, this
    * is the standard deviation.
    *
    * Multiply also by the ratio specified.
    */
   if( optInNbDev != 1.0 )
   {
      for( i=0; i < (int)(*outNBElement); i++ )
      {
         tempReal = outReal[i];
         if( !TA_IS_ZERO_OR_NEG(tempReal) )
            outReal[i] = std_sqrt(tempReal) * optInNbDev;
         else
            outReal[i] = (double)0.0;
      }
   }
   else
   {
      for( i=0; i < (int)(*outNBElement); i++ )
      {
         tempReal = outReal[i];
         if( !TA_IS_ZERO_OR_NEG(tempReal) )
            outReal[i] = std_sqrt(tempReal);
         else
            outReal[i] = (double)0.0;
      }
   }

   return TA_SUCCESS;
}

/* The inMovAvg is the moving average of the inReal. 
 *
 * inMovAvgBegIdx is relative to inReal, in other word
 * this is the 'outBegIdx' who was returned when doing the
 * MA on the inReal.
 *
 * inMovAvgNbElement is the number of element who was returned
 * when doing the MA on the inReal.
 *
 * Note: This function is not used by TA_STDDEV, since TA_STDDEV
 *       is optimized considering it uses always a simple moving
 *       average. Still the function is put here because it is 
 *       closely related.
 */
void TA_INT_stddev_using_precalc_ma( const double *inReal,
                                             const double *inMovAvg,
                                             int inMovAvgBegIdx,                                    
                                             int inMovAvgNbElement,
                                             int timePeriod,
                                             double *output )
{
   double tempReal, periodTotal2, meanValue2;
   int outIdx;

   /* Start/end index for sumation. */
   int startSum, endSum;

   startSum = 1+inMovAvgBegIdx-timePeriod;
   endSum = inMovAvgBegIdx;

   periodTotal2 = 0;

   for( outIdx = startSum; outIdx < endSum; outIdx++)
   {
      tempReal = inReal[outIdx];
      tempReal *= tempReal;
      periodTotal2 += tempReal;
   }

   for( outIdx=0; outIdx < inMovAvgNbElement; outIdx++, startSum++, endSum++ )
   {
      tempReal = inReal[endSum];
      tempReal *= tempReal;
      periodTotal2 += tempReal;
      meanValue2 = periodTotal2/timePeriod;

      tempReal = inReal[startSum];
      tempReal *= tempReal;
      periodTotal2 -= tempReal;

      tempReal = inMovAvg[outIdx];
      tempReal *= tempReal;
      meanValue2 -= tempReal;
      
      if( !TA_IS_ZERO_OR_NEG(meanValue2) )
         output[outIdx] = std_sqrt(meanValue2);
      else
         output[outIdx] = (double)0.0;
   }
}
