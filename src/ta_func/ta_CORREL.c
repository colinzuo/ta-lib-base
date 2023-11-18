#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_CORREL_Lookback( int           optInTimePeriod )  /* From 1 to 100000 */
{
   /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 30;
   else if( ((int)optInTimePeriod < 1) || ((int)optInTimePeriod > 100000) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */
   return optInTimePeriod-1;
}

/*
 * TA_CORREL - Pearson's Correlation Coefficient (r)
 * 
 * Input  = double, double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_CORREL( int    startIdx,
                      int    endIdx,
                      const double inReal0[],
                      const double inReal1[],
                      int           optInTimePeriod, /* From 1 to 100000 */
                      int          *outBegIdx,
                      int          *outNBElement,
                      double        outReal[] )
{
	/* insert local variable here */
    double sumXY, sumX, sumY, sumX2, sumY2, x, y, trailingX, trailingY;
    double tempReal;
    int lookbackTotal, today, trailingIdx, outIdx;

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   if( !inReal0 ) return TA_BAD_PARAM;
   if( !inReal1 ) return TA_BAD_PARAM;
      /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 30;
   else if( ((int)optInTimePeriod < 1) || ((int)optInTimePeriod > 100000) )
      return TA_BAD_PARAM;

   if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* Move up the start index if there is not
    * enough initial data.
    */
   lookbackTotal = optInTimePeriod-1;
   if( startIdx < lookbackTotal )
      startIdx = lookbackTotal;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
   {
      *outBegIdx = 0;
      *outNBElement = 0;
      return TA_SUCCESS;
   }

   *outBegIdx  = startIdx;
   trailingIdx = startIdx - lookbackTotal;

   /* Calculate the initial values. */
   sumXY = sumX = sumY = sumX2 = sumY2 = 0.0;
   for( today=trailingIdx; today <= startIdx; today++ )
   {
      x = inReal0[today];
      sumX  += x;
      sumX2 += x*x;

      y = inReal1[today];
      sumXY += x*y;
      sumY  += y;
      sumY2 += y*y;
   }

   /* Write the first output. 
    * Save first the trailing values since the input
    * and output might be the same array,
    */
   trailingX = inReal0[trailingIdx];
   trailingY = inReal1[trailingIdx++];
   tempReal = (sumX2-((sumX*sumX)/optInTimePeriod)) * (sumY2-((sumY*sumY)/optInTimePeriod));
   if( !TA_IS_ZERO_OR_NEG(tempReal) )
      outReal[0] = (sumXY-((sumX*sumY)/optInTimePeriod)) / std_sqrt(tempReal);
   else 
      outReal[0] = 0.0;

   /* Tight loop to do subsequent values. */
   outIdx = 1;
   while( today <= endIdx )
   {
      /* Remove trailing values */
      sumX  -= trailingX;
      sumX2 -= trailingX*trailingX;

      sumXY -= trailingX*trailingY;
      sumY  -= trailingY;
      sumY2 -= trailingY*trailingY;

      /* Add new values */
      x = inReal0[today];
      sumX  += x;
      sumX2 += x*x;

      y = inReal1[today++];
      sumXY += x*y;
      sumY  += y;
      sumY2 += y*y;

      /* Output new coefficient.
       * Save first the trailing values since the input
       * and output might be the same array,
       */
      trailingX = inReal0[trailingIdx];
      trailingY = inReal1[trailingIdx++];
      tempReal = (sumX2-((sumX*sumX)/optInTimePeriod)) * (sumY2-((sumY*sumY)/optInTimePeriod));
      if( !TA_IS_ZERO_OR_NEG(tempReal) )
         outReal[outIdx++] = (sumXY-((sumX*sumY)/optInTimePeriod)) / std_sqrt(tempReal);
      else
         outReal[outIdx++] = 0.0;
   }  

   *outNBElement = outIdx;

   return TA_SUCCESS;
}
