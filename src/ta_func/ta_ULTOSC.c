#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_ULTOSC_Lookback( int           optInTimePeriod1, /* From 1 to 100000 */
                      int           optInTimePeriod2, /* From 1 to 100000 */
                      int           optInTimePeriod3 )  /* From 1 to 100000 */
{   
   /* insert local variable here */
   int maxPeriod;

#ifndef TA_FUNC_NO_RANGE_CHECK
   /* min/max are checked for optInTimePeriod1. */
   if( (int)optInTimePeriod1 == TA_INTEGER_DEFAULT )
      optInTimePeriod1 = 7;
   else if( ((int)optInTimePeriod1 < 1) || ((int)optInTimePeriod1 > 100000) )
      return -1;

   /* min/max are checked for optInTimePeriod2. */
   if( (int)optInTimePeriod2 == TA_INTEGER_DEFAULT )
      optInTimePeriod2 = 14;
   else if( ((int)optInTimePeriod2 < 1) || ((int)optInTimePeriod2 > 100000) )
      return -1;

   /* min/max are checked for optInTimePeriod3. */
   if( (int)optInTimePeriod3 == TA_INTEGER_DEFAULT )
      optInTimePeriod3 = 28;
   else if( ((int)optInTimePeriod3 < 1) || ((int)optInTimePeriod3 > 100000) )
      return -1;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* Lookback for the Ultimate Oscillator is the lookback of the SMA with the longest
    * time period, plus 1 for the True Range.
    */
    maxPeriod = max( max(optInTimePeriod1, optInTimePeriod2), optInTimePeriod3);
    return TA_SMA_Lookback( maxPeriod ) + 1;
}

/*
 * TA_ULTOSC - Ultimate Oscillator
 * 
 * Input  = High, Low, Close
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod1:(From 1 to 100000)
 *    Number of bars for 1st period.
 * 
 * optInTimePeriod2:(From 1 to 100000)
 *    Number of bars fro 2nd period
 * 
 * optInTimePeriod3:(From 1 to 100000)
 *    Number of bars for 3rd period
 * 
 * 
 */
TA_RetCode TA_ULTOSC( int    startIdx,
                      int    endIdx,
                      const double inHigh[],
                      const double inLow[],
                      const double inClose[],
                      int           optInTimePeriod1, /* From 1 to 100000 */
                      int           optInTimePeriod2, /* From 1 to 100000 */
                      int           optInTimePeriod3, /* From 1 to 100000 */
                      int          *outBegIdx,
                      int          *outNBElement,
                      double        outReal[] )
{
   /* insert local variable here */
   double a1Total, a2Total, a3Total;
   double b1Total, b2Total, b3Total;
   double trueLow, trueRange, closeMinusTrueLow;
   double tempDouble, output, tempHT, tempLT, tempCY;
   int lookbackTotal;
   int longestPeriod, longestIndex;
   int i,j,today,outIdx;
   int trailingIdx1, trailingIdx2, trailingIdx3;

   ARRAY_INT_LOCAL(usedFlag,3);
   ARRAY_INT_LOCAL(periods,3);
   ARRAY_INT_LOCAL(sortedPeriods,3);

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   /* Verify required price component. */
   if(!inHigh||!inLow||!inClose)
      return TA_BAD_PARAM;

      /* min/max are checked for optInTimePeriod1. */
   if( (int)optInTimePeriod1 == TA_INTEGER_DEFAULT )
      optInTimePeriod1 = 7;
   else if( ((int)optInTimePeriod1 < 1) || ((int)optInTimePeriod1 > 100000) )
      return TA_BAD_PARAM;

   /* min/max are checked for optInTimePeriod2. */
   if( (int)optInTimePeriod2 == TA_INTEGER_DEFAULT )
      optInTimePeriod2 = 14;
   else if( ((int)optInTimePeriod2 < 1) || ((int)optInTimePeriod2 > 100000) )
      return TA_BAD_PARAM;

   /* min/max are checked for optInTimePeriod3. */
   if( (int)optInTimePeriod3 == TA_INTEGER_DEFAULT )
      optInTimePeriod3 = 28;
   else if( ((int)optInTimePeriod3 < 1) || ((int)optInTimePeriod3 > 100000) )
      return TA_BAD_PARAM;

   if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   *outBegIdx = 0;
   *outNBElement = 0;

   /* Ensure that the time periods are ordered from shortest to longest.
    * Sort. */
   periods[0] = optInTimePeriod1;
   periods[1] = optInTimePeriod2;
   periods[2] = optInTimePeriod3;
   usedFlag[0] = 0;
   usedFlag[1] = 0;
   usedFlag[2] = 0;
   for ( i = 0; i < 3; ++i ) 
   {
      longestPeriod = 0;
      longestIndex = 0;
      for ( j = 0; j < 3; ++j ) 
      {
         if ( (usedFlag[j] == 0) && (periods[j] > longestPeriod) ) 
         {
            longestPeriod = periods[j];
            longestIndex = j;
          }
      }
      usedFlag[longestIndex] = 1;
      sortedPeriods[i] = longestPeriod;
   }
   optInTimePeriod1 = sortedPeriods[2];
   optInTimePeriod2 = sortedPeriods[1];
   optInTimePeriod3 = sortedPeriods[0];

   /* Adjust startIdx for lookback period. */
   lookbackTotal = TA_ULTOSC_Lookback( optInTimePeriod1, optInTimePeriod2, optInTimePeriod3 );
   if( startIdx < lookbackTotal ) startIdx = lookbackTotal;
   
   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx ) return TA_SUCCESS;
   
   /* Prime running totals used in moving averages */
   #define CALC_TERMS(day)                        \
   {                                              \
      tempLT = inLow[day];                        \
      tempHT = inHigh[day];                       \
      tempCY = inClose[day-1];                    \
      trueLow = min( tempLT, tempCY );            \
      closeMinusTrueLow = inClose[day] - trueLow; \
      trueRange = tempHT - tempLT;                \
      tempDouble = std_fabs( tempCY - tempHT );       \
      if( tempDouble > trueRange )                 \
         trueRange = tempDouble;                  \
      tempDouble = std_fabs( tempCY - tempLT  );      \
      if( tempDouble > trueRange )                 \
         trueRange = tempDouble;                  \
   }

   #define PRIME_TOTALS(aTotal, bTotal, period)                 \
   {                                                            \
      aTotal = 0;                                               \
      bTotal = 0;                                               \
      for ( i = startIdx-period+1; i < startIdx; ++i )          \
      {                                                         \
         CALC_TERMS(i);                                         \
         aTotal += closeMinusTrueLow;                           \
         bTotal += trueRange;                                   \
      }                                                         \
   }

   PRIME_TOTALS(a1Total, b1Total, optInTimePeriod1);
   PRIME_TOTALS(a2Total, b2Total, optInTimePeriod2);
   PRIME_TOTALS(a3Total, b3Total, optInTimePeriod3);

   #undef PRIME_TOTALS

   /* Calculate oscillator */
   today = startIdx;
   outIdx = 0;
   trailingIdx1 = today - optInTimePeriod1 + 1;
   trailingIdx2 = today - optInTimePeriod2 + 1;
   trailingIdx3 = today - optInTimePeriod3 + 1;
   while( today <= endIdx )
   { 
      /* Add on today's terms */
      CALC_TERMS(today);
      a1Total += closeMinusTrueLow;
      a2Total += closeMinusTrueLow;
      a3Total += closeMinusTrueLow;
      b1Total += trueRange;
      b2Total += trueRange;
      b3Total += trueRange;
      
      /* Calculate the oscillator value for today */
      output = 0.0; 
      
      if( !TA_IS_ZERO(b1Total) ) output += 4.0*(a1Total/b1Total);
      if( !TA_IS_ZERO(b2Total) ) output += 2.0*(a2Total/b2Total);
      if( !TA_IS_ZERO(b3Total) ) output += a3Total/b3Total; 
                  
      /* Remove the trailing terms to prepare for next day */
      CALC_TERMS(trailingIdx1);
      a1Total -= closeMinusTrueLow;
      b1Total -= trueRange;

      CALC_TERMS(trailingIdx2);
      a2Total -= closeMinusTrueLow;
      b2Total -= trueRange;
      
      CALC_TERMS(trailingIdx3);
      a3Total -= closeMinusTrueLow;
      b3Total -= trueRange;
      
      /* Last operation is to write the output. Must
       * be done after the trailing index have all been
       * taken care of because the caller is allowed
       * to have the input array to be also the output 
       * array.
       */
      outReal[outIdx] = 100.0 * (output / 7.0);

      /* Increment indexes */
      outIdx++;
      today++; 
      trailingIdx1++; 
      trailingIdx2++; 
      trailingIdx3++;
   }
   #undef CALC_TERMS
   
   /* All done. Indicate the output limits and return. */
   *outNBElement = outIdx;
   *outBegIdx    = startIdx;

   return TA_SUCCESS;
}
