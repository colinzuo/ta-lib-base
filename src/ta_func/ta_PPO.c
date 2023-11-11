#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_PPO_Lookback( int           optInFastPeriod, /* From 2 to 100000 */
                   int           optInSlowPeriod, /* From 2 to 100000 */
                   TA_MAType     optInMAType ) 
{
   /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInFastPeriod. */
   if( (int)optInFastPeriod == TA_INTEGER_DEFAULT )
      optInFastPeriod = 12;
   else if( ((int)optInFastPeriod < 2) || ((int)optInFastPeriod > 100000) )
      return -1;

   /* min/max are checked for optInSlowPeriod. */
   if( (int)optInSlowPeriod == TA_INTEGER_DEFAULT )
      optInSlowPeriod = 26;
   else if( ((int)optInSlowPeriod < 2) || ((int)optInSlowPeriod > 100000) )
      return -1;

   if( (int)optInMAType == TA_INTEGER_DEFAULT )
      optInMAType = (TA_MAType)0;
   else if( ((int)optInMAType < 0) || ((int)optInMAType > 8) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Lookback is driven by the slowest MA. */
   return TA_MA_Lookback( max(optInSlowPeriod,optInFastPeriod), optInMAType );
}

/*
 * TA_PPO - Percentage Price Oscillator
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInFastPeriod:(From 2 to 100000)
 *    Number of period for the fast MA
 * 
 * optInSlowPeriod:(From 2 to 100000)
 *    Number of period for the slow MA
 * 
 * optInMAType:
 *    Type of Moving Average
 * 
 * 
 */
TA_RetCode TA_PPO( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInFastPeriod, /* From 2 to 100000 */
                   int           optInSlowPeriod, /* From 2 to 100000 */
                   TA_MAType     optInMAType,
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] )
{
   /* Insert local variables here. */
   ARRAY_REF(tempBuffer);
   TA_RetCode retCode;

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   if( !inReal ) return TA_BAD_PARAM;
      /* min/max are checked for optInFastPeriod. */
   if( (int)optInFastPeriod == TA_INTEGER_DEFAULT )
      optInFastPeriod = 12;
   else if( ((int)optInFastPeriod < 2) || ((int)optInFastPeriod > 100000) )
      return TA_BAD_PARAM;

   /* min/max are checked for optInSlowPeriod. */
   if( (int)optInSlowPeriod == TA_INTEGER_DEFAULT )
      optInSlowPeriod = 26;
   else if( ((int)optInSlowPeriod < 2) || ((int)optInSlowPeriod > 100000) )
      return TA_BAD_PARAM;

   if( (int)optInMAType == TA_INTEGER_DEFAULT )
      optInMAType = (TA_MAType)0;
   else if( ((int)optInMAType < 0) || ((int)optInMAType > 8) )
      return TA_BAD_PARAM;

   if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* Allocate an intermediate buffer. */
   ARRAY_ALLOC( tempBuffer, endIdx-startIdx+1 );
   #if !defined( _JAVA )
      if( !tempBuffer )
         return TA_ALLOC_ERR;
   #endif

   retCode = TA_PREFIX(INT_PO)( startIdx, endIdx, inReal,
                                    optInFastPeriod,
                                    optInSlowPeriod,
                                    optInMAType,
                                    outBegIdx,
                                    outNBElement,
                                    outReal,
                                    tempBuffer,
                                    1 /* Do percentage processing. */ );

   ARRAY_FREE( tempBuffer );
   
   return retCode;
}
