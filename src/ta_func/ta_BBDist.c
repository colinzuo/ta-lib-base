#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_BBDist_Lookback(int           optInTimePeriod, /* From 2 to 100000 */
    double        optInNbDev)
{
    /* insert local variable here */

    /* insert lookback code here. */
    (void)optInNbDev;

    /* The lookback is driven by the middle band moving average. */
    return TA_SMA_Lookback(optInTimePeriod);
}

/*
 * TA_BBDist - Bollinger Bands Dist
 *
 * Input  = double
 * Output = double
 *
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 *
 * optInNbDev:(From TA_REAL_MIN to TA_REAL_MAX)
 *    Deviation multiplier
 */
TA_RetCode TA_BBDist(int    startIdx,
    int    endIdx,
    const double inReal[],
    int           optInTimePeriod, /* From 2 to 100000 */
    double        optInNbDev, /* From TA_REAL_MIN to TA_REAL_MAX */
    int* outBegIdx,
    int* outNBElement,
    double        outRealUDist[],
    double        outRealLDist[])
{
    /* Insert local variables here. */
    TA_RetCode retCode;
    int i;
    double tempClose, tempWidth, tempUp;
    ARRAY_REF(tempBuffer1);
    ARRAY_REF(tempBuffer2);

#ifndef TA_FUNC_NO_RANGE_CHECK

    /* Validate the requested output range. */
    if (startIdx < 0)
        return TA_OUT_OF_RANGE_START_INDEX;

    if ((endIdx < 0) || (endIdx < startIdx))
        return TA_OUT_OF_RANGE_END_INDEX;

    if (!inReal) return TA_BAD_PARAM;

    if ((optInTimePeriod < 2) || (optInTimePeriod > 100000))
        return TA_BAD_PARAM;

    if ((optInNbDev < -3.000000e+37) || (optInNbDev > 3.000000e+37))
        return TA_BAD_PARAM;

    if (!outRealUDist)
        return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

    /* Insert TA function code here. */

    tempBuffer1 = outRealUDist;
    tempBuffer2 = outRealLDist;

    retCode = TA_INT_SMA(startIdx, endIdx, inReal,
        optInTimePeriod, outBegIdx, outNBElement, tempBuffer1);

    if ((retCode != TA_SUCCESS) || ((*outNBElement) == 0))
    {
        *outNBElement = 0;
        return retCode;
    }

    /* Calculate the standard deviation into tempBuffer2. */
    TA_INT_stddev_using_precalc_ma(inReal, tempBuffer1,
        (*outBegIdx), (*outNBElement),
        optInTimePeriod, tempBuffer2);

    for (i = 0; i < (*outNBElement); i++)
    {
        tempClose = inReal[*outBegIdx + i];
        tempWidth = tempBuffer2[i] * optInNbDev;
        tempUp = tempBuffer1[i] + tempWidth;
        outRealUDist[i] = (tempClose - tempUp) * 50 / tempWidth;
        outRealLDist[i] = 100 + outRealUDist[i];
    }

    return TA_SUCCESS;
}
