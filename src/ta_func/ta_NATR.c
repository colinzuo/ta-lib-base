#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_NATR_Lookback( int           optInTimePeriod )  /* From 1 to 100000 */
{
   /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 14;
   else if( ((int)optInTimePeriod < 1) || ((int)optInTimePeriod > 100000) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */

   /* The ATR lookback is the sum of:
    *    1 + (optInTimePeriod - 1)
    *
    * Where 1 is for the True Range, and
    * (optInTimePeriod-1) is for the simple
    * moving average.
    */
   return optInTimePeriod + TA_GLOBALS_UNSTABLE_PERIOD(TA_FUNC_UNST_NATR,Natr);
}

/*
 * TA_NATR - Normalized Average True Range
 * 
 * Input  = High, Low, Close
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_NATR( int    startIdx,
                    int    endIdx,
                    const double inHigh[],
                    const double inLow[],
                    const double inClose[],
                    int           optInTimePeriod, /* From 1 to 100000 */
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] )
{
   /* insert local variable here */
   TA_RetCode retCode;
   int outIdx, today, lookbackTotal;
   int nbATR;
   int outBegIdx1;
   int outNbElement1;

   double prevATR, tempValue;
   ARRAY_REF( tempBuffer );
   ARRAY_LOCAL(prevATRTemp,1);

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
   else if( ((int)optInTimePeriod < 1) || ((int)optInTimePeriod > 100000) )
      return TA_BAD_PARAM;

   if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* This function is very similar as ATR, except
    * it is being normalized as follow:
    *
    *    NATR = (ATR(period) / Close) * 100
    *
    *
    * Normalization make the ATR function more relevant
    * in the folllowing scenario:
    *    - Long term analysis where the price changes drastically.
    *    - Cross-market or cross-security ATR comparison.
    *
    * More Info: 
    *      Technical Analysis of Stock & Commodities (TASC)
    *      May 2006 by John Forman
    */

   /* Average True Range is the greatest of the following: 
    *
    *  val1 = distance from today's high to today's low.
    *  val2 = distance from yesterday's close to today's high.
    *  val3 = distance from yesterday's close to today's low.   
    *
    * These value are averaged for the specified period using
    * Wilder method. This method have an unstable period comparable
    * to an Exponential Moving Average (EMA).
    */
   *outBegIdx = 0;
   *outNBElement = 0;

   /* Adjust startIdx to account for the lookback period. */
   lookbackTotal = TA_NATR_Lookback( optInTimePeriod );

   if( startIdx < lookbackTotal )
      startIdx = lookbackTotal;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
      return TA_SUCCESS;

   /* Trap the case where no smoothing is needed. */
   if( optInTimePeriod <= 1 )
   {
      /* No smoothing needed. Just do a TRANGE. */
      return TA_TRANGE( startIdx, endIdx,
                                    inHigh, inLow, inClose,
                                    outBegIdx, outNBElement, outReal );
   }

   /* Allocate an intermediate buffer for TRANGE. */
   ARRAY_ALLOC(tempBuffer, lookbackTotal+(endIdx-startIdx)+1 );

   /* Do TRANGE in the intermediate buffer. */
   retCode = TA_TRANGE( (startIdx-lookbackTotal+1), endIdx,
                                    inHigh, inLow, inClose,
                                    &outBegIdx1, &outNbElement1,
								    tempBuffer );

   if( retCode != TA_SUCCESS )
   {
      ARRAY_FREE( tempBuffer );
      return retCode;
   }

   /* First value of the ATR is a simple Average of
    * the TRANGE output for the specified period.
    */
   retCode = TA_INT_SMA( optInTimePeriod-1,
                                            optInTimePeriod-1,
                                            tempBuffer, optInTimePeriod,
                                            &outBegIdx1, &outNbElement1,
						                    prevATRTemp );

   if( retCode != TA_SUCCESS )
   {
      ARRAY_FREE( tempBuffer );
      return retCode;    
   }
   prevATR = prevATRTemp[0];

   /* Subsequent value are smoothed using the
    * previous ATR value (Wilder's approach).
    *  1) Multiply the previous ATR by 'period-1'. 
    *  2) Add today TR value. 
    *  3) Divide by 'period'.
    */
   today = optInTimePeriod;
   outIdx = TA_GLOBALS_UNSTABLE_PERIOD(TA_FUNC_UNST_NATR,Natr);
   /* Skip the unstable period. */
   while( outIdx != 0 )
   {      
      prevATR *= optInTimePeriod - 1;
      prevATR += tempBuffer[today++];
      prevATR /= optInTimePeriod;
      outIdx--;
   }

   /* Now start to write the final ATR in the caller 
    * provided outReal.
    */
   outIdx = 1;
   tempValue = inClose[today];
   if( !TA_IS_ZERO(tempValue) )
      outReal[0] = (prevATR/tempValue)*100.0;
   else
      outReal[0] = 0.0;

   /* Now do the number of requested ATR. */
   nbATR = (endIdx - startIdx)+1;
   
   while( --nbATR != 0 )
   {
      prevATR *= optInTimePeriod - 1;
      prevATR += tempBuffer[today++];
      prevATR /= optInTimePeriod;
      tempValue = inClose[today];
      if( !TA_IS_ZERO(tempValue) )
         outReal[outIdx] = (prevATR/tempValue)*100.0;
      else
         outReal[0] = 0.0;
      outIdx++;
   }

   *outBegIdx    = startIdx;
   *outNBElement = outIdx;
   
   ARRAY_FREE( tempBuffer );
    
   return retCode;
}
