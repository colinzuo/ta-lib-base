#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_TYPPRICE_Lookback( void )
{
   return 0;
}

/*
 * TA_TYPPRICE - Typical Price
 * 
 * Input  = High, Low, Close
 * Output = double
 * 
 */
TA_RetCode TA_TYPPRICE( int    startIdx,
                        int    endIdx,
                        const double inHigh[],
                        const double inLow[],
                        const double inClose[],
                        int          *outBegIdx,
                        int          *outNBElement,
                        double        outReal[] )
{
   /* Insert local variables here. */
   int outIdx, i;

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   /* Verify required price component. */
   if(!inHigh||!inLow||!inClose)
      return TA_BAD_PARAM;

      if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* Typical price = (High + Low + Close ) / 3 */
   outIdx    = 0;

   for( i= startIdx; i <= endIdx; i++ )
   {
      outReal[outIdx++] = ( inHigh [i] +
                              inLow  [i] +
                              inClose[i] ) / 3.0;
   }

   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
