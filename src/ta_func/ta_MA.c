#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_MA_Lookback( int           optInTimePeriod, /* From 1 to 100000 */
                  TA_MAType     optInMAType ) 
{
   /* insert local variable here */
   int retValue;

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 30;
   else if( ((int)optInTimePeriod < 1) || ((int)optInTimePeriod > 100000) )
      return -1;

   if( (int)optInMAType == TA_INTEGER_DEFAULT )
      optInMAType = (TA_MAType)0;
   else if( ((int)optInMAType < 0) || ((int)optInMAType > 8) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */

   if( optInTimePeriod <= 1 )
      return 0;
   
   switch( optInMAType )
   {
   case TA_MAType_SMA:
      retValue = TA_SMA_Lookback( optInTimePeriod );
      break;

   case TA_MAType_EMA:
      retValue = TA_EMA_Lookback( optInTimePeriod );
      break;

   case TA_MAType_WMA:
      retValue = TA_WMA_Lookback( optInTimePeriod );
      break;

   case TA_MAType_DEMA:
      retValue = TA_DEMA_Lookback( optInTimePeriod );
      break;

   case TA_MAType_TEMA:
      retValue = TA_TEMA_Lookback( optInTimePeriod );
      break;

   case TA_MAType_TRIMA:
      retValue = TA_TRIMA_Lookback( optInTimePeriod );
      break;

   case TA_MAType_KAMA:
      retValue = TA_KAMA_Lookback( optInTimePeriod );
      break;

   case TA_MAType_MAMA:
      retValue = TA_MAMA_Lookback( 0.5, 0.05 );
      break;

   case TA_MAType_T3:
      retValue = TA_T3_Lookback( optInTimePeriod, 0.7 );
      break;

   default:
      retValue = 0;
   }

   return retValue;
}

/*
 * TA_MA - Moving average
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * optInMAType:
 *    Type of Moving Average
 * 
 * 
 */
TA_RetCode TA_MA( int    startIdx,
                  int    endIdx,
                  const double inReal[],
                  int           optInTimePeriod, /* From 1 to 100000 */
                  TA_MAType     optInMAType,
                  int          *outBegIdx,
                  int          *outNBElement,
                  double        outReal[] )
{
   /* Insert local variables here. */
   ARRAY_REF(dummyBuffer);
   TA_RetCode retCode;

   int nbElement;
   int outIdx, todayIdx;

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

   if( (int)optInMAType == TA_INTEGER_DEFAULT )
      optInMAType = (TA_MAType)0;
   else if( ((int)optInMAType < 0) || ((int)optInMAType > 8) )
      return TA_BAD_PARAM;

   if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   if( optInTimePeriod == 1 )
   {
      nbElement = endIdx-startIdx+1;
      *outNBElement = nbElement;      
      for( todayIdx=startIdx, outIdx=0; outIdx < nbElement; outIdx++, todayIdx++ )
         outReal[outIdx] = inReal[todayIdx];
      *outBegIdx    = startIdx;
      return TA_SUCCESS;
   }
   /* Simply forward the job to the corresponding TA function. */
   switch( optInMAType )
   {
   case TA_MAType_SMA:
      retCode = TA_SMA( startIdx, endIdx, inReal, optInTimePeriod,
                                    outBegIdx, outNBElement, outReal );
      break;

   case TA_MAType_EMA:
      retCode = TA_EMA( startIdx, endIdx, inReal, optInTimePeriod,                                     
                                    outBegIdx, outNBElement, outReal );
      break;

   case TA_MAType_WMA:
      retCode = TA_WMA( startIdx, endIdx, inReal, optInTimePeriod,
                                    outBegIdx, outNBElement, outReal );
      break;

   case TA_MAType_DEMA:
      retCode = TA_DEMA( startIdx, endIdx, inReal, optInTimePeriod,
                                     outBegIdx, outNBElement, outReal );
      break;

   case TA_MAType_TEMA:
      retCode = TA_TEMA( startIdx, endIdx, inReal, optInTimePeriod,
                                     outBegIdx, outNBElement, outReal );
      break;

   case TA_MAType_TRIMA:
      retCode = TA_TRIMA( startIdx, endIdx, inReal, optInTimePeriod,
                                      outBegIdx, outNBElement, outReal );
      break;

   case TA_MAType_KAMA:
      retCode = TA_KAMA( startIdx, endIdx, inReal, optInTimePeriod,
                                     outBegIdx, outNBElement, outReal );
      break;

   case TA_MAType_MAMA:
      /* The optInTimePeriod is ignored and the FAMA output of the MAMA
       * is ignored.
       */
      ARRAY_ALLOC(dummyBuffer, (endIdx-startIdx+1) );

      #if !defined( _JAVA )
         if( !dummyBuffer )
            return TA_ALLOC_ERR;
      #endif

      retCode = TA_MAMA( startIdx, endIdx, inReal, 0.5, 0.05,                           
                                     outBegIdx, outNBElement,
                                     outReal, dummyBuffer );
                         
      ARRAY_FREE( dummyBuffer );
      break;

   case TA_MAType_T3:
      retCode = TA_T3( startIdx, endIdx, inReal,
                                   optInTimePeriod, 0.7,
                                   outBegIdx, outNBElement, outReal );
      break;

   default: 
      retCode = TA_BAD_PARAM;
      break;
   }

   return retCode;
}
