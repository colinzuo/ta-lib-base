#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_SMA_Lookback(int           optInTimePeriod)  /* From 2 to 100000 */
{
    /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInTimePeriod. */
    if ((optInTimePeriod < 2) || (optInTimePeriod > 100000))
        return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

    /* insert lookback code here. */

    return optInTimePeriod - 1;
}

/*
 * TA_SMA - Simple Moving Average
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
TA_RetCode TA_SMA(int    startIdx,
    int    endIdx,
    const double inReal[],
    int           optInTimePeriod, /* From 2 to 100000 */
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
    if ((optInTimePeriod < 2) || (optInTimePeriod > 100000))
        return TA_BAD_PARAM;

    if (!outReal)
        return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

    return TA_INT_SMA(startIdx, endIdx, inReal, optInTimePeriod,
        outBegIdx, outNBElement, outReal);
}

TA_RetCode TA_INT_SMA(int    startIdx,
    int    endIdx,
    const double* inReal,
    int      optInTimePeriod, /* From 1 to TA_INTEGER_MAX */
    int* outBegIdx,
    int* outNBElement,
    double* outReal)
{
    double periodTotal, tempReal;
    int i, outIdx, trailingIdx, lookbackTotal;

    /* Identify the minimum number of price bar needed
    * to calculate at least one output.
    */
    lookbackTotal = optInTimePeriod - 1;

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
    while (i < startIdx)
        periodTotal += inReal[i++];

    /* Proceed with the calculation for the requested range.
     * Note that this algorithm allows the inReal and
     * outReal to be the same buffer.
     */
    outIdx = 0;
    do
    {
        periodTotal += inReal[i++];
        tempReal = periodTotal;
        periodTotal -= inReal[trailingIdx++];
        outReal[outIdx++] = tempReal / optInTimePeriod;
    } while (i <= endIdx);

    /* All done. Indicate the output limits and return. */
    *outNBElement = outIdx;
    *outBegIdx = startIdx;

    return TA_SUCCESS;
}
