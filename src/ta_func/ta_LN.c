#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_LN_Lookback( void )
{
   return 0;
}

/*
 * TA_LN - Vector Log Natural
 * 
 * Input  = double
 * Output = double
 * 
 */
TA_RetCode TA_LN( int    startIdx,
                  int    endIdx,
                  const double inReal[],
                  int          *outBegIdx,
                  int          *outNBElement,
                  double        outReal[] )
{
   /* insert local variable here */
   int outIdx;
   int i;

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   if( !inReal ) return TA_BAD_PARAM;
      if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* Default return values */
   for( i=startIdx, outIdx=0; i <= endIdx; i++, outIdx++ )
   {
      outReal[outIdx] = std_log(inReal[i]);
   }    

   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
