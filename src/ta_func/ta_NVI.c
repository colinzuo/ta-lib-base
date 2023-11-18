#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_NVI_Lookback( void )
{
   return 0;
}

/*
 * TA_NVI - Negative Volume Index
 * 
 * Input  = Close, Volume
 * Output = double
 * 
 */
TA_RetCode TA_NVI( int    startIdx,
                   int    endIdx,
                   const double inClose[],
                   const int    inVolume[],
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] )
{
	/* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   /* Validate the parameters. */
   /* Verify required price component. */
   if(!inClose||!inVolume)
      return TA_BAD_PARAM;

   if( outReal == NULL )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* Default return values */
   *outBegIdx    = 0;
   *outNBElement = 0;

   return TA_SUCCESS;
}
