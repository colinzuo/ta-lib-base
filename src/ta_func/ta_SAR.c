/* SAR_ROUNDING is just for test purpose when cross-referencing that
 * function with example from Wilder's book. Wilder is using two
 * decimal rounding for simplification. TA-Lib does not round.
 */
/* #define SAR_ROUNDING(x) x=round_pos_2(x) */
#define SAR_ROUNDING(x)

#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_SAR_Lookback( double        optInAcceleration, /* From 0 to TA_REAL_MAX */
                   double        optInMaximum )  /* From 0 to TA_REAL_MAX */
{
   /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   if( optInAcceleration == TA_REAL_DEFAULT )
      optInAcceleration = 2.000000e-2;
   else if( (optInAcceleration < 0.000000e+0) || (optInAcceleration > 3.000000e+37) )
      return -1;

   if( optInMaximum == TA_REAL_DEFAULT )
      optInMaximum = 2.000000e-1;
   else if( (optInMaximum < 0.000000e+0) || (optInMaximum > 3.000000e+37) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */
   (void)optInAcceleration;
   (void)optInMaximum;

   /* SAR always sacrify one price bar to establish the
    * initial extreme price.
    */
   return 1;
}

/*
 * TA_SAR - Parabolic SAR
 * 
 * Input  = High, Low
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInAcceleration:(From 0 to TA_REAL_MAX)
 *    Acceleration Factor used up to the Maximum value
 * 
 * optInMaximum:(From 0 to TA_REAL_MAX)
 *    Acceleration Factor Maximum value
 * 
 * 
 */
TA_RetCode TA_SAR( int    startIdx,
                   int    endIdx,
                   const double inHigh[],
                   const double inLow[],
                   double        optInAcceleration, /* From 0 to TA_REAL_MAX */
                   double        optInMaximum, /* From 0 to TA_REAL_MAX */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] )
{
	/* insert local variable here */
   TA_RetCode retCode;

   int isLong; /* > 0 indicates long. == 0 indicates short */
   int todayIdx, outIdx;

   int tempInt;

   double newHigh, newLow, prevHigh, prevLow;
   double af, ep, sar;
   ARRAY_LOCAL(ep_temp,1);

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   /* Verify required price component. */
   if(!inHigh||!inLow)
      return TA_BAD_PARAM;

      if( optInAcceleration == TA_REAL_DEFAULT )
      optInAcceleration = 2.000000e-2;
   else if( (optInAcceleration < 0.000000e+0) || (optInAcceleration > 3.000000e+37) )
      return TA_BAD_PARAM;

   if( optInMaximum == TA_REAL_DEFAULT )
      optInMaximum = 2.000000e-1;
   else if( (optInMaximum < 0.000000e+0) || (optInMaximum > 3.000000e+37) )
      return TA_BAD_PARAM;

   if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* Implementation of the SAR has been a little bit open to interpretation
    * since Wilder (the original author) did not define a precise algorithm
    * on how to bootstrap the algorithm. Take any existing software application
    * and you will see slight variation on how the algorithm was adapted.
    *
    * What is the initial trade direction? Long or short?
    * ===================================================
    * The interpretation of what should be the initial SAR values is
    * open to interpretation, particularly since the caller to the function
    * does not specify the initial direction of the trade.
    *
    * In TA-Lib, the following logic is used:
    *  - Calculate +DM and -DM between the first and
    *    second bar. The highest directional indication will
    *    indicate the assumed direction of the trade for the second
    *    price bar. 
    *  - In the case of a tie between +DM and -DM,
    *    the direction is LONG by default.
    *
    * What is the initial "extreme point" and thus SAR?
    * =================================================
    * The following shows how different people took different approach:
    *  - Metastock use the first price bar high/low depending of
    *    the direction. No SAR is calculated for the first price
    *    bar.
    *  - Tradestation use the closing price of the second bar. No
    *    SAR are calculated for the first price bar.
    *  - Wilder (the original author) use the SIP from the
    *    previous trade (cannot be implement here since the
    *    direction and length of the previous trade is unknonw).
    *  - The Magazine TASC seems to follow Wilder approach which
    *    is not practical here.
    *
    * TA-Lib "consume" the first price bar and use its high/low as the
    * initial SAR of the second price bar. I found that approach to be
    * the closest to Wilders idea of having the first entry day use
    * the previous extreme point, except that here the extreme point is
    * derived solely from the first price bar. I found the same approach
    * to be used by Metastock.
    */

   /* Identify the minimum number of price bar needed
    * to calculate at least one output.
    *
    * Move up the start index if there is not
    * enough initial data.
    */
   if( startIdx < 1 )
      startIdx = 1;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
   {
      *outBegIdx = 0;
      *outNBElement = 0;
      return TA_SUCCESS;
   }   

   /* Make sure the acceleration and maximum are coherent.
    * If not, correct the acceleration.
    */
   af = optInAcceleration;
   if( af > optInMaximum )
      af = optInAcceleration = optInMaximum;

   /* Identify if the initial direction is long or short.
    * (ep is just used as a temp buffer here, the name
    *  of the parameter is not significant).
    */
   retCode = TA_MINUS_DM( startIdx, startIdx, inHigh, inLow, 1,
                                      &tempInt, &tempInt,
								      ep_temp );
   if( ep_temp[0] > 0 )
      isLong = 0;
   else
      isLong = 1;

   if( retCode != TA_SUCCESS )
   {
      *outBegIdx = 0;
      *outNBElement = 0;
      return retCode;
   }

   *outBegIdx = startIdx;
   outIdx = 0;

   /* Write the first SAR. */
   todayIdx = startIdx;

   newHigh = inHigh[todayIdx-1];
   newLow  = inLow[todayIdx-1];

   SAR_ROUNDING(newHigh);
   SAR_ROUNDING(newLow);

   if( isLong == 1 )
   {
      ep  = inHigh[todayIdx];
      sar = newLow;
   }
   else
   {
      ep  = inLow[todayIdx];
      sar = newHigh;
   }

   SAR_ROUNDING(sar);

   /* Cheat on the newLow and newHigh for the
    * first iteration.
    */
   newLow  = inLow[todayIdx];
   newHigh = inHigh[todayIdx];

   while( todayIdx <= endIdx )
   {
      prevLow  = newLow;
      prevHigh = newHigh;
      newLow  = inLow[todayIdx];
      newHigh = inHigh[todayIdx];   
      todayIdx++;

      SAR_ROUNDING(newLow);
      SAR_ROUNDING(newHigh);

      if( isLong == 1 )
      {  
         /* Switch to short if the low penetrates the SAR value. */
         if( newLow <= sar )
         {
            /* Switch and Overide the SAR with the ep */
            isLong = 0;
            sar = ep;

            /* Make sure the overide SAR is within
             * yesterday's and today's range.
             */
            if( sar < prevHigh )
               sar = prevHigh;            
            if( sar < newHigh )
               sar = newHigh;

            /* Output the overide SAR  */
            outReal[outIdx++] = sar;

            /* Adjust af and ep */
            af = optInAcceleration;
            ep = newLow;
 
            /* Calculate the new SAR */
            sar = sar + af * (ep - sar);
            SAR_ROUNDING( sar );

            /* Make sure the new SAR is within
             * yesterday's and today's range.
             */
            if( sar < prevHigh )
               sar = prevHigh;            
            if( sar < newHigh )
               sar = newHigh;
         }
         else
         {
            /* No switch */

            /* Output the SAR (was calculated in the previous iteration) */
            outReal[outIdx++] = sar;
 
            /* Adjust af and ep. */
            if( newHigh > ep )
            {
               ep = newHigh;
               af += optInAcceleration;
               if( af > optInMaximum )
                  af = optInMaximum;
            }

            /* Calculate the new SAR */
            sar = sar + af * (ep - sar);
            SAR_ROUNDING( sar );

            /* Make sure the new SAR is within
             * yesterday's and today's range.
             */
            if( sar > prevLow )
               sar = prevLow;            
            if( sar > newLow )
               sar = newLow;
         }
      }
      else
      {
         /* Switch to long if the high penetrates the SAR value. */
         if( newHigh >= sar )
         {
            /* Switch and Overide the SAR with the ep */
            isLong = 1;
            sar = ep;

            /* Make sure the overide SAR is within
             * yesterday's and today's range.
             */
            if( sar > prevLow )
               sar = prevLow;            
            if( sar > newLow )
               sar = newLow;

            /* Output the overide SAR  */
            outReal[outIdx++] = sar;

            /* Adjust af and ep */
            af = optInAcceleration;
            ep = newHigh;

            /* Calculate the new SAR */
            sar = sar + af * (ep - sar);
            SAR_ROUNDING( sar );

            /* Make sure the new SAR is within
             * yesterday's and today's range.
             */
            if( sar > prevLow )
               sar = prevLow;            
            if( sar > newLow )
               sar = newLow;
         }
         else
         {
            /* No switch */

            /* Output the SAR (was calculated in the previous iteration) */
            outReal[outIdx++] = sar;

            /* Adjust af and ep. */
            if( newLow < ep )
            {
               ep = newLow;
               af += optInAcceleration;
               if( af > optInMaximum )
                  af = optInMaximum;
            }

            /* Calculate the new SAR */
            sar = sar + af * (ep - sar);
            SAR_ROUNDING( sar );

            /* Make sure the new SAR is within
             * yesterday's and today's range.
             */
            if( sar < prevHigh )
               sar = prevHigh;            
            if( sar < newHigh )
               sar = newHigh;
         }
      }
   }

   *outNBElement = outIdx;

   return TA_SUCCESS;
}
