
#pragma once

#include <stdio.h>
#include <limits.h>
#include <float.h>

#include "ta_defs.h"

/* The following macros are used to return internal errors.
 * The Id can be from 1 to 999 and translate to the user 
 * as the return code 5000 to 5999.
 *
 * Everytime you wish to add a new fatal error code,
 * use the "NEXT AVAILABLE NUMBER" and increment the
 * number in this file.
 *
 * NEXT AVAILABLE NUMBER: 181
 */
#define TA_INTERNAL_ERROR(Id) ((TA_RetCode)(TA_INTERNAL_ERROR+Id))

#ifdef __cplusplus
extern "C" {
#endif

/* Some functions to get the version of TA-Lib.
 *
 * Format is "Major.Minor.Patch (Month Day Year Hour:Min:Sec)"
 * 
 * Example: "1.2.0 (Jan 17 2004 23:59:59)"
 *
 * Major increments indicates an "Highly Recommended" update.
 * 
 * Minor increments indicates arbitrary milestones in the
 * development of the next major version.
 *
 * Patch are fixes to a "Major.Minor" release.
 */
const char *TA_GetVersionString( void );

/* Get individual component of the Version string */
const char *TA_GetVersionMajor ( void );
const char *TA_GetVersionMinor ( void );
const char *TA_GetVersionBuild ( void );
const char *TA_GetVersionDate  ( void );
const char *TA_GetVersionTime  ( void );

/* General purpose structure containing an array of string. 
 *
 * Example of usage:
 *    void printStringTable( TA_StringTable *table )
 *    {
 *       int i;
 *       for( i=0; i < table->size; i++ )
 *          cout << table->string[i] << endl;
 *    }
 *
 */
typedef struct TA_StringTable
{
    unsigned int size;    /* Number of string. */
    const char **string;  /* Pointer to the strings. */

   /* Hidden data for internal use by TA-Lib. Do not modify. */
   void *hiddenData;
} TA_StringTable;

/* End-user can get additional information related to a TA_RetCode. 
 *
 * Example:
 *        TA_RetCodeInfo info;
 *
 *        retCode = TA_Initialize( ... );
 *
 *        if( retCode != TA_SUCCESS )
 *        {
 *           TA_SetRetCodeInfo( retCode, &info );
 *           printf( "Error %d(%s): %s\n",
 *                   retCode,
 *                   info.enumStr,
 *                   info.infoStr );
 *        }
 *
 * Would display:
 *        "Error 1(TA_LIB_NOT_INITIALIZE): TA_Initialize was not sucessfully called"
 */
typedef struct TA_RetCodeInfo
{
   const char *enumStr; /* Like "TA_IP_SOCKETERROR"     */
   const char *infoStr; /* Like "Error creating socket" */      
} TA_RetCodeInfo;

/* Info is always returned, even when 'theRetCode' is invalid. */
void TA_SetRetCodeInfo( TA_RetCode theRetCode, TA_RetCodeInfo *retCodeInfo );
 
/* TA_Initialize() initialize the ressources used by TA-Lib. This
 * function must be called once prior to any other functions declared in
 * this file.
 *
 * TA_Shutdown() allows to free all ressources used by TA-Lib. Following
 * a shutdown, TA_Initialize() must be called again for re-using TA-Lib.
 *
 * TA_Shutdown() should be called prior to exiting the application code.
 */
TA_RetCode TA_Initialize( void );
TA_RetCode TA_Shutdown( void );

/* Some TA functions takes a certain amount of input data
 * before stabilizing and outputing meaningful data. This is
 * a behavior pertaining to the algo of some TA functions and
 * is not particular to the TA-Lib implementation.
 * TA-Lib allows you to automatically strip off these unstabl
 * data from your output and from any internal processing.
 * (See documentation for more info)
 *
 * Examples:
 *      TA_SetUnstablePeriod( TA_FUNC_UNST_EMA, 30 );
 *           Always strip off 30 price bar for the TA_EMA function.
 *
 *      TA_SetUnstablePeriod( TA_FUNC_UNST_ALL, 30 );
 *           Always strip off 30 price bar from ALL functions
 *           having an unstable period.
 *
 * See ta_defs.h for the enumeration TA_FuncUnstId
 */

TA_RetCode TA_SetUnstablePeriod(TA_FuncUnstId id,
    unsigned int  unstablePeriod);

unsigned int TA_GetUnstablePeriod(TA_FuncUnstId id);

/* You can change slightly the behavior of the TA functions
 * by requesting compatibiliy with some existing software.
 *
 * By default, the behavior is as close as the original
 * author of the TA functions intend it to be.
 *
 * See ta_defs.h for the enumeration TA_Compatibility.
 */
TA_RetCode TA_SetCompatibility(TA_Compatibility value);
TA_Compatibility TA_GetCompatibility(void);

/* Candlesticks struct and functions
 * Because candlestick patterns are subjective, it is necessary
 * to allow the user to specify what should be the meaning of
 * 'long body', 'short shadows', etc.
 */

 /* Call TA_SetCandleSettings to set that when comparing a candle
  * basing on settingType it must be compared with the average
  * of the last avgPeriod candles' rangeType multiplied by factor.
  * This setting is valid until TA_RestoreCandleDefaultSettings is called
  */
TA_RetCode TA_SetCandleSettings(TA_CandleSettingType settingType,
    TA_RangeType rangeType,
    int avgPeriod,
    double factor);

/* Call TA_RestoreCandleDefaultSettings after using custom settings
 * to restore the default settings for the specified settingType
 */
TA_RetCode TA_RestoreCandleDefaultSettings(TA_CandleSettingType settingType);

#ifdef __cplusplus
}
#endif
