#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_HT_SINE_Lookback( void )
{
   /*  31 input are skip 
    * +32 output are skip to account for misc lookback
    * ---
    *  63 Total Lookback
    *
    * 31 is for being compatible with Tradestation.
    * See TA_MAMA_Lookback for an explanation of the "32".
    */
   return 63 + TA_GLOBALS_UNSTABLE_PERIOD(TA_FUNC_UNST_HT_SINE,HtSine);
}

/*
 * TA_HT_SINE - Hilbert Transform - SineWave
 * 
 * Input  = double
 * Output = double, double
 * 
 */
TA_RetCode TA_HT_SINE( int    startIdx,
                       int    endIdx,
                       const double inReal[],
                       int          *outBegIdx,
                       int          *outNBElement,
                       double        outSine[],
                       double        outLeadSine[] )
{
	/* insert local variable here */
   int outIdx, i;
   int lookbackTotal, today;
   double tempReal, tempReal2;

   double adjustedPrevPeriod, period;

   /* Variable used for the price smoother (a weighted moving average). */
   int trailingWMAIdx;
   double periodWMASum, periodWMASub, trailingWMAValue;
   double smoothedValue;

   /* Variables used for the Hilbert Transormation */
   const double a = 0.0962;
   const double b = 0.5769;
   double hilbertTempReal;
   int hilbertIdx;

   HILBERT_VARIABLES( detrender );
   HILBERT_VARIABLES( Q1 );
   HILBERT_VARIABLES( jI );
   HILBERT_VARIABLES( jQ );

   double Q2, I2, prevQ2, prevI2, Re, Im;

   double I1ForOddPrev2,  I1ForOddPrev3;
   double I1ForEvenPrev2, I1ForEvenPrev3;

   double rad2Deg, deg2Rad, constDeg2RadBy360;

   double todayValue, smoothPeriod;

   /* Varaible used to keep track of the previous
    * smooth price. In the case of this algorithm,
    * we will never need more than 50 values.
    */
   #define SMOOTH_PRICE_SIZE 50
   CIRCBUF_PROLOG(smoothPrice,double,SMOOTH_PRICE_SIZE);
   int idx;

   /* Variable used to calculate the dominant cycle phase */
   int DCPeriodInt;
   double DCPhase, DCPeriod, imagPart, realPart;

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   if( !inReal ) return TA_BAD_PARAM;
      if( !outSine )
      return TA_BAD_PARAM;

   if( !outLeadSine )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   CIRCBUF_INIT_LOCAL_ONLY(smoothPrice,double);

   /* The following could be replaced by constant eventually. */
   tempReal = std_atan(1);
   rad2Deg = 45.0/tempReal;
   deg2Rad = 1.0/rad2Deg;
   constDeg2RadBy360 = tempReal*8.0;

   /* Identify the minimum number of price bar needed
    * to calculate at least one output.
    */
   lookbackTotal = 63 + TA_GLOBALS_UNSTABLE_PERIOD(TA_FUNC_UNST_HT_SINE,HtSine);

   /* Move up the start index if there is not
    * enough initial data.
    */
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

   /* Initialize the price smoother, which is simply a weighted
    * moving average of the price.
    * To understand this algorithm, I strongly suggest to understand
    * first how TA_WMA is done.
    */
   trailingWMAIdx = startIdx - lookbackTotal;
   today = trailingWMAIdx;

   /* Initialization is same as WMA, except loop is unrolled
    * for speed optimization.
    */
   tempReal = inReal[today++];
   periodWMASub = tempReal;
   periodWMASum = tempReal;
   tempReal = inReal[today++];
   periodWMASub += tempReal;
   periodWMASum += tempReal*2.0;
   tempReal = inReal[today++];
   periodWMASub += tempReal;
   periodWMASum += tempReal*3.0;

   trailingWMAValue = 0.0;

   /* Subsequent WMA value are evaluated by using
    * the DO_PRICE_WMA macro.
    */
   #define DO_PRICE_WMA(varNewPrice,varToStoreSmoothedValue) { \
      periodWMASub     += varNewPrice; \
      periodWMASub     -= trailingWMAValue; \
      periodWMASum     += varNewPrice*4.0; \
      trailingWMAValue  = inReal[trailingWMAIdx++]; \
      varToStoreSmoothedValue = periodWMASum*0.1; \
      periodWMASum -= periodWMASub; \
   }

   i = 34;
   do
   {
      tempReal = inReal[today++];
      DO_PRICE_WMA(tempReal,smoothedValue);
   } while( --i != 0);

   /* Initialize the circular buffers used by the hilbert
    * transform logic. 
    * A buffer is used for odd day and another for even days.
    * This minimize the number of memory access and floating point
    * operations needed (note also that by using static circular buffer, 
    * no large dynamic memory allocation is needed for storing
    * intermediate calculation!).
    */
   hilbertIdx = 0;

   INIT_HILBERT_VARIABLES(detrender);
   INIT_HILBERT_VARIABLES(Q1);
   INIT_HILBERT_VARIABLES(jI);
   INIT_HILBERT_VARIABLES(jQ);

   period = 0.0;
   outIdx = 0;

   prevI2 = prevQ2 = 0.0;
   Re     = Im     = 0.0;
   I1ForOddPrev3 = I1ForEvenPrev3 = 0.0;
   I1ForOddPrev2 = I1ForEvenPrev2 = 0.0;
   smoothPeriod  = 0.0;

   for( i=0; i < SMOOTH_PRICE_SIZE; i++ )
      smoothPrice[i] = 0.0;

   /* The code is speed optimized and is most likely very
    * hard to follow if you do not already know well the
    * original algorithm.
    * To understadn better, it is strongly suggested to look 
    * first at the Excel implementation in "test_MAMA.xls" included
    * in this package.
    */
   DCPhase = 0.0;
   while( today <= endIdx )
   {
      adjustedPrevPeriod = (0.075*period)+0.54;

      todayValue = inReal[today];
      DO_PRICE_WMA(todayValue,smoothedValue);

      /* Remember the smoothedValue into the smoothPrice
       * circular buffer.
       */
      smoothPrice[smoothPrice_Idx] = smoothedValue;

      if( (today%2) == 0 )
      {
         /* Do the Hilbert Transforms for even price bar */
         DO_HILBERT_EVEN(detrender,smoothedValue);
         DO_HILBERT_EVEN(Q1,detrender);
         DO_HILBERT_EVEN(jI,I1ForEvenPrev3);
         DO_HILBERT_EVEN(jQ,Q1);
         if( ++hilbertIdx == 3 )
            hilbertIdx = 0;

         Q2 = (0.2*(Q1 + jI)) + (0.8*prevQ2);
         I2 = (0.2*(I1ForEvenPrev3 - jQ)) + (0.8*prevI2);

         /* The variable I1 is the detrender delayed for
          * 3 price bars. 
          *
          * Save the current detrender value for being
          * used by the "odd" logic later.
          */
         I1ForOddPrev3 = I1ForOddPrev2;
         I1ForOddPrev2 = detrender;      
      }
      else
      {
         /* Do the Hilbert Transforms for odd price bar */
         DO_HILBERT_ODD(detrender,smoothedValue);
         DO_HILBERT_ODD(Q1,detrender);
         DO_HILBERT_ODD(jI,I1ForOddPrev3);
         DO_HILBERT_ODD(jQ,Q1);

         Q2 = (0.2*(Q1 + jI)) + (0.8*prevQ2);
         I2 = (0.2*(I1ForOddPrev3 - jQ)) + (0.8*prevI2);

         /* The varaiable I1 is the detrender delayed for
          * 3 price bars. 
          *
          * Save the current detrender value for being
          * used by the "even" logic later.
          */
         I1ForEvenPrev3 = I1ForEvenPrev2;
         I1ForEvenPrev2 = detrender;
      }

      /* Adjust the period for next price bar */
      Re = (0.2*((I2*prevI2)+(Q2*prevQ2)))+(0.8*Re);
      Im = (0.2*((I2*prevQ2)-(Q2*prevI2)))+(0.8*Im);
      prevQ2 = Q2;
      prevI2 = I2;
      tempReal = period;
      if( (Im != 0.0) && (Re != 0.0) )
         period = 360.0 / (std_atan(Im/Re)*rad2Deg);
      tempReal2 = 1.5*tempReal;
      if( period > tempReal2)
         period = tempReal2;
      tempReal2 = 0.67*tempReal;
      if( period < tempReal2 )
         period = tempReal2;
      if( period < 6 )
         period = 6;
      else if( period > 50 )
         period = 50;
      period = (0.2*period) + (0.8 * tempReal);

      smoothPeriod = (0.33*period)+(0.67*smoothPeriod);

      /* Compute Dominant Cycle Phase */
      DCPeriod    = smoothPeriod+0.5;
      DCPeriodInt = (int)DCPeriod;
      realPart = 0.0;
      imagPart = 0.0;

      /* idx is used to iterate for up to 50 of the last
       * value of smoothPrice.
       */
      idx = smoothPrice_Idx;
      for( i=0; i < DCPeriodInt; i++ )
      {
         tempReal  = ((double)i*constDeg2RadBy360)/(double)DCPeriodInt;
         tempReal2 = smoothPrice[idx];
         realPart += std_sin(tempReal)*tempReal2;
         imagPart += std_cos(tempReal)*tempReal2;
         if( idx == 0 )
            idx = SMOOTH_PRICE_SIZE-1;
         else
            idx--;
      }

      tempReal = std_fabs(imagPart);
      if( tempReal > 0.0 )
         DCPhase = std_atan(realPart/imagPart)*rad2Deg;
      else if( tempReal <= 0.01 )
      {
         if( realPart < 0.0 )
            DCPhase -= 90.0;
         else if( realPart > 0.0 )
            DCPhase += 90.0;
      }
      DCPhase += 90.0;

      /* Compensate for one bar lag of the weighted moving average */
      DCPhase += 360.0 / smoothPeriod;
      if( imagPart < 0.0 )
         DCPhase += 180.0;
      if( DCPhase > 315.0 )
         DCPhase -= 360.0;

      if( today >= startIdx )
      {
         outSine[outIdx]       = std_sin(DCPhase*deg2Rad);
         outLeadSine[outIdx++] = std_sin((DCPhase+45)*deg2Rad);
      }

      /* Ooof... let's do the next price bar now! */
      CIRCBUF_NEXT(smoothPrice);
      today++;
   }

   *outNBElement = outIdx;
 
   return TA_SUCCESS;
}
