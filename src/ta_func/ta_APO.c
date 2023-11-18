#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_APO_Lookback( int           optInFastPeriod, /* From 2 to 100000 */
                   int           optInSlowPeriod, /* From 2 to 100000 */
                   TA_MAType     optInMAType ) 
{
   /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInFastPeriod. */
   if( (int)optInFastPeriod == TA_INTEGER_DEFAULT )
      optInFastPeriod = 12;
   else if( ((int)optInFastPeriod < 2) || ((int)optInFastPeriod > 100000) )
      return -1;

   /* min/max are checked for optInSlowPeriod. */
   if( (int)optInSlowPeriod == TA_INTEGER_DEFAULT )
      optInSlowPeriod = 26;
   else if( ((int)optInSlowPeriod < 2) || ((int)optInSlowPeriod > 100000) )
      return -1;

   if( (int)optInMAType == TA_INTEGER_DEFAULT )
      optInMAType = (TA_MAType)0;
   else if( ((int)optInMAType < 0) || ((int)optInMAType > 8) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */

   /* The slow MA is the key factor determining the lookback period. */
   return TA_MA_Lookback( max(optInSlowPeriod,optInFastPeriod), optInMAType );
}

/*
 * TA_APO - Absolute Price Oscillator
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInFastPeriod:(From 2 to 100000)
 *    Number of period for the fast MA
 * 
 * optInSlowPeriod:(From 2 to 100000)
 *    Number of period for the slow MA
 * 
 * optInMAType:
 *    Type of Moving Average
 * 
 * 
 */
TA_RetCode TA_APO( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInFastPeriod, /* From 2 to 100000 */
                   int           optInSlowPeriod, /* From 2 to 100000 */
                   TA_MAType     optInMAType,
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] )
{
   /* Insert local variables here. */
   ARRAY_REF(tempBuffer);
   TA_RetCode retCode;

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   if( !inReal ) return TA_BAD_PARAM;
      /* min/max are checked for optInFastPeriod. */
   if( (int)optInFastPeriod == TA_INTEGER_DEFAULT )
      optInFastPeriod = 12;
   else if( ((int)optInFastPeriod < 2) || ((int)optInFastPeriod > 100000) )
      return TA_BAD_PARAM;

   /* min/max are checked for optInSlowPeriod. */
   if( (int)optInSlowPeriod == TA_INTEGER_DEFAULT )
      optInSlowPeriod = 26;
   else if( ((int)optInSlowPeriod < 2) || ((int)optInSlowPeriod > 100000) )
      return TA_BAD_PARAM;

   if( (int)optInMAType == TA_INTEGER_DEFAULT )
      optInMAType = (TA_MAType)0;
   else if( ((int)optInMAType < 0) || ((int)optInMAType > 8) )
      return TA_BAD_PARAM;

   if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* Allocate an intermediate buffer. */
   ARRAY_ALLOC(tempBuffer, (endIdx-startIdx+1) );
   #if !defined(_JAVA)
      if( !tempBuffer )
         return TA_ALLOC_ERR;
   #endif

   retCode = TA_INT_PO( startIdx, endIdx,                        
                                    inReal,
                                    optInFastPeriod, 
                                    optInSlowPeriod, 
                                    optInMAType,
                                    outBegIdx,
                                    outNBElement,
                                    outReal,
                                    tempBuffer,
                                    0 /* No percentage. */ );

   ARRAY_FREE( tempBuffer );

   return retCode;
}

/* Internal price oscillator function.
 *
 * A buffer must be provided for intermediate processing.
 */
TA_RetCode TA_INT_PO( int    startIdx,
                              int    endIdx,
                              const double *inReal,
                              int    optInFastPeriod, 
                              int    optInSlowPeriod, 
                              TA_MAType optInMethod_2,
                              int   *outBegIdx,
                              int   *outNBElement,
                              double      *outReal,
                              double      *tempBuffer,
                              int  doPercentageOutput )
{
   TA_RetCode retCode;

   double tempReal;
   int tempInteger;
   int outBegIdx1;
   int outNbElement1;
   int outBegIdx2;
   int outNbElement2;

   int i, j;

   /* Make sure slow is really slower than
    * the fast period! if not, swap...
    */
   if( optInSlowPeriod < optInFastPeriod )
   {
       /* swap */
       tempInteger     = optInSlowPeriod;
       optInSlowPeriod = optInFastPeriod;
       optInFastPeriod = tempInteger;
   }

   /* Calculate the fast MA into the tempBuffer. */
   retCode = TA_MA( startIdx, endIdx,
                                inReal,
                                optInFastPeriod, 
                                optInMethod_2,                    
                                &outBegIdx2, &outNbElement2,
							    tempBuffer );

   if( retCode == TA_SUCCESS )
   {
      /* Calculate the slow MA into the output. */
      retCode = TA_MA( startIdx, endIdx,                       
                                   inReal,
                                   optInSlowPeriod, 
                                   optInMethod_2,
                                   &outBegIdx1, &outNbElement1,
							       outReal );

      if( retCode == TA_SUCCESS )
      {
         tempInteger = outBegIdx1 - outBegIdx2;
         if( doPercentageOutput != 0 )
         {
            /* Calculate ((fast MA)-(slow MA))/(slow MA) in the output. */   
            for( i=0,j=tempInteger; i < outNbElement1; i++, j++ )
            {
               tempReal = outReal[i];
               if( !TA_IS_ZERO(tempReal) )
                  outReal[i] = ((tempBuffer[j]-tempReal)/tempReal)*100.0;
               else
                  outReal[i] = 0.0;
            }
         }
         else
         {
            /* Calculate (fast MA)-(slow MA) in the output. */   
            for( i=0,j=tempInteger; i < outNbElement1; i++, j++ )
               outReal[i] = tempBuffer[j]-outReal[i];
         }

         *outBegIdx     = outBegIdx1;
         *outNBElement  = outNbElement1;
      }
   }

   if( retCode != TA_SUCCESS )
   {
      *outBegIdx = 0;
      *outNBElement = 0;
   }

   return retCode;
}
