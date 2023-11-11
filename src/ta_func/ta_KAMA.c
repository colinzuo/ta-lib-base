#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_KAMA_Lookback( int           optInTimePeriod )  /* From 2 to 100000 */
{

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 30;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   return optInTimePeriod + TA_GLOBALS_UNSTABLE_PERIOD(TA_FUNC_UNST_KAMA,Kama);
}

/*
 * TA_KAMA - Kaufman Adaptive Moving Average
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_KAMA( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int           optInTimePeriod, /* From 2 to 100000 */
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] )
{
	/* insert local variable here */

   const double constMax = 2.0/(30.0+1.0);
   const double constDiff = 2.0/(2.0+1.0) - constMax;

   double tempReal, tempReal2;
   double sumROC1, periodROC, prevKAMA;
   int i, today, outIdx, lookbackTotal;
   int trailingIdx;
   double trailingValue;

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   if( !inReal ) return TA_BAD_PARAM;
      /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 30;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return TA_BAD_PARAM;

   if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* Default return values */
   *outBegIdx = 0;
   *outNBElement = 0;

   /* Identify the minimum number of price bar needed
    * to calculate at least one output.
    */
   lookbackTotal = optInTimePeriod + TA_GLOBALS_UNSTABLE_PERIOD(TA_FUNC_UNST_KAMA,Kama);

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
      
   /* Initialize the variables by going through
    * the lookback period.
    */
   sumROC1 = 0.0;
   today = startIdx-lookbackTotal;   
   trailingIdx = today;
   i = optInTimePeriod;
   while( i-- > 0 )
   {      
      tempReal  = inReal[today++];
      tempReal -= inReal[today];
      sumROC1  += std_fabs(tempReal);
   }

   /* At this point sumROC1 represent the 
    * summation of the 1-day price difference
    * over the (optInTimePeriod-1)
    */

   /* Calculate the first KAMA */

   /* The yesterday price is used here as the previous KAMA. */
   prevKAMA = inReal[today-1];

   tempReal  = inReal[today];
   tempReal2 = inReal[trailingIdx++];
   periodROC = tempReal-tempReal2;

   /* Save the trailing value. Do this because inReal
    * and outReal can be pointers to the same buffer. 
    */
   trailingValue = tempReal2;

   /* Calculate the efficiency ratio */
   if( (sumROC1 <= periodROC) || TA_IS_ZERO(sumROC1))
      tempReal = 1.0;
   else
      tempReal = std_fabs(periodROC/sumROC1);

   /* Calculate the smoothing constant */
   tempReal  = (tempReal*constDiff)+constMax;
   tempReal *= tempReal;

   /* Calculate the KAMA like an EMA, using the
    * smoothing constant as the adaptive factor.
    */
   prevKAMA = ((inReal[today++]-prevKAMA)*tempReal) + prevKAMA;

   /* 'today' keep track of where the processing is within the
    * input.
    */

   /* Skip the unstable period. Do the whole processing 
    * needed for KAMA, but do not write it in the output.
    */   
   while( today <= startIdx )
   {
      tempReal  = inReal[today];
      tempReal2 = inReal[trailingIdx++];
      periodROC = tempReal-tempReal2;

      /* Adjust sumROC1:
       *  - Remove trailing ROC1 
       *  - Add new ROC1
       */
      sumROC1 -= std_fabs(trailingValue-tempReal2);
      sumROC1 += std_fabs(tempReal-inReal[today-1]);

      /* Save the trailing value. Do this because inReal
       * and outReal can be pointers to the same buffer. 
       */
      trailingValue = tempReal2;

      /* Calculate the efficiency ratio */
      if( (sumROC1 <= periodROC) || TA_IS_ZERO(sumROC1) )
         tempReal = 1.0;
      else
         tempReal = std_fabs(periodROC/sumROC1);

      /* Calculate the smoothing constant */
      tempReal  = (tempReal*constDiff)+constMax;
      tempReal *= tempReal;

      /* Calculate the KAMA like an EMA, using the
       * smoothing constant as the adaptive factor.
       */
      prevKAMA = ((inReal[today++]-prevKAMA)*tempReal) + prevKAMA;
   }

   /* Write the first value. */
   outReal[0] = prevKAMA;
   outIdx = 1;
   *outBegIdx = today-1;

   /* Do the KAMA calculation for the requested range. */
   while( today <= endIdx )
   {
      tempReal  = inReal[today];
      tempReal2 = inReal[trailingIdx++];
      periodROC = tempReal-tempReal2;

      /* Adjust sumROC1:
       *  - Remove trailing ROC1 
       *  - Add new ROC1
       */
      sumROC1 -= std_fabs(trailingValue-tempReal2);
      sumROC1 += std_fabs(tempReal-inReal[today-1]);

      /* Save the trailing value. Do this because inReal
       * and outReal can be pointers to the same buffer. 
       */
      trailingValue = tempReal2;

      /* Calculate the efficiency ratio */
      if( (sumROC1 <= periodROC) || TA_IS_ZERO(sumROC1) )
         tempReal = 1.0;
      else
         tempReal = std_fabs(periodROC / sumROC1);

      /* Calculate the smoothing constant */
      tempReal  = (tempReal*constDiff)+constMax;
      tempReal *= tempReal;

      /* Calculate the KAMA like an EMA, using the
       * smoothing constant as the adaptive factor.
       */
      prevKAMA = ((inReal[today++]-prevKAMA)*tempReal) + prevKAMA;
      outReal[outIdx++] = prevKAMA;
   }

   *outNBElement = outIdx;

   return TA_SUCCESS;
}
