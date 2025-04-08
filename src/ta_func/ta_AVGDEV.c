
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
 *  090812 AB     Initial Version
 */

#include <string.h>
#include <math.h>

#include "ta_func.h"
#include "ta_utility.h"
#include "ta_memory.h"

int TA_AVGDEV_Lookback( int           optInTimePeriod )  /* From 2 to 100000 */
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

   return optInTimePeriod - 1;
}

/*
 * TA_AVGDEV - Average Deviation
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
TA_RetCode TA_AVGDEV(	int    startIdx,
                    	int    endIdx,
						const double inReal[],
						int           optInTimePeriod, /* From 2 to 100000 */
						int          *outBegIdx,
						int          *outNBElement,
						double        outReal[] )
{
   /* insert local variable here */
	int today, outIdx, lookback;

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   if( !inReal ) return TA_BAD_PARAM;
   /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 14;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return TA_BAD_PARAM;

   if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */


	lookback = optInTimePeriod - 1;

	if (startIdx < lookback) {
		startIdx = lookback;
	}
	today = startIdx;

	/* Make sure there is still something to evaluate. */
	if( today > endIdx ) {
	    (*outBegIdx) = 0;
		(*outNBElement) = 0;
		return TA_SUCCESS;
	}

	/* Process the initial DM and TR */
	(*outBegIdx) = today;

	outIdx = 0;

	while (today <= endIdx) {
	   	double todaySum, todayDev;
		int i;

		todaySum = 0.0;
		for (i = 0; i < optInTimePeriod; i++) {
			todaySum += inReal[today-i];
		}

		todayDev = 0.0;
		for (i = 0; i < optInTimePeriod; i++) {
			todayDev += std_fabs(inReal[today-i] - todaySum/optInTimePeriod);
		}
		outReal[outIdx] = todayDev/optInTimePeriod;

		outIdx++;
		today++;
	}

	(*outNBElement) = outIdx;

	return TA_SUCCESS;
}
