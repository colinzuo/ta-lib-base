#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_MACDFIX_Lookback( int           optInSignalPeriod )  /* From 1 to 100000 */
{
   /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
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
    *  by the fix 26 period EMA).
    */
   return   TA_EMA_Lookback( 26 )
          + TA_EMA_Lookback( optInSignalPeriod );
}

/*
 * TA_MACDFIX - Moving Average Convergence/Divergence Fix 12/26
 * 
 * Input  = double
 * Output = double, double, double
 * 
 * Optional Parameters
 * -------------------
 * optInSignalPeriod:(From 1 to 100000)
 *    Smoothing for the signal line (nb of period)
 * 
 * 
 */
TA_RetCode TA_MACDFIX( int    startIdx,
                       int    endIdx,
                       const double inReal[],
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

   return TA_PREFIX(INT_MACD)( startIdx, endIdx, inReal,
                                   0, /* 0 indicate fix 12 == 0.15  for optInFastPeriod */
                                   0, /* 0 indicate fix 26 == 0.075 for optInSlowPeriod */
                                   optInSignalPeriod,
                                   outBegIdx,
                                   outNBElement,
                                   outMACD,
                                   outMACDSignal,
                                   outMACDHist );
}
