#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_MACDEXT_Lookback( int           optInFastPeriod, /* From 2 to 100000 */
                       TA_MAType     optInFastMAType,
                       int           optInSlowPeriod, /* From 2 to 100000 */
                       TA_MAType     optInSlowMAType,
                       int           optInSignalPeriod, /* From 1 to 100000 */
                       TA_MAType     optInSignalMAType ) 
{
   /* insert local variable here */
   int tempInteger, lookbackLargest;

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInFastPeriod. */
   if( (int)optInFastPeriod == TA_INTEGER_DEFAULT )
      optInFastPeriod = 12;
   else if( ((int)optInFastPeriod < 2) || ((int)optInFastPeriod > 100000) )
      return -1;

   if( (int)optInFastMAType == TA_INTEGER_DEFAULT )
      optInFastMAType = (TA_MAType)0;
   else if( ((int)optInFastMAType < 0) || ((int)optInFastMAType > 8) )
      return -1;

   /* min/max are checked for optInSlowPeriod. */
   if( (int)optInSlowPeriod == TA_INTEGER_DEFAULT )
      optInSlowPeriod = 26;
   else if( ((int)optInSlowPeriod < 2) || ((int)optInSlowPeriod > 100000) )
      return -1;

   if( (int)optInSlowMAType == TA_INTEGER_DEFAULT )
      optInSlowMAType = (TA_MAType)0;
   else if( ((int)optInSlowMAType < 0) || ((int)optInSlowMAType > 8) )
      return -1;

   /* min/max are checked for optInSignalPeriod. */
   if( (int)optInSignalPeriod == TA_INTEGER_DEFAULT )
      optInSignalPeriod = 9;
   else if( ((int)optInSignalPeriod < 1) || ((int)optInSignalPeriod > 100000) )
      return -1;

   if( (int)optInSignalMAType == TA_INTEGER_DEFAULT )
      optInSignalMAType = (TA_MAType)0;
   else if( ((int)optInSignalMAType < 0) || ((int)optInSignalMAType > 8) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */

   /* Find the MA with the largest lookback */
   lookbackLargest = TA_MA_Lookback( optInFastPeriod, optInFastMAType );
   tempInteger     = TA_MA_Lookback( optInSlowPeriod, optInSlowMAType );
   if( tempInteger > lookbackLargest )
      lookbackLargest = tempInteger;

   /* Add to the largest MA lookback the signal line lookback */
   return lookbackLargest + TA_MA_Lookback( optInSignalPeriod, optInSignalMAType );
}

/*
 * TA_MACDEXT - MACD with controllable MA type
 * 
 * Input  = double
 * Output = double, double, double
 * 
 * Optional Parameters
 * -------------------
 * optInFastPeriod:(From 2 to 100000)
 *    Number of period for the fast MA
 * 
 * optInFastMAType:
 *    Type of Moving Average for fast MA
 * 
 * optInSlowPeriod:(From 2 to 100000)
 *    Number of period for the slow MA
 * 
 * optInSlowMAType:
 *    Type of Moving Average for slow MA
 * 
 * optInSignalPeriod:(From 1 to 100000)
 *    Smoothing for the signal line (nb of period)
 * 
 * optInSignalMAType:
 *    Type of Moving Average for signal line
 * 
 * 
 */
TA_RetCode TA_MACDEXT( int    startIdx,
                       int    endIdx,
                       const double inReal[],
                       int           optInFastPeriod, /* From 2 to 100000 */
                       TA_MAType     optInFastMAType,
                       int           optInSlowPeriod, /* From 2 to 100000 */
                       TA_MAType     optInSlowMAType,
                       int           optInSignalPeriod, /* From 1 to 100000 */
                       TA_MAType     optInSignalMAType,
                       int          *outBegIdx,
                       int          *outNBElement,
                       double        outMACD[],
                       double        outMACDSignal[],
                       double        outMACDHist[] )
{
	/* insert local variable here */
   ARRAY_REF( slowMABuffer );
   ARRAY_REF( fastMABuffer );
   TA_RetCode retCode;
   int tempInteger;
   int outBegIdx1;
   int outNbElement1;
   int outBegIdx2;
   int outNbElement2;
   int lookbackTotal, lookbackSignal, lookbackLargest;
   int i;
   TA_MAType tempMAType;

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

   if( (int)optInFastMAType == TA_INTEGER_DEFAULT )
      optInFastMAType = (TA_MAType)0;
   else if( ((int)optInFastMAType < 0) || ((int)optInFastMAType > 8) )
      return TA_BAD_PARAM;

   /* min/max are checked for optInSlowPeriod. */
   if( (int)optInSlowPeriod == TA_INTEGER_DEFAULT )
      optInSlowPeriod = 26;
   else if( ((int)optInSlowPeriod < 2) || ((int)optInSlowPeriod > 100000) )
      return TA_BAD_PARAM;

   if( (int)optInSlowMAType == TA_INTEGER_DEFAULT )
      optInSlowMAType = (TA_MAType)0;
   else if( ((int)optInSlowMAType < 0) || ((int)optInSlowMAType > 8) )
      return TA_BAD_PARAM;

   /* min/max are checked for optInSignalPeriod. */
   if( (int)optInSignalPeriod == TA_INTEGER_DEFAULT )
      optInSignalPeriod = 9;
   else if( ((int)optInSignalPeriod < 1) || ((int)optInSignalPeriod > 100000) )
      return TA_BAD_PARAM;

   if( (int)optInSignalMAType == TA_INTEGER_DEFAULT )
      optInSignalMAType = (TA_MAType)0;
   else if( ((int)optInSignalMAType < 0) || ((int)optInSignalMAType > 8) )
      return TA_BAD_PARAM;

   if( !outMACD )
      return TA_BAD_PARAM;

   if( !outMACDSignal )
      return TA_BAD_PARAM;

   if( !outMACDHist )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* Make sure slow is really slower than
    * the fast period! if not, swap...
    */
   if( optInSlowPeriod < optInFastPeriod )
   {
       /* swap period */
       tempInteger     = optInSlowPeriod;
       optInSlowPeriod = optInFastPeriod;
       optInFastPeriod = tempInteger;
       /* swap type */
       tempMAType      = optInSlowMAType;
       optInSlowMAType = optInFastMAType;
       optInFastMAType = tempMAType;
   }

   /* Find the MA with the largest lookback */
   lookbackLargest = TA_MA_Lookback( optInFastPeriod, optInFastMAType );
   tempInteger     = TA_MA_Lookback( optInSlowPeriod, optInSlowMAType );
   if( tempInteger > lookbackLargest )
      lookbackLargest = tempInteger;

   /* Add the lookback needed for the signal line */
   lookbackSignal = TA_MA_Lookback( optInSignalPeriod, optInSignalMAType ); 
   lookbackTotal  = lookbackSignal+lookbackLargest;

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

   /* Allocate intermediate buffer for fast/slow MA. */
   tempInteger = (endIdx-startIdx)+1+lookbackSignal;
   ARRAY_ALLOC( fastMABuffer, tempInteger );
   #if !defined( _JAVA )
      if( !fastMABuffer )
      {
         *outBegIdx = 0;
         *outNBElement = 0;
         return TA_ALLOC_ERR;
      }
   #endif

   ARRAY_ALLOC( slowMABuffer, tempInteger );
   #if !defined( _JAVA )
      if( !slowMABuffer )
      { 
         *outBegIdx = 0;
         *outNBElement = 0;
         ARRAY_FREE( fastMABuffer );
         return TA_ALLOC_ERR;
      }
   #endif

   /* Calculate the slow MA. 
    *
    * Move back the startIdx to get enough data
    * for the signal period. That way, once the
    * signal calculation is done, all the output
    * will start at the requested 'startIdx'.
    */
   tempInteger = startIdx-lookbackSignal;
   retCode = TA_PREFIX(MA)( tempInteger, endIdx,
                                inReal, optInSlowPeriod, optInSlowMAType,
                                &outBegIdx1, &outNbElement1, 
							    slowMABuffer );

   if( retCode != TA_SUCCESS )
   {
      *outBegIdx = 0;
      *outNBElement = 0;
      ARRAY_FREE( fastMABuffer );
      ARRAY_FREE( slowMABuffer );
      return retCode;
   }

   /* Calculate the fast MA. */
   retCode = TA_PREFIX(MA)( tempInteger, endIdx,
                                inReal, optInFastPeriod, optInFastMAType,
                                &outBegIdx2, &outNbElement2,
							    fastMABuffer );

   if( retCode != TA_SUCCESS )
   {
      *outBegIdx = 0;
      *outNBElement = 0;
      ARRAY_FREE( fastMABuffer );
      ARRAY_FREE( slowMABuffer );
      return retCode;
   }

   /* Parano tests. Will be removed eventually. */
   if( (outBegIdx1 != tempInteger) || 
       (outBegIdx2 != tempInteger) || 
       (outNbElement1 != outNbElement2) ||
       (outNbElement1 != (endIdx-startIdx)+1+lookbackSignal) )
   {
      *outBegIdx = 0;
      *outNBElement = 0;
      ARRAY_FREE( fastMABuffer );
      ARRAY_FREE( slowMABuffer );
      return TA_INTERNAL_ERROR(119);
   }

   /* Calculate (fast MA) - (slow MA). */
   for( i=0; i < outNbElement1; i++ )
      fastMABuffer[i] = fastMABuffer[i] - slowMABuffer[i];

   /* Copy the result into the output for the caller. */
   ARRAY_MEMMOVE( outMACD, 0, fastMABuffer, lookbackSignal, (endIdx-startIdx)+1 );

   /* Calculate the signal/trigger line. */
   retCode = TA_MA( 0, outNbElement1-1,
                                       fastMABuffer, optInSignalPeriod, optInSignalMAType,
                                       &outBegIdx2, &outNbElement2, outMACDSignal );

   ARRAY_FREE( fastMABuffer );
   ARRAY_FREE( slowMABuffer );

   if( retCode != TA_SUCCESS )
   {
      *outBegIdx = 0;
      *outNBElement = 0;
      return retCode;
   }

   /* Calculate the histogram. */
   for( i=0; i < outNbElement2; i++ )
      outMACDHist[i] = outMACD[i]-outMACDSignal[i];

   /* All done! Indicate the output limits and return success. */
   *outBegIdx     = startIdx;
   *outNBElement  = outNbElement2;

   return TA_SUCCESS;
}
