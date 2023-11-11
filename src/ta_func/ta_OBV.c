#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_OBV_Lookback( void )
{
   return 0;
}

/*
 * TA_OBV - On Balance Volume
 * 
 * Input  = double, Volume
 * Output = double
 * 
 */
TA_RetCode TA_OBV( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   const double inVolume[],
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] )
{
	/* insert local variable here */
   int i;
   int outIdx;
   double prevReal, tempReal, prevOBV;

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   if( !inReal ) return TA_BAD_PARAM;
   /* Verify required price component. */
   if(!inVolume)
      return TA_BAD_PARAM;

      if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */
   prevOBV  = inVolume[startIdx];
   prevReal = inReal[startIdx];
   outIdx = 0;

   for(i=startIdx; i <= endIdx; i++ )
   {
      tempReal = inReal[i];
      if( tempReal > prevReal )
         prevOBV += inVolume[i];
      else if( tempReal < prevReal )
         prevOBV -= inVolume[i];

      outReal[outIdx++] = prevOBV;
      prevReal = tempReal;
   }

   *outBegIdx = startIdx;
   *outNBElement = outIdx;

   return TA_SUCCESS;
}
