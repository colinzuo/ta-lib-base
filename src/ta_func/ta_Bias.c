#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_Bias_Lookback(int           optInTimePeriod1,
    int           optInTimePeriod2,
    int           optInTimePeriod2Shift)
{
    /* insert local variable here */

    /* insert lookback code here. */
    (void)optInTimePeriod1;

    return max(TA_SmaExt_Lookback(optInTimePeriod1, 0), TA_SmaExt_Lookback(optInTimePeriod2, optInTimePeriod2Shift));
}

/*
 * TA_Bias - Bias
 *
 * Input  = double
 * Output = double
 *
 * Optional Parameters
 * -------------------
 * optInTimePeriod1:(From 1 to 100000)
 *    Number of period 1
 *
 * optInTimePeriod2:(From 1 to 100000)
 *    Number of period 2
 *
 * optInTimePeriod2Shift:(From 0 to 100000)
 *    shift of period 2
 */
TA_RetCode TA_Bias(int    startIdx,
    int    endIdx,
    const double inReal[],
    int           optInTimePeriod1,
    int           optInTimePeriod2,
    int           optInTimePeriod2Shift,
    int* outBegIdx,
    int* outNBElement,
    double        outPercent[])
{
    /* Insert local variables here. */
    TA_RetCode retCode;
    int i;
    double tempReal1, tempReal2;
    int begIdx1, nbElement1, begIdx2, nbElement2, offset1, offset2;
    ARRAY_REF(tempBuffer1);
    ARRAY_REF(tempBuffer2);

#ifndef TA_FUNC_NO_RANGE_CHECK

    /* Validate the requested output range. */
    if (startIdx < 0)
        return TA_OUT_OF_RANGE_START_INDEX;

    if ((endIdx < 0) || (endIdx < startIdx))
        return TA_OUT_OF_RANGE_END_INDEX;

    if (!inReal) return TA_BAD_PARAM;

    if ((optInTimePeriod1 < 1) || (optInTimePeriod1 > 100000))
        return TA_BAD_PARAM;

    if ((optInTimePeriod2 < 1) || (optInTimePeriod2 > 100000))
        return TA_BAD_PARAM;

    if ((optInTimePeriod2Shift < 0) || (optInTimePeriod2Shift > 100000))
        return TA_BAD_PARAM;

    if (!outPercent)
        return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

    /* Insert TA function code here. */
    ARRAY_ALLOC(tempBuffer1, endIdx - startIdx + 1);

    if (!tempBuffer1)
    {
        *outNBElement = 0;
        return TA_ALLOC_ERR;
    }

    ARRAY_ALLOC(tempBuffer2, endIdx - startIdx + 1);

    if (!tempBuffer2)
    {
        ARRAY_FREE(tempBuffer1);
        *outNBElement = 0;
        return TA_ALLOC_ERR;
    }

    retCode = TA_SmaExt(startIdx, endIdx, inReal,
        optInTimePeriod1, 0, &begIdx1, &nbElement1, tempBuffer1);

    if ((retCode != TA_SUCCESS) || (nbElement1 == 0))
    {
        ARRAY_FREE(tempBuffer1);
        ARRAY_FREE(tempBuffer2);

        *outNBElement = 0;
        return retCode;
    }

    retCode = TA_SmaExt(startIdx, endIdx, inReal,
        optInTimePeriod2, optInTimePeriod2Shift, &begIdx2, &nbElement2, tempBuffer2);

    if ((retCode != TA_SUCCESS) || (nbElement2 == 0))
    {
        ARRAY_FREE(tempBuffer1);
        ARRAY_FREE(tempBuffer2);

        *outNBElement = 0;
        return retCode;
    }

    if (nbElement2 <= nbElement1) {
        *outBegIdx = begIdx2;
        *outNBElement = nbElement2;

        offset1 = begIdx2 - begIdx1;
        offset2 = 0;
    }
    else {
        *outBegIdx = begIdx1;
        *outNBElement = nbElement1;

        offset1 = 0;
        offset2 = begIdx1 - begIdx2;
    }

    for (i = 0; i < (*outNBElement); i++)
    {
        tempReal1 = tempBuffer1[offset1 + i];
        tempReal2 = tempBuffer2[offset2 + i];
        outPercent[i] = (tempReal1 - tempReal2) * 100 / tempReal2;
    }

    ARRAY_FREE(tempBuffer1);
    ARRAY_FREE(tempBuffer2);

    return TA_SUCCESS;
}
