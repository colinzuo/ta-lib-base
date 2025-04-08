
/* List of contributors:
 *
 *  Initial  Name/description
 *  -------------------------------------------------------------------
 *  AB       Anatoliy Belsky
 *
 * Change history:
 *
 *  MMDDYY BY     Description
 *  -------------------------------------------------------------------
 *  181012 AB    Initial Version
 */

#include <string.h>
#include <math.h>

#include "ta_func.h"
#include "ta_utility.h"
#include "ta_memory.h"

int TA_IMI_Lookback( int           optInTimePeriod )  /* From 2 to 100000 */
{
   /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 14;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */

   return optInTimePeriod + TA_GLOBALS_UNSTABLE_PERIOD(TA_FUNC_UNST_IMI, Imi) - 1;
}

/*
 * TA_IMI - Intraday Momentum Index
 * 
 * Input  = Open, Close
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_IMI(	int    startIdx,
                    int    endIdx,
					const double inOpen[],
					const double inClose[],
					int           optInTimePeriod, /* From 2 to 100000 */
					int          *outBegIdx,
					int          *outNBElement,
					double        outReal[] )
{
   /* insert local variable here */
	int lookback, outIdx = 0;

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   /* Verify required price component. */
   if(!inOpen||!inClose)
      return TA_BAD_PARAM;

   /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 14;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return TA_BAD_PARAM;

   if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   lookback = TA_IMI_Lookback( optInTimePeriod );

   if(startIdx < lookback)
      startIdx = lookback;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx ) {
		(*outBegIdx) = 0;
		(*outNBElement) = 0;
		return TA_SUCCESS;
   }

   (*outBegIdx) = startIdx;

   while (startIdx <= endIdx) {
		double upsum = .0, downsum = .0;
		int i;

		for (i = startIdx - lookback; i <= startIdx; i++) {
			double close = inClose[i];
			double open = inOpen[i];

			if (close > open) {
				upsum += (close - open);
			} else {
				downsum += (open - close);
			}

			outReal[outIdx] = 100.0*(upsum/(upsum + downsum));
		}

		startIdx++;
		outIdx++;
   }

   (*outNBElement) = outIdx;

   return TA_SUCCESS;
}
