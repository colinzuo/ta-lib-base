#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_STOCHRSI_Lookback( int           optInTimePeriod, /* From 2 to 100000 */
                        int           optInFastK_Period, /* From 1 to 100000 */
                        int           optInFastD_Period, /* From 1 to 100000 */
                        TA_MAType     optInFastD_MAType ) 
{
   /* insert local variable here */
   int retValue;

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 14;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return -1;

   /* min/max are checked for optInFastK_Period. */
   if( (int)optInFastK_Period == TA_INTEGER_DEFAULT )
      optInFastK_Period = 5;
   else if( ((int)optInFastK_Period < 1) || ((int)optInFastK_Period > 100000) )
      return -1;

   /* min/max are checked for optInFastD_Period. */
   if( (int)optInFastD_Period == TA_INTEGER_DEFAULT )
      optInFastD_Period = 3;
   else if( ((int)optInFastD_Period < 1) || ((int)optInFastD_Period > 100000) )
      return -1;

   if( (int)optInFastD_MAType == TA_INTEGER_DEFAULT )
      optInFastD_MAType = (TA_MAType)0;
   else if( ((int)optInFastD_MAType < 0) || ((int)optInFastD_MAType > 8) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */
   retValue = TA_RSI_Lookback( optInTimePeriod ) + TA_STOCHF_Lookback( optInFastK_Period, optInFastD_Period, optInFastD_MAType );

   return retValue;
}

/*
 * TA_STOCHRSI - Stochastic Relative Strength Index
 * 
 * Input  = double
 * Output = double, double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * optInFastK_Period:(From 1 to 100000)
 *    Time period for building the Fast-K line
 * 
 * optInFastD_Period:(From 1 to 100000)
 *    Smoothing for making the Fast-D line. Usually set to 3
 * 
 * optInFastD_MAType:
 *    Type of Moving Average for Fast-D
 * 
 * 
 */
TA_RetCode TA_STOCHRSI( int    startIdx,
                        int    endIdx,
                        const double inReal[],
                        int           optInTimePeriod, /* From 2 to 100000 */
                        int           optInFastK_Period, /* From 1 to 100000 */
                        int           optInFastD_Period, /* From 1 to 100000 */
                        TA_MAType     optInFastD_MAType,
                        int          *outBegIdx,
                        int          *outNBElement,
                        double        outFastK[],
                        double        outFastD[] )
{
   /* insert local variable here */
   ARRAY_REF(tempRSIBuffer);

   TA_RetCode retCode;
   int lookbackTotal, lookbackSTOCHF, tempArraySize;
   int outBegIdx1;
   int outBegIdx2;
   int outNbElement1;

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   if( !inReal ) return TA_BAD_PARAM;
      /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 14;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return TA_BAD_PARAM;

   /* min/max are checked for optInFastK_Period. */
   if( (int)optInFastK_Period == TA_INTEGER_DEFAULT )
      optInFastK_Period = 5;
   else if( ((int)optInFastK_Period < 1) || ((int)optInFastK_Period > 100000) )
      return TA_BAD_PARAM;

   /* min/max are checked for optInFastD_Period. */
   if( (int)optInFastD_Period == TA_INTEGER_DEFAULT )
      optInFastD_Period = 3;
   else if( ((int)optInFastD_Period < 1) || ((int)optInFastD_Period > 100000) )
      return TA_BAD_PARAM;

   if( (int)optInFastD_MAType == TA_INTEGER_DEFAULT )
      optInFastD_MAType = (TA_MAType)0;
   else if( ((int)optInFastD_MAType < 0) || ((int)optInFastD_MAType > 8) )
      return TA_BAD_PARAM;

   if( !outFastK )
      return TA_BAD_PARAM;

   if( !outFastD )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */
  
   /* Stochastic RSI
    * 
    * Reference: "Stochastic RSI and Dynamic Momentum Index"
    *            by Tushar Chande and Stanley Kroll
    *            Stock&Commodities V.11:5 (189-199)
    *
    * The TA-Lib version offer flexibility beyond what is explain
    * in the Stock&Commodities article.
    *
    * To calculate the "Unsmoothed stochastic RSI" with symetry like 
    * explain in the article, keep the optInTimePeriod and optInFastK_Period
    * equal. Example:
    *        
    *    unsmoothed stoch RSI 14 : optInTimePeriod   = 14
    *                              optInFastK_Period = 14
    *                              optInFastD_Period = 'x'
    *
    * The outFastK is the unsmoothed RSI discuss in the article.
    *
    * You can set the optInFastD_Period to smooth the RSI. The smooth
    * version will be found in outFastD. The outFastK will still contain
    * the unsmoothed stoch RSI. If you do not care about the smoothing of
    * the StochRSI, just leave optInFastD_Period to 1 and ignore outFastD.
    */

   *outBegIdx = 0;
   *outNBElement = 0;

   /* Adjust startIdx to account for the lookback period. */
   lookbackSTOCHF  = TA_STOCHF_Lookback( optInFastK_Period, optInFastD_Period, optInFastD_MAType );
   lookbackTotal   = TA_RSI_Lookback( optInTimePeriod ) + lookbackSTOCHF;

   if( startIdx < lookbackTotal )
      startIdx = lookbackTotal;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
   {
      *outBegIdx = 0;
      *outNBElement = 0;
      return TA_SUCCESS;
   }

   *outBegIdx = startIdx;

   tempArraySize = (endIdx - startIdx) + 1 + lookbackSTOCHF;

   ARRAY_ALLOC( tempRSIBuffer, tempArraySize );

   retCode = TA_RSI(startIdx-lookbackSTOCHF, 
                                endIdx, 
                                inReal, 
                                optInTimePeriod, 
                                &outBegIdx1,
                                &outNbElement1, 
                                tempRSIBuffer);

   if( retCode != TA_SUCCESS || outNbElement1 == 0 )
   {
      ARRAY_FREE( tempRSIBuffer );
      *outBegIdx = 0;
      *outNBElement = 0;
      return retCode;
   }
   
   retCode = TA_STOCHF(0,
                                          tempArraySize-1,
                                          tempRSIBuffer,
                                          tempRSIBuffer,
                                          tempRSIBuffer,
                                          optInFastK_Period,
                                          optInFastD_Period,
                                          optInFastD_MAType,
                                          &outBegIdx2,
                                          outNBElement,
                                          outFastK,
                                          outFastD);
 
   ARRAY_FREE( tempRSIBuffer );

   if( retCode != TA_SUCCESS || ((int)(*outNBElement)) == 0 )
   {
      *outBegIdx = 0;
      *outNBElement = 0;
      return retCode;
   }

   return TA_SUCCESS;
}
