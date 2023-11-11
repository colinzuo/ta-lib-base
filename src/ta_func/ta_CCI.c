#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_CCI_Lookback( int           optInTimePeriod )  /* From 2 to 100000 */
{
   /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 14;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */
   return (optInTimePeriod-1);
}

/*
 * TA_CCI - Commodity Channel Index
 * 
 * Input  = High, Low, Close
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_CCI( int    startIdx,
                   int    endIdx,
                   const double inHigh[],
                   const double inLow[],
                   const double inClose[],
                   int           optInTimePeriod, /* From 2 to 100000 */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] )
{

	/* insert local variable here */
   double tempReal, tempReal2, theAverage, lastValue;
   int i, j, outIdx, lookbackTotal;

   /* This ptr will points on a circular buffer of
    * at least "optInTimePeriod" element.
    */
   CIRCBUF_PROLOG(circBuffer,double,30);

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   /* Verify required price component. */
   if(!inHigh||!inLow||!inClose)
      return TA_BAD_PARAM;

      /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 14;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return TA_BAD_PARAM;

   if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */
   
   /* Identify the minimum number of price bar needed
    * to calculate at least one output.
    */
   lookbackTotal = (optInTimePeriod-1);

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

   /* Allocate a circular buffer equal to the requested
    * period.
    */
   CIRCBUF_INIT( circBuffer, double, optInTimePeriod );
  
   /* Do the MA calculation using tight loops. */

   /* Add-up the initial period, except for the last value. 
    * Fill up the circular buffer at the same time.
    */
   i=startIdx-lookbackTotal;
   if( optInTimePeriod > 1 )
   {
      while( i < startIdx )
      {
         circBuffer[circBuffer_Idx] = (inHigh[i]+inLow[i]+inClose[i])/3;
         i++;
         CIRCBUF_NEXT(circBuffer);
      }
   }

   /* Proceed with the calculation for the requested range.
    * Note that this algorithm allows the inReal and
    * outReal to be the same buffer.
    */
   outIdx = 0;
   do
   {
      lastValue = (inHigh[i]+inLow[i]+inClose[i])/3;
      circBuffer[circBuffer_Idx] = lastValue;

      /* Calculate the average for the whole period. */
      theAverage = 0;
      for( j=0; j < optInTimePeriod; j++ )
         theAverage += circBuffer[j];
      theAverage /= optInTimePeriod;

      /* Do the summation of the ABS(TypePrice-average)
       * for the whole period.
       */
      tempReal2 = 0;
      for( j=0; j < optInTimePeriod; j++ )
         tempReal2 += std_fabs(circBuffer[j]-theAverage);

      /* And finally, the CCI... */
      tempReal = lastValue-theAverage;

      if( (tempReal != 0.0) && (tempReal2 != 0.0) )
      {
         outReal[outIdx++] = tempReal/(0.015*(tempReal2/optInTimePeriod));
      }
      else
         outReal[outIdx++] = 0.0;
      
      /* Move forward the circular buffer indexes. */
      CIRCBUF_NEXT(circBuffer);

      i++;
   } while( i <= endIdx );

   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   /* Free the circular buffer if it was dynamically allocated. */
   CIRCBUF_DESTROY(circBuffer);

   return TA_SUCCESS;
}
