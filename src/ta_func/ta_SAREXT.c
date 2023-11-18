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


int TA_SAREXT_Lookback( double        optInStartValue, /* From TA_REAL_MIN to TA_REAL_MAX */
                      double        optInOffsetOnReverse, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationInitLong, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationLong, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationMaxLong, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationInitShort, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationShort, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationMaxShort )  /* From 0 to TA_REAL_MAX */
{
   /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   if( optInStartValue == TA_REAL_DEFAULT )
      optInStartValue = 0.000000e+0;
   else if( (optInStartValue < -3.000000e+37) || (optInStartValue > 3.000000e+37) )
      return -1;

   if( optInOffsetOnReverse == TA_REAL_DEFAULT )
      optInOffsetOnReverse = 0.000000e+0;
   else if( (optInOffsetOnReverse < 0.000000e+0) || (optInOffsetOnReverse > 3.000000e+37) )
      return -1;

   if( optInAccelerationInitLong == TA_REAL_DEFAULT )
      optInAccelerationInitLong = 2.000000e-2;
   else if( (optInAccelerationInitLong < 0.000000e+0) || (optInAccelerationInitLong > 3.000000e+37) )
      return -1;

   if( optInAccelerationLong == TA_REAL_DEFAULT )
      optInAccelerationLong = 2.000000e-2;
   else if( (optInAccelerationLong < 0.000000e+0) || (optInAccelerationLong > 3.000000e+37) )
      return -1;

   if( optInAccelerationMaxLong == TA_REAL_DEFAULT )
      optInAccelerationMaxLong = 2.000000e-1;
   else if( (optInAccelerationMaxLong < 0.000000e+0) || (optInAccelerationMaxLong > 3.000000e+37) )
      return -1;

   if( optInAccelerationInitShort == TA_REAL_DEFAULT )
      optInAccelerationInitShort = 2.000000e-2;
   else if( (optInAccelerationInitShort < 0.000000e+0) || (optInAccelerationInitShort > 3.000000e+37) )
      return -1;

   if( optInAccelerationShort == TA_REAL_DEFAULT )
      optInAccelerationShort = 2.000000e-2;
   else if( (optInAccelerationShort < 0.000000e+0) || (optInAccelerationShort > 3.000000e+37) )
      return -1;

   if( optInAccelerationMaxShort == TA_REAL_DEFAULT )
      optInAccelerationMaxShort = 2.000000e-1;
   else if( (optInAccelerationMaxShort < 0.000000e+0) || (optInAccelerationMaxShort > 3.000000e+37) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */
   (void)optInStartValue;
   (void)optInOffsetOnReverse;
   (void)optInAccelerationInitLong;
   (void)optInAccelerationLong;
   (void)optInAccelerationMaxLong;
   (void)optInAccelerationInitShort;
   (void)optInAccelerationShort;
   (void)optInAccelerationMaxShort;

   /* SAR always sacrifices one price bar to establish the
    * initial extreme price.
    */

   return 1;
}

/*
 * TA_SAREXT - Parabolic SAR - Extended
 * 
 * Input  = High, Low
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInStartValue:(From TA_REAL_MIN to TA_REAL_MAX)
 *    Start value and direction. 0 for Auto, >0 for Long, <0 for Short
 * 
 * optInOffsetOnReverse:(From 0 to TA_REAL_MAX)
 *    Percent offset added/removed to initial stop on short/long reversal
 * 
 * optInAccelerationInitLong:(From 0 to TA_REAL_MAX)
 *    Acceleration Factor initial value for the Long direction
 * 
 * optInAccelerationLong:(From 0 to TA_REAL_MAX)
 *    Acceleration Factor for the Long direction
 * 
 * optInAccelerationMaxLong:(From 0 to TA_REAL_MAX)
 *    Acceleration Factor maximum value for the Long direction
 * 
 * optInAccelerationInitShort:(From 0 to TA_REAL_MAX)
 *    Acceleration Factor initial value for the Short direction
 * 
 * optInAccelerationShort:(From 0 to TA_REAL_MAX)
 *    Acceleration Factor for the Short direction
 * 
 * optInAccelerationMaxShort:(From 0 to TA_REAL_MAX)
 *    Acceleration Factor maximum value for the Short direction
 * 
 * 
 */
TA_RetCode TA_SAREXT( int    startIdx,
                      int    endIdx,
                      const double inHigh[],
                      const double inLow[],
                      double        optInStartValue, /* From TA_REAL_MIN to TA_REAL_MAX */
                      double        optInOffsetOnReverse, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationInitLong, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationLong, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationMaxLong, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationInitShort, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationShort, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationMaxShort, /* From 0 to TA_REAL_MAX */
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
   double afLong, afShort, ep, sar;
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

      if( optInStartValue == TA_REAL_DEFAULT )
      optInStartValue = 0.000000e+0;
   else if( (optInStartValue < -3.000000e+37) || (optInStartValue > 3.000000e+37) )
      return TA_BAD_PARAM;

   if( optInOffsetOnReverse == TA_REAL_DEFAULT )
      optInOffsetOnReverse = 0.000000e+0;
   else if( (optInOffsetOnReverse < 0.000000e+0) || (optInOffsetOnReverse > 3.000000e+37) )
      return TA_BAD_PARAM;

   if( optInAccelerationInitLong == TA_REAL_DEFAULT )
      optInAccelerationInitLong = 2.000000e-2;
   else if( (optInAccelerationInitLong < 0.000000e+0) || (optInAccelerationInitLong > 3.000000e+37) )
      return TA_BAD_PARAM;

   if( optInAccelerationLong == TA_REAL_DEFAULT )
      optInAccelerationLong = 2.000000e-2;
   else if( (optInAccelerationLong < 0.000000e+0) || (optInAccelerationLong > 3.000000e+37) )
      return TA_BAD_PARAM;

   if( optInAccelerationMaxLong == TA_REAL_DEFAULT )
      optInAccelerationMaxLong = 2.000000e-1;
   else if( (optInAccelerationMaxLong < 0.000000e+0) || (optInAccelerationMaxLong > 3.000000e+37) )
      return TA_BAD_PARAM;

   if( optInAccelerationInitShort == TA_REAL_DEFAULT )
      optInAccelerationInitShort = 2.000000e-2;
   else if( (optInAccelerationInitShort < 0.000000e+0) || (optInAccelerationInitShort > 3.000000e+37) )
      return TA_BAD_PARAM;

   if( optInAccelerationShort == TA_REAL_DEFAULT )
      optInAccelerationShort = 2.000000e-2;
   else if( (optInAccelerationShort < 0.000000e+0) || (optInAccelerationShort > 3.000000e+37) )
      return TA_BAD_PARAM;

   if( optInAccelerationMaxShort == TA_REAL_DEFAULT )
      optInAccelerationMaxShort = 2.000000e-1;
   else if( (optInAccelerationMaxShort < 0.000000e+0) || (optInAccelerationMaxShort > 3.000000e+37) )
      return TA_BAD_PARAM;

   if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* This function is the same as TA_SAR, except that the caller has
    * greater control on the SAR dynamic and initial state.
    *
    * In additon, the TA_SAREXT returns negative values when the position
    * is short. This allow to distinguish when the SAR do actually reverse.
    */

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
    * In TA-Lib, the following default logic is used:
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
    *
    *
    * Can I force the initial SAR?
    * ============================
    * Yes. Using the optInStartValue_0 parameter:
    *  optInStartValue_0 >  0 : SAR is long at optInStartValue_0.
    *  optInStartValue_0 <  0 : SAR is short at fabs(optInStartValue_0).
    *  
    * And when optInStartValue_0 == 0, the logic is the same as for TA_SAR
    * (See previous two sections).
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


   /* Check if the acceleration factors are being defined by the user.
    * Make sure the acceleration and maximum are coherent.
    * If not, correct the acceleration.    
    * Default afLong = 0.02
    * Default afShort = 0.02
    */

   afLong = optInAccelerationInitLong;
   afShort = optInAccelerationInitShort;

   if( afLong > optInAccelerationMaxLong )
      afLong = optInAccelerationInitLong = optInAccelerationMaxLong;

   if( optInAccelerationLong > optInAccelerationMaxLong )
      optInAccelerationLong = optInAccelerationMaxLong;

   if( afShort > optInAccelerationMaxShort) 
      afShort = optInAccelerationInitShort = optInAccelerationMaxShort;

   if( optInAccelerationShort > optInAccelerationMaxShort )
      optInAccelerationShort = optInAccelerationMaxShort;      

   /* Initialise SAR calculations */

   if(optInStartValue == 0) /* Default action */
   {
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
   }
   else if( optInStartValue > 0 ) /* Start Long */
   {
      isLong = 1;
   }
   else /* optInStartValue_0 < 0 => Start Short */
   {
      isLong = 0;
   }

   *outBegIdx = startIdx;
   outIdx = 0;


   /* Write the first SAR. */
   todayIdx = startIdx;

   newHigh = inHigh[todayIdx-1];
   newLow  = inLow[todayIdx-1];

   SAR_ROUNDING(newHigh);
   SAR_ROUNDING(newLow);

   if(optInStartValue == 0) /* Default action */
   {
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
   }
   else if ( optInStartValue > 0 ) /* Start Long at specified value. */
   {
      ep  = inHigh[todayIdx];
      sar = optInStartValue;
   }
   else /* if optInStartValue < 0 => Start Short at specified value. */
   {
      ep  = inLow[todayIdx];
      sar = std_fabs(optInStartValue);
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
            if( optInOffsetOnReverse != 0.0 )
               sar += sar * optInOffsetOnReverse; 
            outReal[outIdx++] = -sar;

            /* Adjust afShort and ep */
            afShort = optInAccelerationInitShort;
            ep = newLow;
 
            /* Calculate the new SAR */
            sar = sar + afShort * (ep - sar);
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
 
            /* Adjust afLong and ep. */
            if( newHigh > ep )
            {
               ep = newHigh;
               afLong += optInAccelerationLong;
               if( afLong > optInAccelerationMaxLong )
                  afLong = optInAccelerationMaxLong;
            }

            /* Calculate the new SAR */
            sar = sar + afLong * (ep - sar);
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
            if( optInOffsetOnReverse != 0.0 )
               sar -= sar * optInOffsetOnReverse;
            outReal[outIdx++] = sar;

            /* Adjust afLong and ep */
            afLong = optInAccelerationInitLong;
            ep = newHigh;

            /* Calculate the new SAR */
            sar = sar + afLong * (ep - sar);
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
            outReal[outIdx++] = -sar;

            /* Adjust afShort and ep. */
            if( newLow < ep )
            {
               ep = newLow;
               afShort += optInAccelerationShort;
               if( afShort > optInAccelerationMaxShort )
                  afShort = optInAccelerationMaxShort;
            }

            /* Calculate the new SAR */
            sar = sar + afShort * (ep - sar);
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
