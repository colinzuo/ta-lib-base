

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
 *     Test functions that have one output and takes a period and
 *     basically use only an EMA in its algorithm.
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
typedef enum {
TA_TRIX_TEST
} TA_TestId;

typedef struct
{
   int doRangeTestFlag;

   TA_TestId  theFunction;

   int unstablePeriod;

   int startIdx;
   int endIdx;
   int optInTimePeriod;
   
   TA_RetCode expectedRetCode;

   int oneOfTheExpectedOutRealIndex;
   double    oneOfTheExpectedOutReal;

   int expectedBegIdx;
   int expectedNbElement;
} TA_Test;

typedef struct
{
   const TA_Test *test;
   const double *close;
} TA_RangeTestParam;

/**** Local functions declarations.    ****/
static ErrorNumber do_test_per_ema( const TA_History *history,
                                    const TA_Test *test );

/**** Local variables definitions.     ****/

static TA_Test tableTest[] =
{
   /****************/
   /*   TRIX TEST  */
   /****************/
   { 0, TA_TRIX_TEST, 0, 0, 251,  5, TA_SUCCESS,      0,   0.2589, 13,  252-13 }, /* First Value */
   { 0, TA_TRIX_TEST, 0, 0, 251,  5, TA_SUCCESS,      1,   0.010495, 13,  252-13 },
   { 0, TA_TRIX_TEST, 0, 0, 251,  5, TA_SUCCESS, 252-15,  -0.058, 13,  252-13 },
   { 0, TA_TRIX_TEST, 0, 0, 251,  5, TA_SUCCESS, 252-14,  -0.095, 13,  252-13 }, /* Last Value */

#if 0
   /* Metastock values. */
   { 0, TA_TRIX_TEST, 0, 0, 251,  5, TA_SUCCESS,      0,   0.221, 13,  252-13 }, /* First Value */
   { 0, TA_TRIX_TEST, 0, 0, 251,  5, TA_SUCCESS,      1,  -0.009, 13,  252-13 },
   { 0, TA_TRIX_TEST, 0, 0, 251,  5, TA_SUCCESS, 252-15,  -0.058, 13,  252-13 },
   { 0, TA_TRIX_TEST, 0, 0, 251,  5, TA_SUCCESS, 252-14,  -0.095, 13,  252-13 }, /* Last Value */
#endif

};

#define NB_TEST (sizeof(tableTest)/sizeof(TA_Test))

/**** Global functions definitions.   ****/
ErrorNumber test_func_per_ema( TA_History *history )
{
   unsigned int i;
   ErrorNumber retValue;

   /* Re-initialize all the unstable period to zero. */
   TA_SetUnstablePeriod( TA_FUNC_UNST_ALL, 0 );

   for( i=0; i < NB_TEST; i++ )
   {
      if( (int)tableTest[i].expectedNbElement > (int)history->nbBars )
      {
         printf( "TA_MA Failed Bad Parameter for Test #%d (%d,%d)\n",
                 i, tableTest[i].expectedNbElement, history->nbBars );
         return TA_TESTUTIL_TFRR_BAD_PARAM;
      }

      retValue = do_test_per_ema( history, &tableTest[i] );
      if( retValue != 0 )
      {
         printf( "TA_MA Failed Test #%d (Code=%d)\n", i, retValue );
         return retValue;
      }
   }

   /* Re-initialize all the unstable period to zero. */
   TA_SetUnstablePeriod( TA_FUNC_UNST_ALL, 0 );

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

   switch( testParam->test->theFunction )
   {
   case TA_TRIX_TEST:
      retCode = TA_TRIX( startIdx,
                         endIdx,
                         testParam->close,
                         testParam->test->optInTimePeriod,
                         outBegIdx,
                         outNbElement,
                         outputBuffer );
      *lookback = TA_TRIX_Lookback( testParam->test->optInTimePeriod );
   default:
      retCode = TA_INTERNAL_ERROR(131);
   } 

   return retCode;
}

static ErrorNumber do_test_per_ema( const TA_History *history,
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
   setInputBuffer( 0, history->close, history->nbBars );
   setInputBuffer( 1, history->close, history->nbBars );
   
   /* Set the unstable period requested for that test. */
   retCode = TA_SetUnstablePeriod( TA_FUNC_UNST_EMA, test->unstablePeriod );
   if( retCode != TA_SUCCESS )
      return TA_TEST_TFRR_SETUNSTABLE_PERIOD_FAIL;

   /* Make a simple first call. */
   switch( test->theFunction )
   {
   case TA_TRIX_TEST:
      retCode = TA_TRIX( test->startIdx,
                         test->endIdx,
                         gBuffer[0].in,
                         test->optInTimePeriod,
                         &outBegIdx,
                         &outNbElement,
                         gBuffer[0].out0 );

   }

   errNb = checkDataSame( gBuffer[0].in, history->close,history->nbBars );

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
   switch( test->theFunction )
   {
   case TA_TRIX_TEST:
      retCode = TA_TRIX( test->startIdx,
                         test->endIdx,
                         gBuffer[1].in,
                         test->optInTimePeriod,
                         &outBegIdx,
                         &outNbElement,
                         gBuffer[1].in );
   }

   /* The previous call to TA_MA should have the same output
    * as this call.
    *
    * checkSameContent verify that all value different than NAN in
    * the first parameter is identical in the second parameter.
    */
   errNb = checkSameContent( gBuffer[0].out0, gBuffer[1].in );
   if( errNb != TA_TEST_PASS )
      return errNb;

   errNb = checkExpectedValue( gBuffer[1].in, 
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
   testParam.close = history->close;

   if( test->doRangeTestFlag )
   {
      errNb = doRangeTest( rangeTestFunction, 
                           TA_FUNC_UNST_EMA,
                           (void *)&testParam, 1, 0 );
      if( errNb != TA_TEST_PASS )
         return errNb;
   }

   return TA_TEST_PASS;
}

