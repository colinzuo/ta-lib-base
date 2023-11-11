#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_MAVP_Lookback( int           optInMinPeriod, /* From 2 to 100000 */
                    int           optInMaxPeriod, /* From 2 to 100000 */
                    TA_MAType     optInMAType ) 
{
   /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInMinPeriod. */
   if( (int)optInMinPeriod == TA_INTEGER_DEFAULT )
      optInMinPeriod = 2;
   else if( ((int)optInMinPeriod < 2) || ((int)optInMinPeriod > 100000) )
      return -1;

   /* min/max are checked for optInMaxPeriod. */
   if( (int)optInMaxPeriod == TA_INTEGER_DEFAULT )
      optInMaxPeriod = 30;
   else if( ((int)optInMaxPeriod < 2) || ((int)optInMaxPeriod > 100000) )
      return -1;

   if( (int)optInMAType == TA_INTEGER_DEFAULT )
      optInMAType = (TA_MAType)0;
   else if( ((int)optInMAType < 0) || ((int)optInMAType > 8) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   return TA_MA_Lookback(optInMaxPeriod, optInMAType);
}

/*
 * TA_MAVP - Moving average with variable period
 * 
 * Input  = double, double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInMinPeriod:(From 2 to 100000)
 *    Value less than minimum will be changed to Minimum period
 * 
 * optInMaxPeriod:(From 2 to 100000)
 *    Value higher than maximum will be changed to Maximum period
 * 
 * optInMAType:
 *    Type of Moving Average
 * 
 * 
 */
TA_RetCode TA_MAVP( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    const double inPeriods[],
                    int           optInMinPeriod, /* From 2 to 100000 */
                    int           optInMaxPeriod, /* From 2 to 100000 */
                    TA_MAType     optInMAType,
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] )
{
   /* insert local variable here */
   int i, j, lookbackTotal, outputSize, tempInt, curPeriod;
   ARRAY_INT_REF(localPeriodArray);
   ARRAY_REF(localOutputArray);
   int localBegIdx;
   int localNbElement;
   TA_RetCode retCode;

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   if( !inReal ) return TA_BAD_PARAM;
   if( !inPeriods ) return TA_BAD_PARAM;
      /* min/max are checked for optInMinPeriod. */
   if( (int)optInMinPeriod == TA_INTEGER_DEFAULT )
      optInMinPeriod = 2;
   else if( ((int)optInMinPeriod < 2) || ((int)optInMinPeriod > 100000) )
      return TA_BAD_PARAM;

   /* min/max are checked for optInMaxPeriod. */
   if( (int)optInMaxPeriod == TA_INTEGER_DEFAULT )
      optInMaxPeriod = 30;
   else if( ((int)optInMaxPeriod < 2) || ((int)optInMaxPeriod > 100000) )
      return TA_BAD_PARAM;

   if( (int)optInMAType == TA_INTEGER_DEFAULT )
      optInMAType = (TA_MAType)0;
   else if( ((int)optInMAType < 0) || ((int)optInMAType > 8) )
      return TA_BAD_PARAM;

   if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* Identify the minimum number of price bar needed
    * to calculate at least one output.
    */
   lookbackTotal = TA_MA_Lookback(optInMaxPeriod,optInMAType);

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

   /* Calculate exact output size */      
   if( lookbackTotal > startIdx )
      tempInt = lookbackTotal;
   else
	  tempInt = startIdx;
   if( tempInt > endIdx )
   {
	  /* No output */
      *outBegIdx = 0;
      *outNBElement = 0;
      return TA_SUCCESS;      
   }   
   outputSize = endIdx - tempInt + 1;

   /* Allocate intermediate local buffer. */
   ARRAY_ALLOC(localOutputArray,outputSize);
   ARRAY_INT_ALLOC(localPeriodArray,outputSize);

   /* Copy caller array of period into local buffer.
    * At the same time, truncate to min/max.
	*/
   for( i=0; i < outputSize; i++ )
   {
      tempInt = (int)(inPeriods[startIdx+i]);
	  if( tempInt < optInMinPeriod )
		  tempInt = optInMinPeriod;
	  else if( tempInt > optInMaxPeriod )
          tempInt = optInMaxPeriod;
      localPeriodArray[i] = tempInt;
   }

   /* Process each element of the input. 
    * For each possible period value, the MA is calculated 
	* only once.
	* The outReal is then fill up for all element with
	* the same period.
	* A local flag (value 0) is set in localPeriodArray
	* to avoid doing a second time the same calculation.
	*/   
   for( i=0; i < outputSize; i++ )
   {
	   curPeriod = localPeriodArray[i];
	   if( curPeriod != 0 )
	   {
           /* TODO: This portion of the function can be slightly speed 
            *       optimized by making the function without unstable period 
	        *       start their calculation at 'startIdx+i' instead of startIdx.			
	        */

		   /* Calculation of the MA required. */
		   retCode = TA_PREFIX(MA)( startIdx, endIdx, inReal,
			                            curPeriod, optInMAType,
									    &localBegIdx,&localNbElement,localOutputArray );

           if( retCode != TA_SUCCESS )
		   {
              ARRAY_FREE(localOutputArray);
			  ARRAY_INT_FREE(localPeriodArray);
              *outBegIdx = 0;
              *outNBElement = 0;
              return retCode;
	       }
		   
		   outReal[i] = localOutputArray[i];
		   for( j=i+1; j < outputSize; j++ )
		   {
			   if( localPeriodArray[j] == curPeriod )
			   {
				   localPeriodArray[j] = 0; /* Flag to avoid recalculation */
				   outReal[j] = localOutputArray[j];
			   }
		   }		   
	   }
   }

   ARRAY_FREE(localOutputArray);
   ARRAY_INT_FREE(localPeriodArray);

   /* Done. Inform the caller of the success. */
   *outBegIdx = startIdx;
   *outNBElement = outputSize;
   return TA_SUCCESS;
}
