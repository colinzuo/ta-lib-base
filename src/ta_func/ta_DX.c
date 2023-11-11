#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_DX_Lookback( int           optInTimePeriod )  /* From 2 to 100000 */
{
   /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 14;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */
   if( optInTimePeriod > 1 )
      return optInTimePeriod + TA_GLOBALS_UNSTABLE_PERIOD(TA_FUNC_UNST_DX,Dx);
   else
      return 2;
}

/*
 * TA_DX - Directional Movement Index
 * 
 * Input  = High, Low, Close
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_DX( int    startIdx,
                  int    endIdx,
                  const double inHigh[],
                  const double inLow[],
                  const double inClose[],
                  int           optInTimePeriod, /* From 2 to 100000 */
                  int          *outBegIdx,
                  int          *outNBElement,
                  double        outReal[] )
{
	/* insert local variable here */
   int today, lookbackTotal, outIdx;
   double prevHigh, prevLow, prevClose;
   double prevMinusDM, prevPlusDM, prevTR;
   double tempReal, tempReal2, diffP, diffM;
   double minusDI, plusDI;

   int i;

   #define TRUE_RANGE(TH,TL,YC,OUT) {\
      OUT = TH-TL; \
      tempReal2 = std_fabs(TH-YC); \
      if( tempReal2 > OUT ) \
         OUT = tempReal2; \
      tempReal2 = std_fabs(TL-YC); \
      if( tempReal2 > OUT ) \
         OUT = tempReal2; \
   }

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   /* Verify required price component. */
   if(!inHigh||!inLow||!inClose)
      return TA_BAD_PARAM;

      /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 14;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return TA_BAD_PARAM;

   if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* 
    * The DM1 (one period) is base on the largest part of
    * today's range that is outside of yesterdays range.
    * 
    * The following 7 cases explain how the +DM and -DM are
    * calculated on one period:
    *
    * Case 1:                       Case 2:
    *    C|                        A|
    *     |                         | C|
    *     | +DM1 = (C-A)           B|  | +DM1 = 0
    *     | -DM1 = 0                   | -DM1 = (B-D)
    * A|  |                           D| 
    *  | D|                    
    * B|
    *
    * Case 3:                       Case 4:
    *    C|                           C|
    *     |                        A|  |
    *     | +DM1 = (C-A)            |  | +DM1 = 0
    *     | -DM1 = 0               B|  | -DM1 = (B-D)
    * A|  |                            | 
    *  |  |                           D|
    * B|  |
    *    D|
    * 
    * Case 5:                      Case 6:
    * A|                           A| C|
    *  | C| +DM1 = 0                |  |  +DM1 = 0
    *  |  | -DM1 = 0                |  |  -DM1 = 0
    *  | D|                         |  |
    * B|                           B| D|
    *
    *
    * Case 7:
    * 
    *    C|
    * A|  |
    *  |  | +DM=0
    * B|  | -DM=0
    *    D|
    *
    * In case 3 and 4, the rule is that the smallest delta between
    * (C-A) and (B-D) determine which of +DM or -DM is zero.
    *
    * In case 7, (C-A) and (B-D) are equal, so both +DM and -DM are
    * zero.
    *
    * The rules remain the same when A=B and C=D (when the highs
    * equal the lows).
    *
    * When calculating the DM over a period > 1, the one-period DM
    * for the desired period are initialy sum. In other word, 
    * for a -DM14, sum the -DM1 for the first 14 days (that's 
    * 13 values because there is no DM for the first day!)
    * Subsequent DM are calculated using the Wilder's
    * smoothing approach:
    * 
    *                                    Previous -DM14
    *  Today's -DM14 = Previous -DM14 -  -------------- + Today's -DM1
    *                                         14
    *
    * Calculation of a -DI14 is as follow:
    * 
    *               -DM14
    *     -DI14 =  --------
    *                TR14
    *
    * Calculation of the TR14 is:
    *
    *                                   Previous TR14
    *    Today's TR14 = Previous TR14 - -------------- + Today's TR1
    *                                         14
    *
    *    The first TR14 is the summation of the first 14 TR1. See the
    *    TA_TRANGE function on how to calculate the true range.
    *
    * Calculation of the DX14 is:
    *    
    *    diffDI = ABS( (-DI14) - (+DI14) )
    *    sumDI  = (-DI14) + (+DI14)
    *
    *    DX14 = 100 * (diffDI / sumDI)
    *
    * Reference:
    *    New Concepts In Technical Trading Systems, J. Welles Wilder Jr
    */

   /* Original implementation from Wilder's book was doing some integer
    * rounding in its calculations.
    *
    * This was understandable in the context that at the time the book
    * was written, most user were doing the calculation by hand.
    * 
    * For a computer, rounding is unnecessary (and even problematic when inputs
    * are close to 1).
    *
    * TA-Lib does not do the rounding. Still, if you want to reproduce Wilder's examples,
    * you can comment out the following #undef/#define and rebuild the library.
    */
   #undef  round_pos
   #define round_pos(x) (x)

   if( optInTimePeriod > 1 )
      lookbackTotal = optInTimePeriod + TA_GLOBALS_UNSTABLE_PERIOD(TA_FUNC_UNST_DX,Dx);
   else
      lookbackTotal = 2;

   /* Adjust startIdx to account for the lookback period. */
   if( startIdx < lookbackTotal )
      startIdx = lookbackTotal;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
   {
      *outBegIdx = 0;
      *outNBElement = 0;
      return TA_SUCCESS;
   }

   /* Indicate where the next output should be put
    * in the outReal.
    */
   outIdx = 0;

   /* Process the initial DM and TR */
   *outBegIdx = today = startIdx;

   prevMinusDM = 0.0;
   prevPlusDM  = 0.0;
   prevTR      = 0.0;
   today       = startIdx - lookbackTotal;
   prevHigh    = inHigh[today];
   prevLow     = inLow[today];
   prevClose   = inClose[today];
   i           = optInTimePeriod-1;
   while( i-- > 0 )
   {
      today++;
      tempReal = inHigh[today];
      diffP    = tempReal-prevHigh; /* Plus Delta */
      prevHigh = tempReal;

      tempReal = inLow[today];
      diffM    = prevLow-tempReal;   /* Minus Delta */
      prevLow  = tempReal;

      if( (diffM > 0) && (diffP < diffM) )
      {
          /* Case 2 and 4: +DM=0,-DM=diffM */
          prevMinusDM += diffM;
      }
      else if( (diffP > 0) && (diffP > diffM) )
      {
          /* Case 1 and 3: +DM=diffP,-DM=0 */
          prevPlusDM += diffP;
      }

      TRUE_RANGE(prevHigh,prevLow,prevClose,tempReal);
      prevTR += tempReal;
      prevClose = inClose[today];
   }

   /* Skip the unstable period. Note that this loop must be executed
    * at least ONCE to calculate the first DI.
    */
   i = TA_GLOBALS_UNSTABLE_PERIOD(TA_FUNC_UNST_DX,Dx) + 1;
   while( i-- != 0 )
   {
      /* Calculate the prevMinusDM and prevPlusDM */
      today++;
      tempReal = inHigh[today];
      diffP    = tempReal-prevHigh; /* Plus Delta */
      prevHigh = tempReal;

      tempReal = inLow[today];
      diffM    = prevLow-tempReal;   /* Minus Delta */
      prevLow  = tempReal;

      prevMinusDM -= prevMinusDM/optInTimePeriod;
      prevPlusDM  -= prevPlusDM/optInTimePeriod;

      if( (diffM > 0) && (diffP < diffM) )
      {
         /* Case 2 and 4: +DM=0,-DM=diffM */
         prevMinusDM += diffM;
      }
      else if( (diffP > 0) && (diffP > diffM) )
      {
         /* Case 1 and 3: +DM=diffP,-DM=0 */
         prevPlusDM += diffP;
      }

      /* Calculate the prevTR */
      TRUE_RANGE(prevHigh,prevLow,prevClose,tempReal);
      prevTR = prevTR - (prevTR/optInTimePeriod) + tempReal;
      prevClose = inClose[today];
   }

   /* Write the first DX output */
   if( !TA_IS_ZERO(prevTR) )
   {
      minusDI = round_pos(100.0*(prevMinusDM/prevTR));
      plusDI  = round_pos(100.0*(prevPlusDM/prevTR));
      tempReal = minusDI+plusDI;
      if( !TA_IS_ZERO(tempReal) )
         outReal[0] = round_pos( 100.0 * (std_fabs(minusDI-plusDI)/tempReal) );
      else
         outReal[0] = 0.0;
   }
   else
      outReal[0] = 0.0;
   outIdx = 1;

   while( today < endIdx )
   {
      /* Calculate the prevMinusDM and prevPlusDM */
      today++;
      tempReal = inHigh[today];
      diffP    = tempReal-prevHigh; /* Plus Delta */
      prevHigh = tempReal;

      tempReal = inLow[today];
      diffM    = prevLow-tempReal;   /* Minus Delta */
      prevLow  = tempReal;

      prevMinusDM -= prevMinusDM/optInTimePeriod;
      prevPlusDM  -= prevPlusDM/optInTimePeriod;

      if( (diffM > 0) && (diffP < diffM) )
      {
         /* Case 2 and 4: +DM=0,-DM=diffM */
         prevMinusDM += diffM;
      }
      else if( (diffP > 0) && (diffP > diffM) )
      {
         /* Case 1 and 3: +DM=diffP,-DM=0 */
         prevPlusDM += diffP;
      }

      /* Calculate the prevTR */
      TRUE_RANGE(prevHigh,prevLow,prevClose,tempReal);
      prevTR = prevTR - (prevTR/optInTimePeriod) + tempReal;
      prevClose = inClose[today];

      /* Calculate the DX. The value is rounded (see Wilder book). */
      if( !TA_IS_ZERO(prevTR))
      {
         minusDI = round_pos(100.0*(prevMinusDM/prevTR));
         plusDI  = round_pos(100.0*(prevPlusDM/prevTR));
         /* This loop is just to accumulate the initial DX */
         tempReal = minusDI+plusDI;
         if( !TA_IS_ZERO(tempReal))         
            outReal[outIdx] = round_pos( 100.0 * (std_fabs(minusDI-plusDI)/tempReal) );
         else
            outReal[outIdx] = outReal[outIdx-1];
      }
      else
         outReal[outIdx] = outReal[outIdx-1];
      outIdx++;
   }

   *outNBElement = outIdx;

   return TA_SUCCESS;
}
