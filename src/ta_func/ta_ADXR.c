#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"

#define TA_PREFIX(x) TA_##x
#define INPUT_TYPE   double

int TA_ADXR_Lookback( int           optInTimePeriod )  /* From 2 to 100000 */
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
      return optInTimePeriod + TA_ADX_Lookback( optInTimePeriod) - 1;
   else
      return 3;
}

/*
 * TA_ADXR - Average Directional Movement Index Rating
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
TA_RetCode TA_ADXR( int    startIdx,
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
   ARRAY_REF( adx );
   int adxrLookback, i, j, outIdx, nbElement;
   TA_RetCode retCode;

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

   /* Move up the start index if there is not
    * enough initial data.
    * Always one price bar gets consumed.
    */
   adxrLookback = TA_ADXR_Lookback( optInTimePeriod );

   if( startIdx < adxrLookback )
      startIdx = adxrLookback;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
   {
      *outBegIdx = 0;
      *outNBElement = 0;
      return TA_SUCCESS;
   }

   ARRAY_ALLOC( adx, endIdx-startIdx+optInTimePeriod );
   #if !defined( _JAVA )
      if( !adx )
         return TA_ALLOC_ERR;
   #endif

   retCode = TA_PREFIX(ADX)( startIdx-(optInTimePeriod-1), endIdx,
                                 inHigh, inLow, inClose,
                                 optInTimePeriod, outBegIdx, outNBElement, adx );

   if( retCode != TA_SUCCESS )      
   {
      ARRAY_FREE( adx );
      return retCode;
   }

   i = optInTimePeriod-1;
   j = 0;
   outIdx = 0;
   nbElement = endIdx-startIdx+2;
   while( --nbElement != 0 )
      outReal[outIdx++] = round_pos( (adx[i++]+adx[j++])/2.0 );

   ARRAY_FREE( adx );

   *outBegIdx    = startIdx;
   *outNBElement = outIdx;

   return TA_SUCCESS;
}
