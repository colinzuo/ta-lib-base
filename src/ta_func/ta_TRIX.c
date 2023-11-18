#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_TRIX_Lookback( int           optInTimePeriod )  /* From 1 to 100000 */
{
   /* insert local variable here */
   int emaLookback;

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 30;
   else if( ((int)optInTimePeriod < 1) || ((int)optInTimePeriod > 100000) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */
   emaLookback = TA_EMA_Lookback( optInTimePeriod );
   return (emaLookback*3) + TA_ROCR_Lookback( 1 );
}

/*
 * TA_TRIX - 1-day Rate-Of-Change (ROC) of a Triple Smooth EMA
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_TRIX( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int           optInTimePeriod, /* From 1 to 100000 */
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] )
{
   /* Insert local variables here. */
   double k;
   ARRAY_REF(tempBuffer);
   int nbElement;
   int begIdx;
   int totalLookback;
   int emaLookback, rocLookback;
   TA_RetCode retCode;
   int nbElementToOutput;

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
   else if( ((int)optInTimePeriod < 1) || ((int)optInTimePeriod > 100000) )
      return TA_BAD_PARAM;

   if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* Adjust the startIdx to account for the lookback. */
   emaLookback   = TA_EMA_Lookback( optInTimePeriod );
   rocLookback   = TA_ROCR_Lookback( 1 );
   totalLookback = (emaLookback*3) + rocLookback;

   if( startIdx < totalLookback )
      startIdx = totalLookback;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
   {
      *outNBElement = 0;
      *outBegIdx = 0;
      return TA_SUCCESS; 
   }

   *outBegIdx = startIdx; 

   nbElementToOutput = (endIdx-startIdx)+1+totalLookback;

   /* Allocate a temporary buffer for performing
    * the calculation.
    */
   ARRAY_ALLOC(tempBuffer, nbElementToOutput );
   #if !defined( _JAVA )
      if( !tempBuffer )
      {
         *outNBElement = 0;
         *outBegIdx = 0;
         return TA_ALLOC_ERR;
      }
   #endif

   /* Calculate the first EMA */   
   k = PER_TO_K(optInTimePeriod);  
   retCode = TA_INT_EMA( (startIdx-totalLookback), endIdx, inReal,
                                     optInTimePeriod, k,
                                     &begIdx, &nbElement,
								     tempBuffer );
   
   /* Verify for failure or if not enough data after
    * calculating the EMA.
    */
   if( (retCode != TA_SUCCESS ) || (nbElement == 0) )
   {
      *outNBElement = 0;
      *outBegIdx = 0;
      ARRAY_FREE( tempBuffer );
      return retCode;
   }

   nbElementToOutput--; /* Make this variable zero base from now on. */

   /* Calculate the second EMA */   
   nbElementToOutput -= emaLookback;
   retCode = TA_INT_EMA( 0, nbElementToOutput, tempBuffer,
                                            optInTimePeriod, k,
                                            &begIdx, &nbElement,
						                    tempBuffer );
   
   /* Verify for failure or if not enough data after
    * calculating the EMA.
    */
   if( (retCode != TA_SUCCESS ) || (nbElement == 0) )
   {
      *outNBElement = 0;
      *outBegIdx = 0;
      ARRAY_FREE( tempBuffer );
      return retCode;
   }

   /* Calculate the third EMA */   
   nbElementToOutput -= emaLookback;
   retCode = TA_INT_EMA( 0, nbElementToOutput, tempBuffer,
                                            optInTimePeriod, k,
                                            &begIdx, &nbElement,
						                    tempBuffer );
   
   /* Verify for failure or if not enough data after
    * calculating the EMA.
    */
   if( (retCode != TA_SUCCESS ) || (nbElement == 0) )
   {
      *outNBElement = 0;
      *outBegIdx = 0;
      ARRAY_FREE( tempBuffer );
      return retCode;
   }

   /* Calculate the 1-day Rate-Of-Change */
   nbElementToOutput -= emaLookback;
   retCode = TA_ROC( 0, nbElementToOutput,
                                        tempBuffer,
                                        1,  &begIdx, outNBElement,
                                        outReal );

   ARRAY_FREE( tempBuffer );
   /* Verify for failure or if not enough data after
    * calculating the rate-of-change.
    */
   if( (retCode != TA_SUCCESS ) || ((int)(*outNBElement) == 0) )
   {
      *outNBElement = 0;
      *outBegIdx = 0;
      return retCode;
   }

   return TA_SUCCESS;
}
