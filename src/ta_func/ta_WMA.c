#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_WMA_Lookback( int           optInTimePeriod )  /* From 2 to 100000 */
{
   /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 30;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */
   return optInTimePeriod - 1;
}

/*
 * TA_WMA - Weighted Moving Average
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
TA_RetCode TA_WMA( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInTimePeriod, /* From 2 to 100000 */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] )
{
   /* Insert local variables here. */
   int inIdx, outIdx, i, trailingIdx, divider;
   double periodSum, periodSub, tempReal, trailingValue;
   int lookbackTotal;

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
   lookbackTotal = optInTimePeriod-1;

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

   /* To make the rest more efficient, handle exception
    * case where the user is asking for a period of '1'.
    * In that case outputs equals inputs for the requested
    * range.
    */
   if( optInTimePeriod == 1 ) 
   {      
      *outBegIdx    = startIdx;
      *outNBElement = endIdx-startIdx+1;

      #if defined( USE_SUBARRAY ) && !defined( USE_SINGLE_PRECISION_INPUT )
        ARRAY_MEMMOVE( outReal, 0, (inReal->mDataArray), (inReal->mOffset)+startIdx, (int)(*outNBElement) );
      #else
        ARRAY_MEMMOVE( outReal, 0, inReal, startIdx, (int)(*outNBElement) );	  	  
      #endif

      return TA_SUCCESS;
   }

   /* Calculate the divider (always an integer value).
    * By induction: 1+2+3+4+'n' = n(n+1)/2
    * '>>1' is usually faster than '/2' for unsigned.
    */
   divider = (optInTimePeriod*(optInTimePeriod+1))>>1;

   /* The algo used here use a very basic property of
    * multiplication/addition: (x*2) = x+x
    *   
    * As an example, a 3 period weighted can be 
    * interpreted in two way:
    *  (x1*1)+(x2*2)+(x3*3)
    *      OR
    *  x1+x2+x2+x3+x3+x3 (this is the periodSum)
    *   
    * When you move forward in the time serie
    * you can quickly adjust the periodSum for the
    * period by substracting:
    *   x1+x2+x3 (This is the periodSub)
    * Making the new periodSum equals to:
    *   x2+x3+x3
    *
    * You can then add the new price bar
    * which is x4+x4+x4 giving:
    *   x2+x3+x3+x4+x4+x4
    *
    * At this point one iteration is completed and you can
    * see that we are back to the step 1 of this example.
    *
    * Why making it so un-intuitive? The number of memory
    * access and floating point operations are kept to a
    * minimum with this algo.
    */
   outIdx      = 0;
   trailingIdx = startIdx - lookbackTotal;

   /* Evaluate the initial periodSum/periodSub and trailingValue. */
   periodSum = periodSub = (double)0.0;
   inIdx=trailingIdx;
   i = 1;
   while( inIdx < startIdx )
   {
      tempReal = inReal[inIdx++];
      periodSub += tempReal;
      periodSum += tempReal*i;
      i++;
   }
   trailingValue = 0.0;

   /* Tight loop for the requested range. */
   while( inIdx <= endIdx )
   {
      /* Add the current price bar to the sum
       * who are carried through the iterations.
       */
      tempReal = inReal[inIdx++];
      periodSub += tempReal;
      periodSub -= trailingValue;
      periodSum += tempReal*optInTimePeriod;

      /* Save the trailing value for being substract at
       * the next iteration.
       * (must be saved here just in case outReal and
       *  inReal are the same buffer).
       */
      trailingValue = inReal[trailingIdx++];

      /* Calculate the WMA for this price bar. */
      outReal[outIdx++] = periodSum / divider;

      /* Prepare the periodSum for the next iteration. */
      periodSum -= periodSub;
   }

   /* Set output limits. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
