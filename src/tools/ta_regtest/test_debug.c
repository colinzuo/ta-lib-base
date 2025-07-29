#include <stdio.h>
#include <string.h>

#include "ta_test_priv.h"
#include "ta_test_func.h"
#include "ta_utility.h"
#include "ta_memory.h"

static ErrorNumber do_test_debug(const TA_History* history);

ErrorNumber test_func_debug(TA_History* history)
{
    ErrorNumber retValue;

    /* Re-initialize all the unstable period to zero. */
    TA_SetUnstablePeriod(TA_FUNC_UNST_ALL, 0);

    retValue = do_test_debug(history);

    if (retValue != TA_TEST_PASS) {
        return retValue;
    }

    /* Re-initialize all the unstable period to zero. */
    TA_SetUnstablePeriod(TA_FUNC_UNST_ALL, 0);

    /* All test succeed. */
    return TA_TEST_PASS;
}

static ErrorNumber do_test_debug(const TA_History* history) {
    TA_RetCode retCode;
    int outBegIdx;
    int outNbElement;

    /* Set to NAN all the elements of the gBuffers.  */
    clearAllBuffers();

    /* Build the input. */
    setInputBuffer(0, history->close, history->nbBars);
    setInputBuffer(1, history->close, history->nbBars);

    /* Re-initialize all the unstable period to zero. */
    TA_SetUnstablePeriod(TA_FUNC_UNST_ALL, 0);

#if 0
    retCode = TA_TrendData(0,
        251,
        gBuffer[0].in,
        40, 2, 30, 20, 30, 15,
        &outBegIdx,
        &outNbElement,
        gBuffer[0].out0,
        gBuffer[0].out1,
        gBuffer[0].out2,
        gBuffer[1].out0);
#endif

    retCode = TA_Bias(0,
        251,
        gBuffer[0].in,
        1, 40, 0,
        &outBegIdx,
        &outNbElement,
        gBuffer[0].out0);

    return TA_TEST_PASS;
}