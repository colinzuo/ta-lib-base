#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_SmaExt_Lookback(int           optInTimePeriod,
    int           optInTimePeriodShift)
{
    /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInTimePeriod. */
    if ((optInTimePeriod < 1) || (optInTimePeriod > 100000))
        return -1;

    if ((optInTimePeriodShift < 0) || (optInTimePeriod > 100000))
        return -1;
#endif /* TA_FUNC_NO_RANGE_CHECK */

    /* insert lookback code here. */

    return optInTimePeriod - 1 + optInTimePeriodShift;
}

/*
 * TA_SmaExt - Simple Moving Average Ext
 *
 * Input  = double
 * Output = double
 *
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 *
 * optInTimePeriodShift:(From 0 to 100000)
 *    Shift of period
 *
 */
TA_RetCode TA_SmaExt(int    startIdx,
    int    endIdx,
    const double inReal[],
    int           optInTimePeriod,
    int           optInTimePeriodShift,
    int* outBegIdx,
    int* outNBElement,
    double        outReal[])
{
    /* Insert local variables here. */

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
    if (startIdx < 0)
        return TA_OUT_OF_RANGE_START_INDEX;

    if ((endIdx < 0) || (endIdx < startIdx))
        return TA_OUT_OF_RANGE_END_INDEX;

    if (!inReal) return TA_BAD_PARAM;

    /* min/max are checked for optInTimePeriod. */
    if ((optInTimePeriod < 1) || (optInTimePeriod > 100000))
        return TA_BAD_PARAM;

    if ((optInTimePeriodShift < 0) || (optInTimePeriod > 100000))
        return -1;

    if (!outReal)
        return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

    if (optInTimePeriod == 1) {
        *outBegIdx = startIdx + optInTimePeriodShift;
        *outNBElement = endIdx - startIdx + 1 - optInTimePeriodShift;

        ARRAY_COPY(outReal, inReal + startIdx, *outNBElement);

        return TA_SUCCESS;
    }

    double periodTotal, tempReal;
    int i, outIdx, trailingIdx, lookbackTotal, shiftedEndIdx;

    /* Identify the minimum number of price bar needed
    * to calculate at least one output.
    */
    lookbackTotal = optInTimePeriod - 1 + optInTimePeriodShift;

    /* Move up the start index if there is not
    * enough initial data.
    */
    if (startIdx < lookbackTotal)
        startIdx = lookbackTotal;

    /* Make sure there is still something to evaluate. */
    if (startIdx > endIdx)
    {
        *outBegIdx = 0;
        *outNBElement = 0;
        return TA_SUCCESS;
    }

    /* Do the MA calculation using tight loops. */
    /* Add-up the initial period, except for the last value. */
    periodTotal = 0;
    trailingIdx = startIdx - lookbackTotal;

    i = trailingIdx;
    while (i < (startIdx - optInTimePeriodShift))
        periodTotal += inReal[i++];

    outIdx = 0;
    shiftedEndIdx = endIdx - optInTimePeriodShift;
    do
    {
        periodTotal += inReal[i++];
        tempReal = periodTotal;
        periodTotal -= inReal[trailingIdx++];
        outReal[outIdx++] = tempReal / optInTimePeriod;
    } while (i <= shiftedEndIdx);

    /* All done. Indicate the output limits and return. */
    *outNBElement = outIdx;
    *outBegIdx = startIdx;

    return TA_SUCCESS;
}
