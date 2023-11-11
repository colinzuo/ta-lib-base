/* Description:
 *   Provides initialization / shutdown functionality for all modules.
 */

/**** Headers ****/
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ta_common.h"
#include "ta_global.h"

#define TA_LIBC_PRIV_MAGIC_NB           0xA203B203

/**** External functions declarations. ****/
/* None */

/**** External variables declarations. ****/
/* None */

/**** Global variables definitions.    ****/

/* The entry point for all globals */
TA_LibcPriv ta_theGlobals = {0,(TA_Compatibility)0,{0},{{(TA_CandleSettingType)0,(TA_RangeType)0,0,0}}};

TA_LibcPriv *TA_Globals = &ta_theGlobals;

/**** Local declarations.              ****/
/* None */

/**** Local functions declarations.    ****/
/* None */

/**** Local variables definitions.     ****/
/* None */

/**** Global functions definitions.   ****/
TA_RetCode TA_Initialize( void )
{
   /* Initialize the "global variable" used to manage the global
    * variables of all other modules...
    */
   memset( TA_Globals, 0, sizeof( TA_LibcPriv ) );
   TA_Globals->magicNb = TA_LIBC_PRIV_MAGIC_NB;	  

   /*** At this point, TA_Shutdown can be called to clean-up. ***/

   /* Set the default value to global variables */
   TA_RestoreCandleDefaultSettings( TA_AllCandleSettings );

   return TA_SUCCESS;
}

TA_RetCode TA_Shutdown( void )
{
   if( TA_Globals->magicNb != TA_LIBC_PRIV_MAGIC_NB )
      return TA_LIB_NOT_INITIALIZE;

   /* Initialize to all zero to make sure we invalidate that object. */
   memset( TA_Globals, 0, sizeof( TA_LibcPriv ) );

   return TA_SUCCESS;
}

TA_RetCode TA_SetCandleSettings( TA_CandleSettingType settingType, 
                                 TA_RangeType rangeType, 
                                 int avgPeriod, 
                                 double factor )
{
    /*printf("setcdlset:%d  ",settingType);*/
    if ( settingType < 0 || settingType >= TA_AllCandleSettings )
        return TA_BAD_PARAM;
    TA_Globals->candleSettings[settingType].settingType = settingType;
    TA_Globals->candleSettings[settingType].rangeType = rangeType;
    TA_Globals->candleSettings[settingType].avgPeriod = avgPeriod;
    TA_Globals->candleSettings[settingType].factor = factor;
    /*printf("cdlset: %d %d %d %f\n",TA_Globals->candleSettings[settingType].settingType,TA_Globals->candleSettings[settingType].rangeType,
        TA_Globals->candleSettings[settingType].avgPeriod,TA_Globals->candleSettings[settingType].factor);*/
    return TA_SUCCESS;
}

TA_RetCode TA_RestoreCandleDefaultSettings( TA_CandleSettingType settingType )
{
    const TA_CandleSetting TA_CandleDefaultSettings[] = {
        /* real body is long when it's longer than the average of the 10 previous candles' real body */
        { TA_BodyLong, TA_RangeType_RealBody, 10, 1.0 },
        /* real body is very long when it's longer than 3 times the average of the 10 previous candles' real body */
        { TA_BodyVeryLong, TA_RangeType_RealBody, 10, 3.0 },
        /* real body is short when it's shorter than the average of the 10 previous candles' real bodies */
        { TA_BodyShort, TA_RangeType_RealBody, 10, 1.0 },
        /* real body is like doji's body when it's shorter than 10% the average of the 10 previous candles' high-low range */
        { TA_BodyDoji, TA_RangeType_HighLow, 10, 0.1 },
        /* shadow is long when it's longer than the real body */
        { TA_ShadowLong, TA_RangeType_RealBody, 0, 1.0 },
        /* shadow is very long when it's longer than 2 times the real body */
        { TA_ShadowVeryLong, TA_RangeType_RealBody, 0, 2.0 },
        /* shadow is short when it's shorter than half the average of the 10 previous candles' sum of shadows */
        { TA_ShadowShort, TA_RangeType_Shadows, 10, 1.0 },
        /* shadow is very short when it's shorter than 10% the average of the 10 previous candles' high-low range */
        { TA_ShadowVeryShort, TA_RangeType_HighLow, 10, 0.1 },
        /* when measuring distance between parts of candles or width of gaps */
        /* "near" means "<= 20% of the average of the 5 previous candles' high-low range" */
        { TA_Near, TA_RangeType_HighLow, 5, 0.2 },
        /* when measuring distance between parts of candles or width of gaps */
        /* "far" means ">= 60% of the average of the 5 previous candles' high-low range" */
        { TA_Far, TA_RangeType_HighLow, 5, 0.6 },
        /* when measuring distance between parts of candles or width of gaps */
        /* "equal" means "<= 5% of the average of the 5 previous candles' high-low range" */
        { TA_Equal, TA_RangeType_HighLow, 5, 0.05 }
    };

    int i;
    if ( settingType < 0 || settingType > TA_AllCandleSettings )
        return TA_BAD_PARAM;

    if (settingType == TA_AllCandleSettings) {
        for (i = 0; i < TA_AllCandleSettings; ++i) {
            TA_Globals->candleSettings[i] = TA_CandleDefaultSettings[i];
        }
    }
    else {
        TA_Globals->candleSettings[settingType] = TA_CandleDefaultSettings[settingType];
    }

    return TA_SUCCESS;
}

TA_RetCode TA_SetUnstablePeriod(TA_FuncUnstId id, unsigned int  unstablePeriod)
{
    int i;

    if (id < 0 || id > TA_FUNC_UNST_ALL)
        return TA_BAD_PARAM;

    if (id == TA_FUNC_UNST_ALL)
    {
        for (i = 0; i < (int)TA_FUNC_UNST_ALL; i++)
        {
            TA_Globals->unstablePeriod[i] = unstablePeriod;
        }
    }
    else
    {
        TA_Globals->unstablePeriod[id] = unstablePeriod;    
    }

    return TA_SUCCESS;
}

unsigned int TA_GetUnstablePeriod(TA_FuncUnstId id)
{
    if (id < 0 || id >= TA_FUNC_UNST_ALL)
        return 0;

    return TA_Globals->unstablePeriod[id];
}

TA_RetCode TA_SetCompatibility(TA_Compatibility value)
{
    TA_Globals->compatibility = value;
    return TA_SUCCESS;
}

TA_Compatibility TA_GetCompatibility(void)
{
    return TA_Globals->compatibility;
}

/**** Local functions definitions.     ****/
/* None */


