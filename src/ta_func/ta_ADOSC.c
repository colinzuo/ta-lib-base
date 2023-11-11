#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_ADOSC_Lookback( int           optInFastPeriod, /* From 2 to 100000 */
                     int           optInSlowPeriod )  /* From 2 to 100000 */
{
   /* insert local variable here */
   int slowestPeriod;

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInFastPeriod. */
   if( (int)optInFastPeriod == TA_INTEGER_DEFAULT )
      optInFastPeriod = 3;
   else if( ((int)optInFastPeriod < 2) || ((int)optInFastPeriod > 100000) )
      return -1;

   /* min/max are checked for optInSlowPeriod. */
   if( (int)optInSlowPeriod == TA_INTEGER_DEFAULT )
      optInSlowPeriod = 10;
   else if( ((int)optInSlowPeriod < 2) || ((int)optInSlowPeriod > 100000) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */

   /* Use the slowest EMA period to evaluate the total lookback. */
   if( optInFastPeriod < optInSlowPeriod )
      slowestPeriod = optInSlowPeriod;
   else
      slowestPeriod = optInFastPeriod;

   /* Adjust startIdx to account for the lookback period. */
   return TA_EMA_Lookback( slowestPeriod );
}

/*
 * TA_ADOSC - Chaikin A/D Oscillator
 * 
 * Input  = High, Low, Close, Volume
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
 * 
 */
TA_RetCode TA_ADOSC( int    startIdx,
                     int    endIdx,
                     const double inHigh[],
                     const double inLow[],
                     const double inClose[],
                     const double inVolume[],
                     int           optInFastPeriod, /* From 2 to 100000 */
                     int           optInSlowPeriod, /* From 2 to 100000 */
                     int          *outBegIdx,
                     int          *outNBElement,
                     double        outReal[] )
{
	/* insert local variable here */

   int today, outIdx, lookbackTotal;
   int slowestPeriod;
   double high, low, close, tmp;

   double slowEMA, slowk, one_minus_slowk;
   double fastEMA, fastk, one_minus_fastk;
   double ad;

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   /* Verify required price component. */
   if(!inHigh||!inLow||!inClose||!inVolume)
      return TA_BAD_PARAM;

      /* min/max are checked for optInFastPeriod. */
   if( (int)optInFastPeriod == TA_INTEGER_DEFAULT )
      optInFastPeriod = 3;
   else if( ((int)optInFastPeriod < 2) || ((int)optInFastPeriod > 100000) )
      return TA_BAD_PARAM;

   /* min/max are checked for optInSlowPeriod. */
   if( (int)optInSlowPeriod == TA_INTEGER_DEFAULT )
      optInSlowPeriod = 10;
   else if( ((int)optInSlowPeriod < 2) || ((int)optInSlowPeriod > 100000) )
      return TA_BAD_PARAM;

   if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* Implementation Note:
    *     The fastEMA varaible is not neceseraly the
    *     fastest EMA.
    *     In the same way, slowEMA is not neceseraly the
    *     slowest EMA.
    *
    *     The ADOSC is always the (fastEMA - slowEMA) regardless
    *     of the period specified. In other word:
    * 
    *     ADOSC(3,10) = EMA(3,AD) - EMA(10,AD)
    *
    *        while
    *
    *     ADOSC(10,3) = EMA(10,AD)- EMA(3,AD)
    *
    *     In the first case the EMA(3) is truly a faster EMA,
    *     while in the second case, the EMA(10) is still call
    *     fastEMA in the algorithm, even if it is in fact slower.
    *
    *     This gives more flexibility to the user if they want to
    *     experiment with unusual parameter settings.
    */

   /* Identify the slowest period. 
    * This infomration is used soleley to bootstrap
    * the algorithm (skip the lookback period).
    */
   if( optInFastPeriod < optInSlowPeriod )
      slowestPeriod = optInSlowPeriod;
   else
      slowestPeriod = optInFastPeriod;

   /* Adjust startIdx to account for the lookback period. */
   lookbackTotal = TA_EMA_Lookback( slowestPeriod );
   if( startIdx < lookbackTotal )
      startIdx = lookbackTotal;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
   {
      *outBegIdx = 0;
      *outNBElement = 0;
      return TA_SUCCESS;
   }

   *outBegIdx = startIdx;
   today  = startIdx-lookbackTotal;

   /* The following variables and macro are used to
    * calculate the "ad".
    */
   ad = 0.0;
   #define CALCULATE_AD \
   { \
      high  = inHigh[today]; \
      low   = inLow[today]; \
      tmp   = high-low; \
      close = inClose[today]; \
      if( tmp > 0.0 ) \
         ad += (((close-low)-(high-close))/tmp)*((double)inVolume[today]); \
      today++; \
   }

   /* Constants for EMA */
   fastk = PER_TO_K( optInFastPeriod );
   one_minus_fastk = 1.0 - fastk;

   slowk = PER_TO_K( optInSlowPeriod );
   one_minus_slowk = 1.0 - slowk;

   /* Initialize the two EMA
    *
    * Use the same range of initialization inputs for
    * both EMA and simply seed with the first A/D value.
    *
    * Note: Metastock do the same.
    */   
   CALCULATE_AD;
   fastEMA = ad;
   slowEMA = ad;

   /* Initialize the EMA and skip the unstable period. */
   while( today < startIdx )
   {
      CALCULATE_AD;
      fastEMA = (fastk*ad)+(one_minus_fastk*fastEMA);
      slowEMA = (slowk*ad)+(one_minus_slowk*slowEMA);
   }     
      
   /* Perform the calculation for the requested range */
   outIdx = 0;
   while( today <= endIdx )
   {
      CALCULATE_AD;
      fastEMA = (fastk*ad)+(one_minus_fastk*fastEMA);
      slowEMA = (slowk*ad)+(one_minus_slowk*slowEMA);

      outReal[outIdx++] = fastEMA - slowEMA;
   }
   *outNBElement = outIdx;

   return TA_SUCCESS;
}
