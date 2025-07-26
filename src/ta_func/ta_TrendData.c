#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_TrendData_Lookback(int           optInTimePeriod,
    double        optInNbDev,
    double        optInWidthThres,
    double        optInDistThres,
    double        optInDaysThres,
    double        optInPctThres)
{
    /* insert local variable here */

    /* insert lookback code here. */
    (void)optInNbDev;

    /* The lookback is driven by the middle band moving average. */
    return TA_SMA_Lookback(optInTimePeriod);
}

/*
 * TA_TrendData - Trend Data
 *
 * Input  = double
 * Output = double, double, double, double
 *
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 *
 * optInNbDev:(From TA_REAL_MIN to TA_REAL_MAX)
 *    Deviation multiplier
 *
 * optInWidthThres:(From 10 to 60)
 *    band width threshold
 * 
 * optInDistThres:(From 0 to 30)
 *    distance to up/down band threshold
 *
 * optInDaysThres:(From 0 to 100)
 *    days thres to be valid for switch
 *
 * optInPctThres:(From 0 to 30)
 *    percent thres to be valid for switch
 *
 */
TA_RetCode TA_TrendData(int    startIdx,
    int    endIdx,
    const double inReal[],
    int           optInTimePeriod,
    double        optInNbDev,
    double        optInWidthThres,
    double        optInDistThres,
    double        optInDaysThres,
    double        optInPctThres,
    int* outBegIdx,
    int* outNBElement,
    double        outUpDays[],
    double        outUpPercent[],
    double        outDownDays[],
    double        outDownPercent[])
{
    /* Insert local variables here. */
    TA_RetCode retCode;
    int i;
    double tempClose, tempUdist, tempLdist, tempWidth, lastHigh, lastLow, tempDays, tempPct;
    int lastHighIdx = -1, lastLowIdx = -1, lastHighLowState = 0;
    ARRAY_REF(tempBuffer1);
    ARRAY_REF(tempBuffer2);
    ARRAY_REF(tempBuffer3);

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

    if ((optInDistThres < 0) || (optInDistThres > 30))
        return TA_BAD_PARAM;

    if ((optInDaysThres < 0) || (optInDaysThres > 100))
        return TA_BAD_PARAM;

    if ((optInPctThres < 0) || (optInPctThres > 30))
        return TA_BAD_PARAM;

    if (!outUpDays || !outUpPercent)
        return TA_BAD_PARAM;

    if (!outDownDays || !outDownPercent)
        return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

    /* Insert TA function code here. */

    tempBuffer1 = outUpDays;
    tempBuffer2 = outUpPercent;
    tempBuffer3 = outDownDays;

    retCode = TA_BBDist(startIdx, endIdx, inReal,
        optInTimePeriod, optInNbDev, outBegIdx, outNBElement, tempBuffer1, tempBuffer2);

    if ((retCode != TA_SUCCESS) || ((*outNBElement) == 0))
    {
        *outNBElement = 0;
        return retCode;
    }

    retCode = TA_BBWidth(startIdx, endIdx, inReal,
        optInTimePeriod, optInNbDev, outBegIdx, outNBElement, tempBuffer3);

    if ((retCode != TA_SUCCESS) || ((*outNBElement) == 0))
    {
        *outNBElement = 0;
        return retCode;
    }

    for (i = 0; i < (*outNBElement); i++)
    {
        tempClose = inReal[*outBegIdx + i];
        tempUdist = tempBuffer1[i];
        tempLdist = tempBuffer2[i];
        tempWidth = tempBuffer3[i];
        outUpDays[i] = 0;
        outUpPercent[i] = 0;
        outDownDays[i] = 0;
        outDownPercent[i] = 0;
        if (lastHighLowState == 1) {
            if (tempClose >= lastHigh) {
                lastHigh = tempClose;
                lastHighIdx = i;

                if (lastLowIdx >= 0) {
                    outUpDays[i] = i - lastLowIdx;
                    outUpPercent[i] = (tempClose - lastLow) * 100 / lastLow;
                }
            } else if (tempUdist >= -1 * optInDistThres) {
                if (lastLowIdx >= 0) {
                    outUpDays[i] = i - lastLowIdx;
                    outUpPercent[i] = (tempClose - lastLow) * 100 / lastLow;
                }
            } else if (tempLdist <= optInDistThres && tempWidth >= optInWidthThres) {
                tempDays = i - lastHighIdx;
                tempPct = (lastHigh - tempClose) * 100 / lastHigh;

                if (tempDays >= optInDaysThres && tempPct >= optInPctThres) {
                    lastLow = tempClose;
                    lastLowIdx = i;

                    lastHighLowState = -1;

                    outDownDays[i] = i - lastHighIdx;
                    outDownPercent[i] = (lastHigh - tempClose) * 100 / lastHigh;
                }
            }
        }
        else if (lastHighLowState == -1) {
            if (tempClose <= lastLow) {
                lastLow = tempClose;
                lastLowIdx = i;

                if (lastHighIdx >= 0) {
                    outDownDays[i] = i - lastHighIdx;
                    outDownPercent[i] = (lastHigh - tempClose) * 100 / lastHigh;
                }
            }
            else if (tempLdist <= optInDistThres) {
                if (lastHighIdx >= 0) {
                    outDownDays[i] = i - lastHighIdx;
                    outDownPercent[i] = (lastHigh - tempClose) * 100 / lastHigh;
                }
            }
            else if (tempUdist >= -1 * optInDistThres && tempWidth >= optInWidthThres) {
                tempDays = i - lastLowIdx;
                tempPct = (tempClose - lastLow) * 100 / lastLow;

                if (tempDays >= optInDaysThres && tempPct >= optInPctThres) {
                    lastHigh = tempClose;
                    lastHighIdx = i;

                    lastHighLowState = 1;

                    outUpDays[i] = i - lastLowIdx;
                    outUpPercent[i] = (tempClose - lastLow) * 100 / lastLow;
                }
            }
        }
        else {
            if (tempUdist >= -1 * optInDistThres && tempWidth >= optInWidthThres) {
                lastHigh = tempClose;
                lastHighIdx = i;

                lastHighLowState = 1;
            }
            else if (tempLdist <= optInDistThres && tempWidth >= optInWidthThres) {
                lastLow = tempClose;
                lastLowIdx = i;

                lastHighLowState = -1;
            }
        }
    }

    return TA_SUCCESS;
}
