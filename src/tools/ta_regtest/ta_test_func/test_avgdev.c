
/* List of contributors:
 *
 *  Initial  Name/description
 *  -------------------------------------------------------------------
 *  AB       Anatoliy Belsky
 *
 *
 * Change history:
 *
 *  MMDDYY BY   Description
 *  -------------------------------------------------------------------
 *  112400 AB   First version.
 *
 */

/* Description:
 *     Test AVGDEV function.
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

   int startIdx;
   int endIdx;

   int optInTimePeriod;

   TA_RetCode expectedRetCode;

   int oneOfTheExpectedOutRealIndex0;
   double    oneOfTheExpectedOutReal0;

   int expectedBegIdx;
   int expectedNbElement;
} TA_Test;

typedef struct
{
   const TA_Test *test;
   const double *close;
} TA_RangeTestParam;

/**** Local functions declarations.    ****/
static ErrorNumber do_test( const TA_History *history,
                            const TA_Test *test );

/**** Local variables definitions.     ****/

static TA_Test tableTest[] =
{
   /*************************/
   /*      AVGDEV TEST      */
   /*************************/
   { 1, 0, 251, 5, TA_SUCCESS,     0, 1.0184,  4,  252-4 }, /* First Value */
   { 0, 0, 251, 5, TA_SUCCESS,     1, 0.3684,  4,  252-4 },
   { 0, 0, 251, 5, TA_SUCCESS, 252-5, 0.5952,  4,  252-4 }, /* Last Value */
};

#define NB_TEST (sizeof(tableTest)/sizeof(TA_Test))

/**** Global functions definitions.   ****/
ErrorNumber test_func_avgdev( TA_History *history )
{
   unsigned int i;
   ErrorNumber retValue;

   for( i=0; i < NB_TEST; i++ )
   {
      if( (int)tableTest[i].expectedNbElement > (int)history->nbBars )
      {
         printf( "%s Failed Bad Parameter for Test #%d (%d,%d)\n", __FILE__,
                 i, tableTest[i].expectedNbElement, history->nbBars );
         return TA_TESTUTIL_TFRR_BAD_PARAM;
      }

      retValue = do_test( history, &tableTest[i] );
      if( retValue != 0 )
      {
         printf( "%s Failed Test #%d (Code=%d)\n", __FILE__,
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

   retCode = TA_AVGDEV(
                        startIdx,
                        endIdx,
                        testParam->close,
                        testParam->test->optInTimePeriod,
                        outBegIdx,
                        outNbElement,
                        outputBuffer );


   *lookback = TA_AVGDEV_Lookback( testParam->test->optInTimePeriod );

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
   setInputBuffer( 0, history->close, history->nbBars );
   setInputBuffer( 1, history->close, history->nbBars );

   /* Make a simple first call. */
   retCode = TA_AVGDEV(
                        test->startIdx,
                        test->endIdx,
                        gBuffer[0].in,
                        test->optInTimePeriod,
                        &outBegIdx,
                        &outNbElement,
                        gBuffer[0].out0 );

   errNb = checkDataSame( gBuffer[0].in, history->close,history->nbBars );
   if( errNb != TA_TEST_PASS )
      return errNb;

   CHECK_EXPECTED_VALUE( gBuffer[0].out0, 0 );

   outBegIdx = outNbElement = 0;

   /* Make another call where the input and the output are the
    * same buffer.
    */
   retCode = TA_AVGDEV(
                        test->startIdx,
                        test->endIdx,
                        gBuffer[1].in,
                        test->optInTimePeriod,
                        &outBegIdx,
                        &outNbElement,
                        gBuffer[1].in );

   /* The previous call should have the same output as this call.
    *
    * checkSameContent verify that all value different than NAN in
    * the first parameter is identical in the second parameter.
    */
   errNb = checkSameContent( gBuffer[0].out0, gBuffer[1].in );
   if( errNb != TA_TEST_PASS )
      return errNb;

   CHECK_EXPECTED_VALUE( gBuffer[1].in, 0 );

   if( errNb != TA_TEST_PASS )
      return errNb;

   /* Do a systematic test of most of the
    * possible startIdx/endIdx range.
    */
   testParam.test  = test;
   testParam.close = history->close;

   if( test->doRangeTestFlag )
   {
      errNb = doRangeTest(
                           rangeTestFunction,
                           TA_FUNC_UNST_NONE,
                           (void *)&testParam, 1, 0 );
      if( errNb != TA_TEST_PASS )
         return errNb;
   }

   return TA_TEST_PASS;
}

