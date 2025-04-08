
/* List of contributors:
 *
 *  Initial  Name/description
 *  -------------------------------------------------------------------
 *  RM       Robert Meier
 *  MF       Mario Fortier
 *
 * Change history:
 *
 *  MMDDYY BY     Description
 *  -------------------------------------------------------------------
 *  120307 RM     Initial Version
 *  120907 MF     Handling of a few limit cases
 */

#pragma warning(disable: 6386) 

#include <string.h>
#include <math.h>

#include "ta_func.h"
#include "ta_utility.h"
#include "ta_memory.h"


int TA_ACCBANDS_Lookback( int           optInTimePeriod )  /* From 2 to 100000 */
{
   /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 20;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */

   return TA_SMA_Lookback( optInTimePeriod );
}

/*
 * TA_ACCBANDS - Acceleration Bands
 * 
 * Input  = High, Low, Close
 * Output = double, double, double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_ACCBANDS( int    startIdx,
                        int    endIdx,
                        const double inHigh[],
                        const double inLow[],
                        const double inClose[],
                        int           optInTimePeriod, /* From 2 to 100000 */
                        int          *outBegIdx,
                        int          *outNBElement,
                        double        outRealUpperBand[],
                        double        outRealMiddleBand[],
                        double        outRealLowerBand[] )
{
   /* insert local variable here */
   TA_RetCode retCode;
   ARRAY_REF( tempBuffer1 );
   ARRAY_REF( tempBuffer2 );
   int outBegIdxDummy;
   int outNbElementDummy;
   int i, j, outputSize, bufferSize, lookbackTotal;
   double tempReal;


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
      optInTimePeriod = 20;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return TA_BAD_PARAM;

   if( !outRealUpperBand )
      return TA_BAD_PARAM;

   if( !outRealMiddleBand )
      return TA_BAD_PARAM;

   if( !outRealLowerBand )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */


   /* Identify the minimum number of price bar needed
    * to calculate at least one output.
    */
   lookbackTotal = TA_SMA_Lookback( optInTimePeriod );

   /* Move up the start index if there is not
    * enough initial data.
    */
   if( startIdx < lookbackTotal )
      startIdx = lookbackTotal;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
   {
      (*outBegIdx) = 0;
      (*outNBElement) = 0;
      return TA_SUCCESS;
   }

   /* Buffer will contains also the lookback required for SMA
    * to satisfy the caller requested startIdx/endIdx.
	*/
   outputSize = endIdx-startIdx+1;
   bufferSize = outputSize+lookbackTotal;

   ARRAY_ALLOC(tempBuffer1, bufferSize );

    if ( !tempBuffer1 )
	{
        (*outBegIdx) = 0;
        (*outNBElement) = 0;
        return TA_ALLOC_ERR;
	}

   ARRAY_ALLOC(tempBuffer2, bufferSize );

    if ( !tempBuffer2 )
	{
		ARRAY_FREE(tempBuffer1);
        (*outBegIdx) = 0;
        (*outNBElement) = 0;
        return TA_ALLOC_ERR;
	}

   /* Calculate the upper/lower band at the same time (no SMA yet).
    * Must start calculation back enough to cover the lookback
    * required later for the SMA.
	*/
   for(j=0, i=startIdx-lookbackTotal; i<=endIdx; i++, j++)
   {
	    tempReal = inHigh[i]+inLow[i];
	    if( !TA_IS_ZERO(tempReal) )
		{
		   tempReal = 4*(inHigh[i]-inLow[i])/tempReal;
		   tempBuffer1[j] = inHigh[i]*(1+tempReal);
		   tempBuffer2[j] = inLow[i]*(1-tempReal);
		}
		else
		{
		   tempBuffer1[j] = inHigh[i];
		   tempBuffer2[j] = inLow[i];
		}
   }

   /* Calculate the middle band, which is a moving average of the close. */
   retCode = TA_SMA( startIdx, endIdx, inClose,
                                optInTimePeriod,
                                &outBegIdxDummy, &outNbElementDummy, outRealMiddleBand );

   if( (retCode != TA_SUCCESS ) || ((int)outNbElementDummy != outputSize) )
   {
      ARRAY_FREE( tempBuffer1 );
      ARRAY_FREE( tempBuffer2 );
      (*outBegIdx) = 0;
      (*outNBElement) = 0;
      return retCode;
   }

   /* Now let's take the SMA for the upper band. */
   retCode = TA_SMA( 0, bufferSize-1, tempBuffer1,
                                optInTimePeriod,
                                &outBegIdxDummy, &outNbElementDummy,
								outRealUpperBand );

   if( (retCode != TA_SUCCESS ) || ((int)outNbElementDummy != outputSize) )
   {
      ARRAY_FREE( tempBuffer1 );
      ARRAY_FREE( tempBuffer2 );
      (*outBegIdx) = 0;
      (*outNBElement) = 0;
      return retCode;
   }

   /* Now let's take the SMA for the lower band. */
   retCode = TA_SMA( 0, bufferSize-1, tempBuffer2,
                                optInTimePeriod,
                                &outBegIdxDummy, &outNbElementDummy,
								outRealLowerBand );

   ARRAY_FREE( tempBuffer1 );
   ARRAY_FREE( tempBuffer2 );

   if( (retCode != TA_SUCCESS ) || ((int)outNbElementDummy != outputSize) )
   {
      (*outBegIdx) = 0;
      (*outNBElement) = 0;
      return retCode;
   }

   (*outBegIdx) = startIdx;
   (*outNBElement) = outputSize;

   return TA_SUCCESS;
}
