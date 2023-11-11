#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_TRIMA_Lookback( int           optInTimePeriod )  /* From 2 to 100000 */
{
   /* insert local variable here */

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 30;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* insert lookback code here. */
   return optInTimePeriod-1;
}

/*
 * TA_TRIMA - Triangular Moving Average
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_TRIMA( int    startIdx,
                     int    endIdx,
                     const double inReal[],
                     int           optInTimePeriod, /* From 2 to 100000 */
                     int          *outBegIdx,
                     int          *outNBElement,
                     double        outReal[] )
{
	/* insert local variable here */
   int lookbackTotal;

   double numerator;
   double numeratorSub;
   double numeratorAdd;

   int i, outIdx, todayIdx, trailingIdx, middleIdx;
   double factor, tempReal;

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   if( !inReal ) return TA_BAD_PARAM;
      /* min/max are checked for optInTimePeriod. */
   if( (int)optInTimePeriod == TA_INTEGER_DEFAULT )
      optInTimePeriod = 30;
   else if( ((int)optInTimePeriod < 2) || ((int)optInTimePeriod > 100000) )
      return TA_BAD_PARAM;

   if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Insert TA function code here. */

   /* Identify the minimum number of price bar needed
    * to calculate at least one output.
    */
   lookbackTotal = (optInTimePeriod-1);

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

   /* TRIMA Description
    * =================
    * The triangular MA is a weighted moving average. Instead of the
    * TA_WMA who put more weigth on the latest price bar, the triangular
    * put more weigth on the data in the middle of the specified period.
    *
    * Examples:
    *   For TimeSerie={a,b,c,d,e,f...} ('a' is the older price)
    *
    *   1st value for TRIMA 4-Period is:  ((1*a)+(2*b)+(2*c)+(1*d)) / 6
    *   2nd value for TRIMA 4-Period is:  ((1*b)+(2*c)+(2*d)+(1*e)) / 6
    *
    *   1st value for TRIMA 5-Period is:  ((1*a)+(2*b)+(3*c)+(2*d)+(1*e)) / 9
    *   2nd value for TRIMA 5-Period is:  ((1*b)+(2*c)+(3*d)+(2*e)+(1*f)) / 9
    *
    * Generally Accepted Implementation
    * ==================================
    * Using algebra, it can be demonstrated that the TRIMA is equivalent to
    * doing a SMA of a SMA. The following explain the rules:
    *
    *  (1) When the period is even, TRIMA(x,period)=SMA(SMA(x,period/2),(period/2)+1)
    *  (2) When the period is odd,  TRIMA(x,period)=SMA(SMA(x,(period+1)/2),(period+1)/2)
    *
    * In other word:
    *  (1) A period of 4 becomes TRIMA(x,4) = SMA( SMA( x, 2), 3 )
    *  (2) A period of 5 becomes TRIMA(x,5) = SMA( SMA( x, 3), 3 )
    *
    * The SMA of a SMA is the algorithm generaly found in books.
    *
    * Tradestation Implementation
    * ===========================
    * Tradestation deviate from the generally accepted implementation by
    * making the TRIMA to be as follow:
    *    TRIMA(x,period) = SMA( SMA( x, (int)(period/2)+1), (int)(period/2)+1 );
    * This formula is done regardless if the period is even or odd.
    *
    * In other word:
    *  (1) A period of 4 becomes TRIMA(x,4) = SMA( SMA( x, 3), 3 )
    *  (2) A period of 5 becomes TRIMA(x,5) = SMA( SMA( x, 3), 3 )
    *  (3) A period of 6 becomes TRIMA(x,5) = SMA( SMA( x, 4), 4 )
    *  (4) A period of 7 becomes TRIMA(x,5) = SMA( SMA( x, 4), 4 )
    *
    * It is not clear to me if the Tradestation approach is a bug or a deliberate
    * decision to do things differently.
    *    
    * Metastock Implementation
    * ========================
    * Output is the same as the generally accepted implementation.
    *
    * TA-Lib Implementation
    * =====================
    * Output is also the same as the generally accepted implementation.
    *
    * For speed optimization and avoid memory allocation, TA-Lib use
    * a better algorithm than the usual SMA of a SMA.
    *
    * The calculation from one TRIMA value to the next is done by doing 4
    * little adjustment (the following show a TRIMA 4-period):
    *
    * TRIMA at time 'd': ((1*a)+(2*b)+(2*c)+(1*d)) / 6
    * TRIMA at time 'e': ((1*b)+(2*c)+(2*d)+(1*e)) / 6
    * 
    * To go from TRIMA 'd' to 'e', the following is done:
    *       1) 'a' and 'b' are substract from the numerator.
    *       2) 'd' is added to the numerator.
    *       3) 'e' is added to the numerator.
    *       4) Calculate TRIMA by doing numerator / 6
    *       5) Repeat sequence for next output
    *
    * These operations are the same steps done by TA-LIB:
    *       1) is done by numeratorSub
    *       2) is done by numeratorAdd.
    *       3) is obtain from the latest input
    *       4) Calculate and write TRIMA in the output
    *       5) Repeat for next output.
    *
    * Of course, numerotrAdd and numeratorSub needs to be
    * adjusted for each iteration.
    *
    * The update of numeratorSub needs values from the input at
    * the trailingIdx and middleIdx position.
    *
    * The update of numeratorAdd needs values from the input at
    * the middleIdx and todayIdx.
    */

   outIdx = 0;

   if( (optInTimePeriod % 2) == 1 )
   {
      /* Logic for Odd period */

      /* Calculate the factor which is 1 divided by the
       * sumation of the weight.
       *
       * The sum of the weight is calculated as follow:
       *
       * The simple sumation serie 1+2+3... n can be
       * express as n(n+1)/2
       *
       * From this logic, a "triangular" sumation formula
       * can be found depending if the period is odd or even.
       *
       * Odd Period Formula:
       *  period = 5 and with n=(int)(period/2)
       *  the formula for a "triangular" serie is:
       *    1+2+3+2+1 = (n*(n+1))+n+1
       *              = (n+1)*(n+1)
       *              = 3 * 3 = 9
       *
       * Even period Formula:
       *   period = 6 and with n=(int)(period/2)
       *   the formula for a "triangular" serie is:
       *    1+2+3+3+2+1 = n*(n+1)
       *                = 3 * 4 = 12
       */

      /* Note: entirely done with int and becomes double only 
       *       on assignement to the factor variable.
       */
      i = (optInTimePeriod>>1);
      factor = (i+1)*(i+1); 
      factor = 1.0/factor;

      /* Initialize all the variable before
       * starting to iterate for each output.
       */
      trailingIdx = startIdx-lookbackTotal;
      middleIdx   = trailingIdx + i;
      todayIdx    = middleIdx + i;
      numerator   = 0.0;
      numeratorSub = 0.0;
      for( i=middleIdx; i >= trailingIdx; i-- )
      {
         tempReal      = inReal[i];
         numeratorSub += tempReal;
         numerator    += numeratorSub;
      }
      numeratorAdd = 0.0;
      middleIdx++;
      for( i=middleIdx; i <= todayIdx; i++ )
      {
         tempReal      = inReal[i];
         numeratorAdd += tempReal;
         numerator    += numeratorAdd;
      }

      /* Write the first output */
      outIdx = 0;
      tempReal = inReal[trailingIdx++]; 
      outReal[outIdx++] = numerator * factor;
      todayIdx++;

      /* Note: The value at the trailingIdx was saved
       *       in tempReal to account for the case where
       *       outReal and inReal are ptr on the same
       *       buffer.
       */

      /* Iterate for remaining output */
      while( todayIdx <= endIdx )
      {
         /* Step (1) */
         numerator    -= numeratorSub;
         numeratorSub -= tempReal;
         tempReal      = inReal[middleIdx++];
         numeratorSub += tempReal;

         /* Step (2) */
         numerator    += numeratorAdd;
         numeratorAdd -= tempReal;
         tempReal      = inReal[todayIdx++];
         numeratorAdd += tempReal;

         /* Step (3) */
         numerator    += tempReal;

         /* Step (4) */
         tempReal = inReal[trailingIdx++]; 
         outReal[outIdx++] = numerator * factor;
      }
   }
   else
   {
      /* Even logic. 
       *
       * Very similar to the odd logic, except:
       *  - calculation of the factor is different.
       *  - the coverage of the numeratorSub and numeratorAdd is
       *    slightly different.
       *  - Adjustment of numeratorAdd is different. See Step (2).
       */
      i = (optInTimePeriod>>1);
      factor = i*(i+1); 
      factor = 1.0/factor;

      /* Initialize all the variable before
       * starting to iterate for each output.
       */
      trailingIdx = startIdx-lookbackTotal;
      middleIdx   = trailingIdx + i - 1;
      todayIdx    = middleIdx + i;
      numerator   = 0.0;

      numeratorSub = 0.0;

      for( i=middleIdx; i >= trailingIdx; i-- )
      {
         tempReal      = inReal[i];
         numeratorSub += tempReal;
         numerator    += numeratorSub;
      }
      numeratorAdd = 0.0;
      middleIdx++;
      for( i=middleIdx; i <= todayIdx; i++ )
      {
         tempReal      = inReal[i];
         numeratorAdd += tempReal;
         numerator    += numeratorAdd;
      }

      /* Write the first output */
      outIdx = 0;
      tempReal = inReal[trailingIdx++]; 
      outReal[outIdx++] = numerator * factor;
      todayIdx++;

      /* Note: The value at the trailingIdx was saved
       *       in tempReal to account for the case where
       *       outReal and inReal are ptr on the same
       *       buffer.
       */

      /* Iterate for remaining output */
      while( todayIdx <= endIdx )
      {
         /* Step (1) */
         numerator    -= numeratorSub;
         numeratorSub -= tempReal;
         tempReal      = inReal[middleIdx++];
         numeratorSub += tempReal;

         /* Step (2) */
         numeratorAdd -= tempReal;
         numerator    += numeratorAdd;
         tempReal      = inReal[todayIdx++];
         numeratorAdd += tempReal;

         /* Step (3) */
         numerator    += tempReal;

         /* Step (4) */
         tempReal = inReal[trailingIdx++]; 
         outReal[outIdx++] = numerator * factor;
      }

   }
   
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
