#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_TRANGE_Lookback( void )
{
   return 1;
}

/*
 * TA_TRANGE - True Range
 * 
 * Input  = High, Low, Close
 * Output = double
 * 
 */
TA_RetCode TA_TRANGE( int    startIdx,
                      int    endIdx,
                      const double inHigh[],
                      const double inLow[],
                      const double inClose[],
                      int          *outBegIdx,
                      int          *outNBElement,
                      double        outReal[] )
{
   /* Insert local variables here. */
   int today, outIdx;
   double val2, val3, greatest;
   double tempCY, tempLT, tempHT;

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   /* Verify required price component. */
   if(!inHigh||!inLow||!inClose)
      return TA_BAD_PARAM;

      if( !outReal )
      return TA_BAD_PARAM;

#endif /* TA_FUNC_NO_RANGE_CHECK */

   /* True Range is the greatest of the following: 
    *
    *  val1 = distance from today's high to today's low.
    *  val2 = distance from yesterday's close to today's high.
    *  val3 = distance from yesterday's close to today's low.   
    *
    * Some books and software makes the first TR value to be
    * the (high - low) of the first bar. This function instead
    * ignore the first price bar, and only output starting at the
    * second price bar are valid. This is done for avoiding
    * inconsistency.
    */

   /* Move up the start index if there is not
    * enough initial data.
    * Always one price bar gets consumed.
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

   outIdx = 0;
   today = startIdx;
   while( today <= endIdx )
   {

      /* Find the greatest of the 3 values. */
      tempLT = inLow[today];
      tempHT = inHigh[today];
      tempCY = inClose[today-1];
      greatest = tempHT - tempLT; /* val1 */

      val2 = std_fabs( tempCY - tempHT );
      if( val2 > greatest )
         greatest = val2;

      val3 = std_fabs( tempCY - tempLT  );
      if( val3 > greatest )
         greatest = val3;

      outReal[outIdx++] = greatest;
      today++;
   }

   *outNBElement = outIdx;
   *outBegIdx    = startIdx; 

   return TA_SUCCESS;
}
