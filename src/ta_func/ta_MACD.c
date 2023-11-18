#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_MACD_Lookback( int           optInFastPeriod, /* From 2 to 100000 */
                    int           optInSlowPeriod, /* From 2 to 100000 */
                    int           optInSignalPeriod )  /* From 1 to 100000 */
{
   /* insert local variable here */
   int tempInteger;

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

   /* min/max are checked for optInSignalPeriod. */
   if( (int)optInSignalPeriod == TA_INTEGER_DEFAULT )
      optInSignalPeriod = 9;
   else if( ((int)optInSignalPeriod < 1) || ((int)optInSignalPeriod > 100000) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */

   /* The lookback is driven by the signal line output.
    *
    * (must also account for the initial data consume 
    *  by the slow period).
    */

   /* Make sure slow is really slower than
    * the fast period! if not, swap...
    */
   if( optInSlowPeriod < optInFastPeriod )
   {
       /* swap */
       tempInteger       = optInSlowPeriod;
       optInSlowPeriod = optInFastPeriod;
       optInFastPeriod = tempInteger;
   }

   return   TA_EMA_Lookback( optInSlowPeriod   )
          + TA_EMA_Lookback( optInSignalPeriod );
}

/*
 * TA_MACD - Moving Average Convergence/Divergence
 * 
 * Input  = double
 * Output = double, double, double
 * 
 * Optional Parameters
 * -------------------
 * optInFastPeriod:(From 2 to 100000)
 *    Number of period for the fast MA
 * 
 * optInSlowPeriod:(From 2 to 100000)
 *    Number of period for the slow MA
 * 
 * optInSignalPeriod:(From 1 to 100000)
 *    Smoothing for the signal line (nb of period)
 * 
 * 
 */
TA_RetCode TA_MACD( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int           optInFastPeriod, /* From 2 to 100000 */
                    int           optInSlowPeriod, /* From 2 to 100000 */
                    int           optInSignalPeriod, /* From 1 to 100000 */
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outMACD[],
                    double        outMACDSignal[],
                    double        outMACDHist[] )
{
   /* Insert local variables here. */

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

   /* min/max are checked for optInSignalPeriod. */
   if( (int)optInSignalPeriod == TA_INTEGER_DEFAULT )
      optInSignalPeriod = 9;
   else if( ((int)optInSignalPeriod < 1) || ((int)optInSignalPeriod > 100000) )
      return TA_BAD_PARAM;

   if( !outMACD )
      return TA_BAD_PARAM;

   if( !outMACDSignal )
      return TA_BAD_PARAM;

   if( !outMACDHist )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */  

   return TA_INT_MACD( startIdx, endIdx, inReal,
                                   optInFastPeriod,
                                   optInSlowPeriod,
                                   optInSignalPeriod,
                                   outBegIdx,
                                   outNBElement,
                                   outMACD,
                                   outMACDSignal,
                                   outMACDHist );
}

TA_RetCode TA_INT_MACD( int    startIdx,
                                int    endIdx,
                                const double inReal[],
                                int    optInFastPeriod, /* 0 is fix 12 */
                                int    optInSlowPeriod, /* 0 is fix 26 */
                                int    optInSignalPeriod_2,
                                int   *outBegIdx,
                                int   *outNBElement,
                                double       outMACD[],
                                double       outMACDSignal[],
                                double       outMACDHist[] )
{
   ARRAY_REF(slowEMABuffer);
   ARRAY_REF(fastEMABuffer);
   double k1, k2;
   TA_RetCode retCode;
   int tempInteger;
   int outBegIdx1;
   int outNbElement1;
   int outBegIdx2;
   int outNbElement2;
   int lookbackTotal, lookbackSignal;
   int i;

   /* !!! A lot of speed optimization could be done
    * !!! with this function. 
    * !!!
    * !!! A better approach would be to use TA_INT_EMA
    * !!! just to get the seeding values for the
    * !!! fast and slow EMA. Then process the difference
    * !!! in an allocated buffer until enough data is
    * !!! available for the first signal value.
    * !!! From that point all the processing can
    * !!! be done in a tight loop.
    * !!!
    * !!! That approach will have the following
    * !!! advantage:
    * !!!   1) One mem allocation needed instead of two.
    * !!!   2) The mem allocation size will be only the
    * !!!      signal lookback period instead of the 
    * !!!      whole range of data.
    * !!!   3) Processing will be done in a tight loop.
    * !!!      allowing to avoid a lot of memory store-load
    * !!!      operation.
    * !!!   4) The memcpy at the end will be eliminated!
    * !!!
    * !!! If only I had time....
    */

   /* Make sure slow is really slower than
    * the fast period! if not, swap...
    */
   if( optInSlowPeriod < optInFastPeriod )
   {
       /* swap */
       tempInteger       = optInSlowPeriod;
       optInSlowPeriod = optInFastPeriod;
       optInFastPeriod = tempInteger;
   }

   /* Catch special case for fix 26/12 MACD. */
   if( optInSlowPeriod != 0 )
      k1 = PER_TO_K(optInSlowPeriod);
   else
   {
      optInSlowPeriod = 26;
      k1 = (double)0.075; /* Fix 26 */
   }

   if( optInFastPeriod != 0 )
      k2 = PER_TO_K(optInFastPeriod);
   else
   {
      optInFastPeriod = 12;
      k2 = (double)0.15; /* Fix 12 */
   }

   lookbackSignal = TA_EMA_Lookback( optInSignalPeriod_2 ); 

   /* Move up the start index if there is not
    * enough initial data.
    */
   lookbackTotal =  lookbackSignal;
   lookbackTotal += TA_EMA_Lookback( optInSlowPeriod );

   if( startIdx < lookbackTotal )
      startIdx = lookbackTotal;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
   {
      *outBegIdx = 0;
      *outNBElement = 0;
      return TA_SUCCESS;
   }

   /* Allocate intermediate buffer for fast/slow EMA. */
   tempInteger = (endIdx-startIdx)+1+lookbackSignal;
   ARRAY_ALLOC( fastEMABuffer, tempInteger );
   #if !defined( _JAVA )
      if( !fastEMABuffer )
      {
         *outBegIdx = 0;
         *outNBElement = 0;
         return TA_ALLOC_ERR;
      }
   #endif

   ARRAY_ALLOC( slowEMABuffer, tempInteger );
   #if !defined( _JAVA )
      if( !slowEMABuffer )
      {
         *outBegIdx = 0;
         *outNBElement = 0;
         ARRAY_FREE( fastEMABuffer );
         return TA_ALLOC_ERR;
      }
   #endif

   /* Calculate the slow EMA. 
    *
    * Move back the startIdx to get enough data
    * for the signal period. That way, once the
    * signal calculation is done, all the output
    * will start at the requested 'startIdx'.
    */
   tempInteger = startIdx-lookbackSignal;
   retCode = TA_INT_EMA( tempInteger, endIdx,
                                     inReal, optInSlowPeriod, k1,
                                     &outBegIdx1, &outNbElement1, slowEMABuffer );

   if( retCode != TA_SUCCESS )
   {
      *outBegIdx = 0;
      *outNBElement = 0;
      ARRAY_FREE( fastEMABuffer );
      ARRAY_FREE( slowEMABuffer );
      return retCode;
   }

   /* Calculate the fast EMA. */
   retCode = TA_INT_EMA( tempInteger, endIdx,
                                     inReal, optInFastPeriod, k2,
                                     &outBegIdx2, &outNbElement2, fastEMABuffer );

   if( retCode != TA_SUCCESS )
   {
      *outBegIdx = 0;
      *outNBElement = 0;
      ARRAY_FREE( fastEMABuffer );
      ARRAY_FREE( slowEMABuffer );
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
      ARRAY_FREE( fastEMABuffer );
      ARRAY_FREE( slowEMABuffer );
      return TA_INTERNAL_ERROR(119);
   }

   /* Calculate (fast EMA) - (slow EMA). */
   for( i=0; i < outNbElement1; i++ )
      fastEMABuffer[i] = fastEMABuffer[i] - slowEMABuffer[i];


   /* Copy the result into the output for the caller. */
   ARRAY_MEMMOVE( outMACD, 0, fastEMABuffer, lookbackSignal, (endIdx-startIdx)+1 );

   /* Calculate the signal/trigger line. */
   retCode = TA_INT_EMA( 0, outNbElement1-1,
                                            fastEMABuffer, optInSignalPeriod_2, PER_TO_K(optInSignalPeriod_2), 
                                            &outBegIdx2, &outNbElement2, outMACDSignal );


   ARRAY_FREE( fastEMABuffer );
   ARRAY_FREE( slowEMABuffer );

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
