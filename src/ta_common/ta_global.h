#pragma once

#include "ta_common.h"

/* TA_CandleSetting is the one setting struct */
typedef struct {
    TA_CandleSettingType    settingType;
    TA_RangeType            rangeType;
    int                     avgPeriod;
    double                  factor;
} TA_CandleSetting;

/* This is the hidden implementation of TA_Libc. */
typedef struct
{
   unsigned int magicNb; /* Unique identifier of this object. */

   /* For handling the compatibility with other software */
   TA_Compatibility compatibility;

   /* For handling the unstable period of some TA function. */
   unsigned int unstablePeriod[TA_FUNC_UNST_ALL];

   /* For handling the candlestick global settings */
   TA_CandleSetting candleSettings[TA_AllCandleSettings];
} TA_LibcPriv;

/* The following global is used all over the place 
 * and is the entry point for all other globals.
 */
extern TA_LibcPriv *TA_Globals;
