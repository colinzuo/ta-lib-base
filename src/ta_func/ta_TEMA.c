#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_TEMA_Lookback( int           optInTimePeriod )  /* From 2 to 100000 */
{
   /* insert local variable here */
    int retValue;

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 30;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Get lookack for one EMA. */
   retValue = TA_EMA_Lookback( optInTimePeriod );

   return retValue * 3;
}

/*
 * TA_TEMA - Triple Exponential Moving Average
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
TA_RetCode TA_TEMA( int    startIdx,
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
   int thirdEMABegIdx;
   int thirdEMANbElement;

   int tempInt, outIdx, lookbackTotal, lookbackEMA;
   int firstEMAIdx, secondEMAIdx;

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

   /* For an explanation of this function, please read:
    * 
    * Stocks & Commodities V. 12:1 (11-19): 
    *   Smoothing Data With Faster Moving Averages
    * Stocks & Commodities V. 12:2 (72-80): 
    *   Smoothing Data With Less Lag
    *
    * Both magazine articles written by Patrick G. Mulloy
    *
    * Essentially, a TEMA of time serie 't' is:
    *   EMA1 = EMA(t,period)
    *   EMA2 = EMA(EMA(t,period),period)
    *   EMA3 = EMA(EMA(EMA(t,period),period))
    *   TEMA = 3*EMA1 - 3*EMA2 + EMA3
    *
    * TEMA offers a moving average with less lags then the
    * traditional EMA.
    *
    * Do not confuse a TEMA with EMA3. Both are called "Triple EMA"
    * in the litterature.
    *
    * DEMA is very similar (and from the same author).
    */

   /* Will change only on success. */
   *outNBElement = 0;
   *outBegIdx = 0;

   /* Adjust startIdx to account for the lookback period. */
   lookbackEMA = TA_EMA_Lookback( optInTimePeriod );
   lookbackTotal = lookbackEMA * 3;

   if( startIdx < lookbackTotal )
      startIdx = lookbackTotal;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
      return TA_SUCCESS; 

   /* Allocate a temporary buffer for the firstEMA. */
   tempInt = lookbackTotal+(endIdx-startIdx)+1;
   ARRAY_ALLOC(firstEMA,tempInt);
   #if !defined( _JAVA )
      if( !firstEMA )
         return TA_ALLOC_ERR;
   #endif

   /* Calculate the first EMA */   
   k = PER_TO_K(optInTimePeriod);
   retCode = TA_PREFIX(INT_EMA)( startIdx-(lookbackEMA*2), endIdx, inReal,
                                     optInTimePeriod, k,
                                     &firstEMABegIdx, &firstEMANbElement,
								     firstEMA );
   
   /* Verify for failure or if not enough data after
    * calculating the first EMA.
    */
   if( (retCode != TA_SUCCESS ) || (firstEMANbElement == 0) )
   {
      ARRAY_FREE( firstEMA );
      return retCode;
   }

   /* Allocate a temporary buffer for storing the EMA2 */
   ARRAY_ALLOC(secondEMA,firstEMANbElement);
   #if !defined( _JAVA )
      if( !secondEMA )
      {
         ARRAY_FREE( firstEMA );
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
   if( (retCode != TA_SUCCESS ) || (secondEMANbElement == 0) )      
   {
      ARRAY_FREE( firstEMA );
      ARRAY_FREE( secondEMA );
      return retCode;
   }

   /* Calculate the EMA3 into the caller provided output. */
   retCode = TA_INT_EMA( 0, secondEMANbElement-1, secondEMA,
                                            optInTimePeriod, k,
                                            &thirdEMABegIdx, &thirdEMANbElement,
                                            outReal );

   /* Return empty output on failure or if not enough data after
    * calculating the third EMA.
    */
   if( (retCode != TA_SUCCESS ) || (thirdEMANbElement == 0) )
   {
      ARRAY_FREE( firstEMA );
      ARRAY_FREE( secondEMA );
      return retCode;
   }

   /* Indicate where the output starts relative to
    * the caller input.
    */
   firstEMAIdx  = thirdEMABegIdx + secondEMABegIdx;
   secondEMAIdx = thirdEMABegIdx;
   *outBegIdx = firstEMAIdx + firstEMABegIdx;

   /* Do the TEMA:
    *  Iterate through the EMA3 (output buffer) and adjust
    *  the value by using the EMA2 and EMA1.
    */
   outIdx = 0;
   while( outIdx < thirdEMANbElement ) 
   {
      outReal[outIdx] += (3.0*firstEMA[firstEMAIdx++]) - (3.0*secondEMA[secondEMAIdx++]);
      outIdx++;
   }

   ARRAY_FREE( firstEMA );
   ARRAY_FREE( secondEMA );

   /* Indicates to the caller the number of output
    * successfully calculated.
    */
   *outNBElement = outIdx;

   return TA_SUCCESS;
}
