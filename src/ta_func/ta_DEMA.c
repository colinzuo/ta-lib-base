#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_DEMA_Lookback( int           optInTimePeriod )  /* From 2 to 100000 */
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

   /* Get lookback for one EMA.
    * Multiply by two (because double smoothing).
    */
   return TA_EMA_Lookback( optInTimePeriod ) * 2;
}

/*
 * TA_DEMA - Double Exponential Moving Average
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
TA_RetCode TA_DEMA( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int           optInTimePeriod, /* From 2 to 100000 */
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] )
{
   /* Insert local variables here. */
   ARRAY_REF(firstEMA);
   ARRAY_REF(secondEMA);
   double k;
   int firstEMABegIdx;
   int firstEMANbElement;
   int secondEMABegIdx;
   int secondEMANbElement;
   int tempInt, outIdx, firstEMAIdx, lookbackTotal, lookbackEMA;
   TA_RetCode retCode;

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

   /* For an explanation of this function, please read
    * 
    * Stocks & Commodities V. 12:1 (11-19): 
    *   Smoothing Data With Faster Moving Averages
    * Stocks & Commodities V. 12:2 (72-80): 
    *   Smoothing Data With Less Lag
    *
    * Both magazine articles written by Patrick G. Mulloy
    *
    * Essentially, a DEMA of time serie 't' is:
    *   EMA2 = EMA(EMA(t,period),period)
    *   DEMA = 2*EMA(t,period)- EMA2
    *
    * DEMA offers a moving average with less lags then the
    * traditional EMA.
    *
    * Do not confuse a DEMA with the EMA2. Both are called 
    * "Double EMA" in the litterature, but EMA2 is a simple
    * EMA of an EMA, while DEMA is a compostie of a single
    * EMA with EMA2.
    *
    * TEMA is very similar (and from the same author).
    */

   /* Will change only on success. */
   *outNBElement = 0;
   *outBegIdx = 0;

   /* Adjust startIdx to account for the lookback period. */
   lookbackEMA = TA_EMA_Lookback( optInTimePeriod );
   lookbackTotal = lookbackEMA * 2;

   if( startIdx < lookbackTotal )
      startIdx = lookbackTotal;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
      return TA_SUCCESS; 

   /* Allocate a temporary buffer for the firstEMA.
    *
    * When possible, re-use the outputBuffer for temp
    * calculation.
    */
   #if defined(USE_SINGLE_PRECISION_INPUT) || defined( USE_SUBARRAY )
      tempInt = lookbackTotal+(endIdx-startIdx)+1;
      ARRAY_ALLOC(firstEMA, tempInt );
      #if !defined( _JAVA )
         if( !firstEMA )
            return TA_ALLOC_ERR;
      #endif
   #else
      if( inReal == outReal )
         firstEMA = outReal;
      else
      {
         tempInt = lookbackTotal+(endIdx-startIdx)+1;
         ARRAY_ALLOC(firstEMA, tempInt );
         #if !defined( _JAVA )
            if( !firstEMA )
               return TA_ALLOC_ERR;
         #endif
      }
   #endif

   /* Calculate the first EMA */   
   k = PER_TO_K(optInTimePeriod);
   retCode = TA_INT_EMA( startIdx-lookbackEMA, endIdx, inReal,
                                     optInTimePeriod, k,
                                     &firstEMABegIdx, &firstEMANbElement,
								     firstEMA );
   
   /* Verify for failure or if not enough data after
    * calculating the first EMA.
    */
   if( (retCode != TA_SUCCESS) || (firstEMANbElement == 0) )
   {
      ARRAY_FREE_COND( firstEMA != outReal, firstEMA );
      return retCode;
   }

   /* Allocate a temporary buffer for storing the EMA of the EMA. */
   ARRAY_ALLOC(secondEMA, firstEMANbElement);

   #if !defined( _JAVA )
      if( !secondEMA )
      {
         ARRAY_FREE_COND( firstEMA != outReal, firstEMA );
         return TA_ALLOC_ERR;
      }
   #endif

   retCode = TA_INT_EMA( 0, firstEMANbElement-1, firstEMA,
                                            optInTimePeriod, k,
                                            &secondEMABegIdx, &secondEMANbElement,
						                    secondEMA );

   /* Return empty output on failure or if not enough data after
    * calculating the second EMA.
    */
   if( (retCode != TA_SUCCESS) || (secondEMANbElement == 0) )      
   {
      #if defined(USE_SINGLE_PRECISION_INPUT)
         ARRAY_FREE( firstEMA );
      #else
         ARRAY_FREE_COND( firstEMA != outReal, firstEMA );
      #endif
      ARRAY_FREE( secondEMA );
      return retCode;
   }

   /* Iterate through the second EMA and write the DEMA into
    * the output.
    */
   firstEMAIdx = secondEMABegIdx;
   outIdx = 0;
   while( outIdx < secondEMANbElement ) 
   {
      outReal[outIdx] = (2.0*firstEMA[firstEMAIdx++]) - secondEMA[outIdx];
      outIdx++;
   }

   #if defined(USE_SINGLE_PRECISION_INPUT)
      ARRAY_FREE( firstEMA );
   #else
      ARRAY_FREE_COND( firstEMA != outReal, firstEMA );
   #endif
   ARRAY_FREE( secondEMA );

   /* Succeed. Indicate where the output starts relative to
    * the caller input.
    */
   *outBegIdx    = firstEMABegIdx + secondEMABegIdx;
   *outNBElement = outIdx;

   return TA_SUCCESS;
}
