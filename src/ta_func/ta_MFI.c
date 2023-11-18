#include <string.h>
#include <math.h>
#include "ta_func.h"

#include "ta_utility.h"
#include "ta_memory.h"


int TA_MFI_Lookback( int           optInTimePeriod )  /* From 2 to 100000 */
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
   return optInTimePeriod + TA_GLOBALS_UNSTABLE_PERIOD(TA_FUNC_UNST_MFI,Mfi);
}

typedef struct
{
   double positive;
   double negative;
} MoneyFlow;

/*
 * TA_MFI - Money Flow Index
 * 
 * Input  = High, Low, Close, Volume
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_MFI( int    startIdx,
                   int    endIdx,
                   const double inHigh[],
                   const double inLow[],
                   const double inClose[],
                   const double inVolume[],
                   int           optInTimePeriod, /* From 2 to 100000 */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] )
{
	/* insert local variable here */
   double posSumMF, negSumMF, prevValue;
   double tempValue1, tempValue2;
   int lookbackTotal, outIdx, i, today;

   CIRCBUF_PROLOG_CLASS( mflow, MoneyFlow, 50 ); /* Id, Type, Static Size */

#ifndef TA_FUNC_NO_RANGE_CHECK

   /* Validate the requested output range. */
   if( startIdx < 0 )
      return TA_OUT_OF_RANGE_START_INDEX;
   if( (endIdx < 0) || (endIdx < startIdx))
      return TA_OUT_OF_RANGE_END_INDEX;

   /* Verify required price component. */
   if(!inHigh||!inLow||!inClose||!inVolume)
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

   CIRCBUF_INIT_CLASS( mflow, MoneyFlow, optInTimePeriod );   

   *outBegIdx = 0;
   *outNBElement = 0;
   
   /* Adjust startIdx to account for the lookback period. */
   lookbackTotal = optInTimePeriod + TA_GLOBALS_UNSTABLE_PERIOD(TA_FUNC_UNST_MFI,Mfi);

   if( startIdx < lookbackTotal )
      startIdx = lookbackTotal;

   /* Make sure there is still something to evaluate. */
   if( startIdx > endIdx )
   {
      CIRCBUF_DESTROY(mflow);
      return TA_SUCCESS;
   }

   outIdx = 0; /* Index into the output. */

   /* Accumulate the positive and negative money flow
    * among the initial period.
    */
   today = startIdx-lookbackTotal;
   prevValue = (inHigh[today]+inLow[today]+inClose[today])/3.0;

   posSumMF = 0.0;
   negSumMF = 0.0;
   today++;
   for( i=optInTimePeriod; i > 0; i-- )
   {
      tempValue1 = (inHigh[today]+inLow[today]+inClose[today])/3.0;
      tempValue2 = tempValue1 - prevValue;
      prevValue  = tempValue1;
      tempValue1 *= inVolume[today++];
      if( tempValue2 < 0 )
      {
         CIRCBUF_REF(mflow[mflow_Idx])negative = tempValue1;
         negSumMF += tempValue1;
         CIRCBUF_REF(mflow[mflow_Idx])positive = 0.0;
      }
      else if( tempValue2 > 0 )
      {
         CIRCBUF_REF(mflow[mflow_Idx])positive = tempValue1;
         posSumMF += tempValue1;
         CIRCBUF_REF(mflow[mflow_Idx])negative = 0.0;
      }
	  else
	  {
         CIRCBUF_REF(mflow[mflow_Idx])positive = 0.0;
         CIRCBUF_REF(mflow[mflow_Idx])negative = 0.0;
	  }

      CIRCBUF_NEXT(mflow);
   }
   
   /* The following two equations are equivalent:
    *    MFI = 100 - (100 / 1 + (posSumMF/negSumMF))
    *    MFI = 100 * (posSumMF/(posSumMF+negSumMF))
    * The second equation is used here for speed optimization.
    */
   if( today > startIdx )
   {
      tempValue1 = posSumMF+negSumMF;
      if( tempValue1 < 1.0 )
         outReal[outIdx++] = 0.0;
      else
         outReal[outIdx++] = 100.0*(posSumMF/tempValue1);
   }
   else
   {
      /* Skip the unstable period. Do the processing 
       * but do not write it in the output.
       */   
      while( today < startIdx )
      {
         posSumMF -= CIRCBUF_REF(mflow[mflow_Idx])positive;
         negSumMF -= CIRCBUF_REF(mflow[mflow_Idx])negative;

         tempValue1 = (inHigh[today]+inLow[today]+inClose[today])/3.0;
         tempValue2 = tempValue1 - prevValue;
         prevValue  = tempValue1;
         tempValue1 *= inVolume[today++];
         if( tempValue2 < 0 )
         {
            CIRCBUF_REF(mflow[mflow_Idx])negative = tempValue1;
            negSumMF += tempValue1;
            CIRCBUF_REF(mflow[mflow_Idx])positive = 0.0;
         }
         else if( tempValue2 > 0 )
         {
            CIRCBUF_REF(mflow[mflow_Idx])positive = tempValue1;
            posSumMF += tempValue1;
            CIRCBUF_REF(mflow[mflow_Idx])negative = 0.0;
         }
	     else
	     {
            CIRCBUF_REF(mflow[mflow_Idx])positive = 0.0;
            CIRCBUF_REF(mflow[mflow_Idx])negative = 0.0;
	     }

         CIRCBUF_NEXT(mflow);
      }
   }

   /* Unstable period skipped... now continue
    * processing if needed.
    */
   while( today <= endIdx )
   {
      posSumMF -= CIRCBUF_REF(mflow[mflow_Idx])positive;
      negSumMF -= CIRCBUF_REF(mflow[mflow_Idx])negative;

      tempValue1 = (inHigh[today]+inLow[today]+inClose[today])/3.0;
      tempValue2 = tempValue1 - prevValue;
      prevValue  = tempValue1;
      tempValue1 *= inVolume[today++];
      if( tempValue2 < 0 )
      {
         CIRCBUF_REF(mflow[mflow_Idx])negative = tempValue1;
         negSumMF += tempValue1;
         CIRCBUF_REF(mflow[mflow_Idx])positive = 0.0;
      }
      else if( tempValue2 > 0 )
      {
         CIRCBUF_REF(mflow[mflow_Idx])positive = tempValue1;
         posSumMF += tempValue1;
         CIRCBUF_REF(mflow[mflow_Idx])negative = 0.0;
      }
      else
	  {
         CIRCBUF_REF(mflow[mflow_Idx])positive = 0.0;
         CIRCBUF_REF(mflow[mflow_Idx])negative = 0.0;
	  }

      tempValue1 = posSumMF+negSumMF;
      if( tempValue1 < 1.0 )
         outReal[outIdx++] = 0.0;
      else
         outReal[outIdx++] = 100.0*(posSumMF/tempValue1);

      CIRCBUF_NEXT(mflow);
   }

   CIRCBUF_DESTROY(mflow);

   *outBegIdx = startIdx;
   *outNBElement = outIdx;

   return TA_SUCCESS;
}
