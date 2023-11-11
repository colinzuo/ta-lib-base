#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_T3_Lookback( int           optInTimePeriod, /* From 2 to 100000 */
                  double        optInVFactor )  /* From 0 to 1 */
{
   /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 5;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return -1;

   if( optInVFactor == TA_REAL_DEFAULT )
      optInVFactor = 7.000000e-1;
   else if( (optInVFactor < 0.000000e+0) || (optInVFactor > 1.000000e+0) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */
   (void)optInVFactor;
   return 6 * (optInTimePeriod-1) + TA_GLOBALS_UNSTABLE_PERIOD(TA_FUNC_UNST_T3,T3);
}

/*
 * TA_T3 - Triple Exponential Moving Average (T3)
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * optInVFactor:(From 0 to 1)
 *    Volume Factor
 * 
 * 
 */
TA_RetCode TA_T3( int    startIdx,
                  int    endIdx,
                  const double inReal[],
                  int           optInTimePeriod, /* From 2 to 100000 */
                  double        optInVFactor, /* From 0 to 1 */
                  int          *outBegIdx,
                  int          *outNBElement,
                  double        outReal[] )
{
	/* insert local variable here */

   int outIdx, lookbackTotal;
   int today, i;
   double k, one_minus_k;
   double e1, e2, e3, e4, e5, e6;
   double c1, c2, c3, c4;
   double tempReal;

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   if( !inReal ) return TA_BAD_PARAM;
      /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 5;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return TA_BAD_PARAM;

   if( optInVFactor == TA_REAL_DEFAULT )
      optInVFactor = 7.000000e-1;
   else if( (optInVFactor < 0.000000e+0) || (optInVFactor > 1.000000e+0) )
      return TA_BAD_PARAM;

   if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* For an explanation of this function, please read:
    *
    * Magazine articles written by Tim Tillson 
    *
    * Essentially, a T3 of time serie 't' is:
    *   EMA1(x,Period) = EMA(x,Period)
    *   EMA2(x,Period) = EMA(EMA1(x,Period),Period)
    *   GD(x,Period,vFactor) = (EMA1(x,Period)*(1+vFactor)) - (EMA2(x,Period)*vFactor)
    *   T3 = GD (GD ( GD(t, Period, vFactor), Period, vFactor), Period, vFactor);
    *
    * T3 offers a moving average with less lags then the
    * traditional EMA.
    *
    * Do not confuse a T3 with EMA3. Both are called "Triple EMA"
    * in the litterature.
    *
    */
   lookbackTotal = 6 * (optInTimePeriod - 1) + TA_GLOBALS_UNSTABLE_PERIOD(TA_FUNC_UNST_T3,T3);
   if( startIdx <= lookbackTotal )
      startIdx = lookbackTotal;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
   {
      *outNBElement = 0;
      *outBegIdx = 0;
      return TA_SUCCESS; 
   }

   *outBegIdx = startIdx;
   today = startIdx - lookbackTotal;

   k = 2.0/(optInTimePeriod+1.0);
   one_minus_k = 1.0-k;

   /* Initialize e1 */
   tempReal = inReal[today++];
   for( i=optInTimePeriod-1; i > 0 ; i-- ) 
      tempReal += inReal[today++];
   e1 = tempReal / optInTimePeriod;

   /* Initialize e2 */
   tempReal = e1;
   for( i=optInTimePeriod-1; i > 0 ; i-- ) 
   {
      e1 = (k*inReal[today++])+(one_minus_k*e1);
      tempReal += e1;
   }
   e2 = tempReal / optInTimePeriod;

   /* Initialize e3 */
   tempReal = e2;
   for( i=optInTimePeriod-1; i > 0 ; i-- ) 
   {
      e1  = (k*inReal[today++])+(one_minus_k*e1);
      e2  = (k*e1)+(one_minus_k*e2);
      tempReal += e2;
   }
   e3 = tempReal / optInTimePeriod;

   /* Initialize e4 */
   tempReal = e3;
   for( i=optInTimePeriod-1; i > 0 ; i-- ) 
   {
      e1  = (k*inReal[today++])+(one_minus_k*e1);
      e2  = (k*e1)+(one_minus_k*e2);
      e3  = (k*e2)+(one_minus_k*e3);
      tempReal += e3;
   }
   e4 = tempReal / optInTimePeriod;

   /* Initialize e5 */
   tempReal = e4;
   for( i=optInTimePeriod-1; i > 0 ; i-- ) 
   {
      e1  = (k*inReal[today++])+(one_minus_k*e1);
      e2  = (k*e1)+(one_minus_k*e2);
      e3  = (k*e2)+(one_minus_k*e3);
      e4  = (k*e3)+(one_minus_k*e4);
      tempReal += e4;
   }
   e5 = tempReal / optInTimePeriod;

   /* Initialize e6 */
   tempReal = e5;
   for( i=optInTimePeriod-1; i > 0 ; i-- ) 
   {
      e1  = (k*inReal[today++])+(one_minus_k*e1);
      e2  = (k*e1)+(one_minus_k*e2);
      e3  = (k*e2)+(one_minus_k*e3);
      e4  = (k*e3)+(one_minus_k*e4);
      e5  = (k*e4)+(one_minus_k*e5);
      tempReal += e5;
   }
   e6 = tempReal / optInTimePeriod;

   /* Skip the unstable period */
   while( today <= startIdx )
   {
      /* Do the calculation but do not write the output */
      e1  = (k*inReal[today++])+(one_minus_k*e1);
      e2  = (k*e1)+(one_minus_k*e2);
      e3  = (k*e2)+(one_minus_k*e3);
      e4  = (k*e3)+(one_minus_k*e4);
      e5  = (k*e4)+(one_minus_k*e5);
      e6  = (k*e5)+(one_minus_k*e6);
   }

   /* Calculate the constants */
   tempReal = optInVFactor * optInVFactor;
   c1 = -(tempReal * optInVFactor);
   c2 = 3.0 * (tempReal - c1);
   c3 = -6.0 * tempReal - 3.0 * (optInVFactor-c1);
   c4 = 1.0 + 3.0 * optInVFactor - c1 + 3.0 * tempReal;

   /* Write the first output */
   outIdx = 0;
  	outReal[outIdx++] = c1*e6+c2*e5+c3*e4+c4*e3;

   /* Calculate and output the remaining of the range. */
   while( today <= endIdx )
   {
      e1  = (k*inReal[today++])+(one_minus_k*e1);
      e2  = (k*e1)+(one_minus_k*e2);
      e3  = (k*e2)+(one_minus_k*e3);
      e4  = (k*e3)+(one_minus_k*e4);
      e5  = (k*e4)+(one_minus_k*e5);
      e6  = (k*e5)+(one_minus_k*e6);
      outReal[outIdx++] = c1*e6+c2*e5+c3*e4+c4*e3;
   }

   /* Indicates to the caller the number of output
    * successfully calculated.
    */
   *outNBElement = outIdx;

   return TA_SUCCESS;
}
