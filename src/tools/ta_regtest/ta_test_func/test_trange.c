

/* List of contributors:
 *
 *  Initial  Name/description
 *  -------------------------------------------------------------------
 *  MF       Mario Fortier
 *
 *
 * Change history:
 *
 *  MMDDYY BY   Description
 *  -------------------------------------------------------------------
 *  112400 MF   First version.
 *
 */

/* Description:
 *     Test TRANGE and ATR function.
 */

/**** Headers ****/
#include <stdio.h>
#include <string.h>

#include "ta_test_priv.h"
#include "ta_test_func.h"
#include "ta_utility.h"

/**** External functions declarations. ****/
/* None */

/**** External variables declarations. ****/
/* None */

/**** Global variables definitions.    ****/
/* None */

/**** Local declarations.              ****/
typedef struct
{
   int doRangeTestFlag; /* One will do a call to doRangeTest */

   int unstablePeriod;

   int startIdx;
   int endIdx;

   int doAverage;          /* 1 indicate ATR, else TRANGE. */
   int optInTimePeriod;  /* Meaningful only for ATR. */

   TA_RetCode expectedRetCode;

   int oneOfTheExpectedOutRealIndex;
   double    oneOfTheExpectedOutReal;

   int expectedBegIdx;
   int expectedNbElement;
} TA_Test;

typedef struct
{
   const TA_Test *test;
   const double *high;
   const double *low;
   const double *close;
} TA_RangeTestParam;

/**** Local functions declarations.    ****/
static ErrorNumber do_test( const TA_History *history,
                            const TA_Test *test );

/**** Local variables definitions.     ****/

static TA_Test tableTest[] =
{
   /* TRANGE TEST */
   { 1, 0, 0, 251, 0,  0, TA_SUCCESS,   0,  3.535,  1,  251 }, /* First Value */
   { 0, 0, 0, 251, 0,  0, TA_SUCCESS,  12,  9.685,  1,  251 },
   { 0, 0, 0, 251, 0,  0, TA_SUCCESS,  40,  5.125,  1,  251 },
   { 0, 0, 0, 251, 0,  0, TA_SUCCESS, 250,  2.88,   1,  251 }, /* Last Value */

   /* ATR TEST */
   { 1, 0, 0, 251, 1,  1, TA_SUCCESS,   0,  3.535,  1,  251 }, /* First Value */
   { 0, 0, 0, 251, 1,  1, TA_SUCCESS,  12,  9.685,  1,  251 },
   { 0, 0, 0, 251, 1,  1, TA_SUCCESS,  40,  5.125,  1,  251 },
   { 0, 0, 0, 251, 1,  1, TA_SUCCESS, 250,  2.88,   1,  251 }, /* Last Value */

   { 0, 1, 14, 15, 1, 14, TA_SUCCESS,   0, 3.4876, 15, 1 },
   { 0, 1, 15, 16, 1, 14, TA_SUCCESS,   0, 3.4876, 15, 2 },

   { 1, 0, 0, 251, 1, 14, TA_SUCCESS,   0,  3.578, 14,  252-14 }, /* First Value */
   { 0, 0, 0, 251, 1, 14, TA_SUCCESS,   1,  3.4876, 14, 252-14 },
   { 0, 0, 0, 251, 1, 14, TA_SUCCESS,   2,  3.55, 14,  252-14 },
   { 0, 0, 0, 251, 1, 14, TA_SUCCESS,  12,  3.245, 14,  252-14 },
   { 0, 0, 0, 251, 1, 14, TA_SUCCESS,  13,  3.394, 14,  252-14 },
   { 0, 0, 0, 251, 1, 14, TA_SUCCESS,  14,  3.413, 14,  252-14 },
   { 0, 0, 0, 251, 1, 14, TA_SUCCESS, 237,  3.26, 14,  252-14 }, /* Last Value */

};

#define NB_TEST (sizeof(tableTest)/sizeof(TA_Test))

/**** Global functions definitions.   ****/
ErrorNumber test_func_trange( TA_History *history )
{
   unsigned int i;
   ErrorNumber retValue;

   for( i=0; i < NB_TEST; i++ )
   {
      if( (int)tableTest[i].expectedNbElement > (int)history->nbBars )
      {
         printf( "%s Failed Bad Parameter for Test #%d (%d,%d)\n",
                 tableTest[i].doAverage? "TA_ATR":"TA_TRANGE",
                 i, tableTest[i].expectedNbElement, history->nbBars );
         return TA_TESTUTIL_TFRR_BAD_PARAM;
      }

      retValue = do_test( history, &tableTest[i] );
      if( retValue != 0 )
      {
         printf( "%s Failed Test #%d (Code=%d)\n",
                 tableTest[i].doAverage? "TA_ATR":"TA_TRANGE",              
                 i, retValue );
         return retValue;
      }
   }


   /* All test succeed. */
   return TA_TEST_PASS; 
}

/**** Local functions definitions.     ****/
static TA_RetCode rangeTestFunction( int    startIdx,
                                     int    endIdx,
                                     double      *outputBuffer,
                                     int   *outputBufferInt,
                                     int   *outBegIdx,
                                     int   *outNbElement,
                                     int   *lookback,
                                     void         *opaqueData,
                                     unsigned int  outputNb,
                                     unsigned int *isOutputInteger )
{
   TA_RetCode retCode;
   TA_RangeTestParam *testParam;

   (void)outputNb;
   (void)outputBufferInt;
  
   *isOutputInteger = 0;

   testParam = (TA_RangeTestParam *)opaqueData;   


   if( testParam->test->doAverage )
   {
      retCode = TA_ATR(
                        startIdx,
                        endIdx,
                        testParam->high,
                        testParam->low,
                        testParam->close,
                        testParam->test->optInTimePeriod,                        
                        outBegIdx,
                        outNbElement,
                        outputBuffer );
     *lookback = TA_ATR_Lookback( testParam->test->optInTimePeriod );
   }
   else
   {
      retCode = TA_TRANGE(
                        startIdx,
                        endIdx,
                        testParam->high,
                        testParam->low,
                        testParam->close,                        
                        outBegIdx,
                        outNbElement,
                        outputBuffer );

     *lookback = TA_TRANGE_Lookback();
   }

   return retCode;
}

static ErrorNumber do_test( const TA_History *history,
                            const TA_Test *test )
{
   TA_RetCode retCode;
   ErrorNumber errNb;
   int outBegIdx;
   int outNbElement;
   TA_RangeTestParam testParam;

   /* Set to NAN all the elements of the gBuffers.  */
   clearAllBuffers();

   /* Build the input. */
   setInputBuffer( 0, history->high,  history->nbBars );
   setInputBuffer( 1, history->low,   history->nbBars );
   setInputBuffer( 2, history->close, history->nbBars );

   if( test->doAverage )
   {
      TA_SetUnstablePeriod( TA_FUNC_UNST_ATR, test->unstablePeriod );
      retCode = TA_ATR(    test->startIdx,
                           test->endIdx,
                           gBuffer[0].in,
                           gBuffer[1].in,
                           gBuffer[2].in,
                           test->optInTimePeriod,                           
                           &outBegIdx,
                           &outNbElement,
                           gBuffer[0].out0 );
   }
   else
   {
      retCode = TA_TRANGE( test->startIdx,
                           test->endIdx,
                           gBuffer[0].in,
                           gBuffer[1].in,
                           gBuffer[2].in,                           
                           &outBegIdx,
                           &outNbElement,
                           gBuffer[0].out0 );
   }

   errNb = checkDataSame( gBuffer[0].in, history->high,history->nbBars );
   if( errNb != TA_TEST_PASS )
      return errNb;
   errNb = checkDataSame( gBuffer[1].in, history->low, history->nbBars );
   if( errNb != TA_TEST_PASS )
      return errNb;
   errNb = checkDataSame( gBuffer[2].in, history->close, history->nbBars );
   if( errNb != TA_TEST_PASS )
      return errNb;

   errNb = checkExpectedValue( gBuffer[0].out0, 
                               retCode, test->expectedRetCode,
                               outBegIdx, test->expectedBegIdx,
                               outNbElement, test->expectedNbElement,
                               test->oneOfTheExpectedOutReal,
                               test->oneOfTheExpectedOutRealIndex );   
   if( errNb != TA_TEST_PASS )
      return errNb;

   outBegIdx = outNbElement = 0;

   /* Make another call where the input and the output are the
    * same buffer.
    */
   if( test->doAverage )
   {
      TA_SetUnstablePeriod( TA_FUNC_UNST_ATR, test->unstablePeriod );
      retCode = TA_ATR( test->startIdx,
                        test->endIdx,
                        gBuffer[0].in,
                        gBuffer[1].in,
                        gBuffer[2].in,
                        test->optInTimePeriod,                           
                        &outBegIdx,
                        &outNbElement,
                        gBuffer[0].in );
   }
   else
   {
      retCode = TA_TRANGE( test->startIdx,
                           test->endIdx,
                           gBuffer[0].in,
                           gBuffer[1].in,
                           gBuffer[2].in,                           
                           &outBegIdx,
                           &outNbElement,
                           gBuffer[0].in );
   }

   /* The previous call to TA_MA should have the same output
    * as this call.
    *
    * checkSameContent verify that all value different than NAN in
    * the first parameter is identical in the second parameter.
    */
   errNb = checkSameContent( gBuffer[0].out0, gBuffer[0].in );
   if( errNb != TA_TEST_PASS )
      return errNb;

   errNb = checkExpectedValue( gBuffer[0].in, 
                               retCode, test->expectedRetCode,
                               outBegIdx, test->expectedBegIdx,
                               outNbElement, test->expectedNbElement,
                               test->oneOfTheExpectedOutReal,
                               test->oneOfTheExpectedOutRealIndex );   
   if( errNb != TA_TEST_PASS )
      return errNb;

   /* Do a systematic test of most of the
    * possible startIdx/endIdx range.
    */
   testParam.test  = test;
   testParam.high  = history->high;
   testParam.low   = history->low;
   testParam.close = history->close;

   if( test->doRangeTestFlag )
   {
      errNb = doRangeTest( rangeTestFunction, 
                           TA_FUNC_UNST_ATR,
                           (void *)&testParam, 1, 0 );
      if( errNb != TA_TEST_PASS )
         return errNb;
   }

   return TA_TEST_PASS;
}
