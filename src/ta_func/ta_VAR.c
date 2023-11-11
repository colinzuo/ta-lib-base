#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_VAR_Lookback( int           optInTimePeriod, /* From 1 to 100000 */
                   double        optInNbDev )  /* From TA_REAL_MIN to TA_REAL_MAX */
{
   /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 5;
   else if( ((int)optInTimePeriod < 1) || ((int)optInTimePeriod > 100000) )
      return -1;

   if( optInNbDev == TA_REAL_DEFAULT )
      optInNbDev = 1.000000e+0;
   else if( (optInNbDev < -3.000000e+37) || (optInNbDev > 3.000000e+37) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */
   (void)optInNbDev;

   return optInTimePeriod-1;
}

/*
 * TA_VAR - Variance
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * optInNbDev:(From TA_REAL_MIN to TA_REAL_MAX)
 *    Nb of deviations
 * 
 * 
 */
TA_RetCode TA_VAR( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInTimePeriod, /* From 1 to 100000 */
                   double        optInNbDev, /* From TA_REAL_MIN to TA_REAL_MAX */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] )
{
   /* Insert local variables here. */

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
   else if( ((int)optInTimePeriod < 1) || ((int)optInTimePeriod > 100000) )
      return TA_BAD_PARAM;

   if( optInNbDev == TA_REAL_DEFAULT )
      optInNbDev = 1.000000e+0;
   else if( (optInNbDev < -3.000000e+37) || (optInNbDev > 3.000000e+37) )
      return TA_BAD_PARAM;

   if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */
   return TA_PREFIX(INT_VAR)( startIdx, endIdx, inReal,
                                  optInTimePeriod, /* From 1 to TA_INTEGER_MAX */                      
                                  outBegIdx, outNBElement, outReal );
}

TA_RetCode TA_PREFIX(INT_VAR)( int    startIdx,
                               int    endIdx,
                               const INPUT_TYPE *inReal,
                               int    optInTimePeriod, /* From 1 to TA_INTEGER_MAX */                       
                               int   *outBegIdx,
                               int   *outNBElement,
                               double      *outReal )
{
   double tempReal, periodTotal1, periodTotal2, meanValue1, meanValue2;
   int i, outIdx, trailingIdx, nbInitialElementNeeded;

   /* Validate the calculation method type and
    * identify the minimum number of price bar needed
    * to calculate at least one output.
    */
   nbInitialElementNeeded = (optInTimePeriod-1);

   /* Move up the start index if there is not
    * enough initial data.
    */
   if( startIdx < nbInitialElementNeeded )
      startIdx = nbInitialElementNeeded;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
   {
      *outBegIdx = 0;
      *outNBElement = 0;
      return TA_SUCCESS;
   }

   /* Do the MA calculation using tight loops. */
   /* Add-up the initial periods, except for the last value. */
   periodTotal1 = 0;
   periodTotal2 = 0;
   trailingIdx = startIdx-nbInitialElementNeeded;

   i=trailingIdx;
   if( optInTimePeriod > 1 )
   {
      while( i < startIdx ) {
         tempReal = inReal[i++];
         periodTotal1 += tempReal;
         tempReal *= tempReal;
         periodTotal2 += tempReal;
      }
   }

   /* Proceed with the calculation for the requested range.
    * Note that this algorithm allows the inReal and
    * outReal to be the same buffer.
    */
   outIdx = 0;
   do
   {
      tempReal = inReal[i++];

      /* Square and add all the deviation over
       * the same periods.
       */

      periodTotal1 += tempReal;
      tempReal *= tempReal;
      periodTotal2 += tempReal;

      /* Square and add all the deviation over
       * the same period.
       */

      meanValue1 = periodTotal1 / optInTimePeriod;
      meanValue2 = periodTotal2 / optInTimePeriod;

      tempReal = inReal[trailingIdx++];
      periodTotal1 -= tempReal;
      tempReal *= tempReal;
      periodTotal2 -= tempReal;

      outReal[outIdx++] = meanValue2-meanValue1*meanValue1;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx = startIdx;

   return TA_SUCCESS;
}
