#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_ATR_Lookback( int           optInTimePeriod )  /* From 1 to 100000 */
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
   return optInTimePeriod + TA_GLOBALS_UNSTABLE_PERIOD(TA_FUNC_UNST_ATR,Atr);
}

/*
 * TA_ATR - Average True Range
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
TA_RetCode TA_ATR( int    startIdx,
                   int    endIdx,
                   const double inHigh[],
                   const double inLow[],
                   const double inClose[],
                   int           optInTimePeriod, /* From 1 to 100000 */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] )
{
   /* Insert local variables here. */
   TA_RetCode retCode;
   int outIdx, today, lookbackTotal;
   int nbATR;
   int outBegIdx1;
   int outNbElement1;

   double prevATR;
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

   /* Average True Range is the greatest of the following: 
    *
    *  val1 = distance from today's high to today's low.
    *  val2 = distance from yesterday's close to today's high.
    *  val3 = distance from yesterday's close to today's low.   
    *
    * These value are averaged for the specified period using
    * Wilder method. This method have an unstable period comparable
    * to and Exponential Moving Average (EMA).
    */
   *outBegIdx = 0;
   *outNBElement = 0;

   /* Adjust startIdx to account for the lookback period. */
   lookbackTotal = TA_ATR_Lookback( optInTimePeriod );

   if( startIdx < lookbackTotal )
      startIdx = lookbackTotal;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
      return TA_SUCCESS;

   /* Trap the case where no smoothing is needed. */
   if( optInTimePeriod <= 1 )
   {
      /* No smoothing needed. Just do a TRANGE. */
      return TA_PREFIX(TRANGE)( startIdx, endIdx,
                                    inHigh, inLow, inClose,
                                    outBegIdx, outNBElement, outReal );
   }

   /* Allocate an intermediate buffer for TRANGE. */
   ARRAY_ALLOC(tempBuffer, lookbackTotal+(endIdx-startIdx)+1 );

   /* Do TRANGE in the intermediate buffer. */
   retCode = TA_PREFIX(TRANGE)( (startIdx-lookbackTotal+1), endIdx,
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
   outIdx = TA_GLOBALS_UNSTABLE_PERIOD(TA_FUNC_UNST_ATR,Atr);
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
   outReal[0] = prevATR;

   /* Now do the number of requested ATR. */
   nbATR = (endIdx - startIdx)+1;

   while( --nbATR != 0 )
   {
      prevATR *= optInTimePeriod - 1;
      prevATR += tempBuffer[today++];
      prevATR /= optInTimePeriod;
      outReal[outIdx++] = prevATR;
   }

   *outBegIdx    = startIdx;
   *outNBElement = outIdx;
   
   ARRAY_FREE( tempBuffer );
    
   return retCode;
}
