#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_AD_Lookback( void )
{
   return 0;
}

/*
 * TA_AD - Chaikin A/D Line
 * 
 * Input  = High, Low, Close, Volume
 * Output = double
 * 
 */
TA_RetCode TA_AD( int    startIdx,
                  int    endIdx,
                  const double inHigh[],
                  const double inLow[],
                  const double inClose[],
                  const double inVolume[],
                  int          *outBegIdx,
                  int          *outNBElement,
                  double        outReal[] )
{
   /* insert local variable here */
   int nbBar, currentBar, outIdx;

   double high, low, close, tmp;
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

      if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* Note: Results from this function might vary slightly
    *       from Metastock outputs. The reason being that
    *       Metastock use float instead of double and this
    *       cause a different floating-point precision to 
    *       be used.
    *
    *       For most function, this is not an apparent difference
    *       but for function using large cummulative values (like
    *       this AD function), minor imprecision adds up and becomes
    *       significative.
    *
    *       For better precision, TA-Lib use double in all its 
    *       its calculations.
    */

   /* Default return values */
   nbBar = endIdx-startIdx+1;
   *outNBElement = nbBar;
   *outBegIdx = startIdx;
   currentBar = startIdx;
   outIdx = 0;
   ad = 0.0;

   while( nbBar != 0 )
   {
      high  = inHigh[currentBar];
      low   = inLow[currentBar];
      tmp   = high-low;
      close = inClose[currentBar];

      if( tmp > 0.0 )
         ad += (((close-low)-(high-close))/tmp)*((double)inVolume[currentBar]);
      
      outReal[outIdx++] = ad;

      currentBar++;
      nbBar--;
   }

   return TA_SUCCESS;
}
