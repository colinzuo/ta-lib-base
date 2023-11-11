#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_EMA_Lookback( int           optInTimePeriod )  /* From 2 to 100000 */
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
   return optInTimePeriod - 1 + TA_GLOBALS_UNSTABLE_PERIOD(TA_FUNC_UNST_EMA,Ema);
}

/*
 * TA_EMA - Exponential Moving Average
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
TA_RetCode TA_EMA( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInTimePeriod, /* From 2 to 100000 */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] )
{
   /* Insert local variables here. */

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

   /* Simply call the internal implementation of the EMA. */
   return TA_PREFIX(INT_EMA)( startIdx, endIdx, inReal, 
                                  optInTimePeriod,
                                  PER_TO_K( optInTimePeriod ),
                                  outBegIdx, outNBElement, outReal );
}

/* Internal implementation can be called from any other TA function.
 *
 * Faster because there is no parameter check, but it is a double
 * edge sword.
 *
 * The optInK_1 and optInTimePeriod are usually tightly coupled:
 *
 *    optInK_1  = 2 / (optInTimePeriod + 1).
 *
 * These values are going to be related by this equation 99.9% of the
 * time... but there is some exception, this is why both must be provided.
 *
 * The macro PER_TO_K is equivalent to the above formula.
 */
TA_RetCode TA_PREFIX(INT_EMA)( int               startIdx,
                               int               endIdx,
                               const INPUT_TYPE *inReal,
                               int               optInTimePeriod, /* From 1 to TA_INTEGER_MAX */
                               double            optInK_1,          /* Ratio for calculation of EMA. */
                               int              *outBegIdx,
                               int              *outNBElement,
                               double           *outReal )
{
   double tempReal, prevMA;
   int i, today, outIdx, lookbackTotal;

   /* Ususally, optInK_1 = 2 / (optInTimePeriod + 1),
    * but sometime there is exception. This
    * is why both value are parameters.
    */

   /* Identify the minimum number of price bar needed
    * to calculate at least one output.
    */
   lookbackTotal = TA_EMA_Lookback( optInTimePeriod );

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
   *outBegIdx = startIdx;

   /* Do the EMA calculation using tight loops. */
   
   /* The first EMA is calculated differently. It
    * then become the seed for subsequent EMA.
    *
    * The algorithm for this seed vary widely.
    * Only 3 are implemented here:
    *
    * TA_MA_CLASSIC:
    *    Use a simple MA of the first 'period'.
    *    This is the approach most widely documented.
    *
    * TA_MA_METASTOCK:
    *    Use first price bar value as a seed
    *    from the begining of all the available
    *    data.
    *
    * TA_MA_TRADESTATION:
    *    Use 4th price bar as a seed, except when
    *    period is 1 who use 2th price bar or something
    *    like that... (not an obvious one...).
    */
   if( TA_GLOBALS_COMPATIBILITY == TA_COMPATIBILITY_DEFAULT )
   {
      today = startIdx-lookbackTotal;
      i = optInTimePeriod;
      tempReal = 0.0;
      while( i-- > 0 )
         tempReal += inReal[today++];

      prevMA = tempReal / optInTimePeriod;
   }
   else
   {
      prevMA = inReal[0];
      today = 1;
   }

   /* At this point, prevMA is the first EMA (the seed for
    * the rest).
    * 'today' keep track of where the processing is within the
    * input.
    */

   /* Skip the unstable period. Do the processing 
    * but do not write it in the output.
    */   
   while( today <= startIdx )
      prevMA = ((inReal[today++]-prevMA)*optInK_1) + prevMA;

   /* Write the first value. */
   outReal[0] = prevMA;
   outIdx = 1;

   /* Calculate the remaining range. */
   while( today <= endIdx )
   {
      prevMA = ((inReal[today++]-prevMA)*optInK_1) + prevMA;
      outReal[outIdx++] = prevMA;
   }

   *outNBElement = outIdx;

   return TA_SUCCESS;
}
