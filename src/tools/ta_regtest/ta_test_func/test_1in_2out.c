

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
 *  020203 MF   First version.
 *
 */

/* Description:
 *
 *     Test functions which have the following
 *     characterisic: 
 *      - have one input and two outputs
 *      - there is no optional parameters
 */

/**** Headers ****/
#include <stdio.h>
#include <string.h>

#include "ta_test_priv.h"
#include "ta_test_func.h"
#include "ta_utility.h"
#include "ta_memory.h"

/**** External functions declarations. ****/
/* None */

/**** External variables declarations. ****/
/* None */

/**** Global variables definitions.    ****/
/* None */

/**** Local declarations.              ****/
typedef enum {
  TA_HT_PHASOR_TEST,
  TA_HT_SINE_TEST
} TA_TestId;

typedef struct
{
   int  doRangeTestFlag;

   TA_TestId   theFunction;
   int  unstablePeriod;

   int startIdx;
   int endIdx;
   
   TA_RetCode expectedRetCode;

   int oneOfTheExpectedOutRealIndex0;
   double    oneOfTheExpectedOutReal0;

   int oneOfTheExpectedOutRealIndex1;
   double    oneOfTheExpectedOutReal1;

   int expectedBegIdx;
   int expectedNbElement;
} TA_Test;

typedef struct
{
   const TA_Test *test;
   const double *price;
} TA_RangeTestParam;

/**** Local functions declarations.    ****/
static ErrorNumber do_test( const TA_History *history,
                            const TA_Test *test );

/**** Local variables definitions.     ****/

static TA_Test tableTest[] =
{
   /********************************/
   /* Some Hilbert Transform Tests */
   /********************************/
   { 1, TA_HT_SINE_TEST, 0, 0, 251, TA_SUCCESS, 0, 0.38,
                                                0, 0.92,
                                                63, 252-63 },
   { 0, TA_HT_SINE_TEST, 0, 0, 251, TA_SUCCESS, 1, 0.77,
                                                1, 1.00,
                                                63, 252-63 },

   { 0, TA_HT_SINE_TEST, 0, 0, 251, TA_SUCCESS, 5,  0.65,
                                                5, -0.08,
                                                63, 252-63 },

   { 0, TA_HT_SINE_TEST, 0, 0, 251, TA_SUCCESS, 252-67, -0.94,
                                                252-67, -0.44,
                                                63, 252-63 },

   { 0, TA_HT_SINE_TEST, 0, 0, 251, TA_SUCCESS, 252-66, -0.52,
                                                252-66,  0.24,
                                                63, 252-63 },

   { 0, TA_HT_SINE_TEST, 0, 0, 251, TA_SUCCESS, 252-64, 0.73,
                                                252-64, 1.00,
                                                63, 252-63 },

   { 1, TA_HT_PHASOR_TEST, 0, 0, 251, TA_SUCCESS,      0, 0.9456, 
                                                       0, 5.2143, 
                                                       32, 252-32 }, /* First Value */

   { 0, TA_HT_PHASOR_TEST, 0, 0, 251, TA_SUCCESS,      1, 2.7539, 
                                                       1, 2.4129,
                                                      32, 252-32 },

   { 0, TA_HT_PHASOR_TEST, 0, 0, 251, TA_SUCCESS,      9, -0.7235, 
                                                       9, -5.9336,
                                                      32, 252-32 },

   { 0, TA_HT_PHASOR_TEST, 0, 0, 251, TA_SUCCESS, 252-34,  0.8386,
                                                  252-34, -0.8913,  
                                                  32, 252-32 },

   { 0, TA_HT_PHASOR_TEST, 0, 0, 251, TA_SUCCESS, 252-33,  0.3258,
                                                  252-33, -0.9447,  
                                                  32, 252-32 }, /* Last Value */


};

#define NB_TEST (sizeof(tableTest)/sizeof(TA_Test))

/**** Global functions definitions.   ****/
ErrorNumber test_func_1in_2out( TA_History *history )
{
   unsigned int i;
   ErrorNumber retValue;


   for( i=0; i < NB_TEST; i++ )
   {
      /* Re-initialize all the unstable period to zero. */
      TA_SetUnstablePeriod( TA_FUNC_UNST_ALL, 0 );

      if( (int)tableTest[i].expectedNbElement > (int)history->nbBars )
      {
         printf( "Failed Bad Parameter for Test #%d (%d,%d)\n",
                 i, tableTest[i].expectedNbElement, history->nbBars );
         return TA_TESTUTIL_TFRR_BAD_PARAM;
      }

      retValue = do_test( history, &tableTest[i] );
      if( retValue != 0 )
      {
         printf( "Failed Test #%d (Code=%d)\n", i, retValue );
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
   double *out1;
   double *out2;
   double *dummyOutput;
   
   (void)outputBufferInt;

   *isOutputInteger = 0;
  
   testParam = (TA_RangeTestParam *)opaqueData;   

   dummyOutput = TA_Malloc( (endIdx-startIdx+1) * sizeof(double) );
                     
   if( outputNb == 0 )
   {
      out1 = outputBuffer;
      out2 = dummyOutput;
   }
   else
   {
      out1 = dummyOutput;
      out2 = outputBuffer;
   }

   switch( testParam->test->theFunction )
   {
   case TA_HT_PHASOR_TEST:
      retCode = TA_HT_PHASOR( startIdx,
                              endIdx,
                              testParam->price,
                              outBegIdx,
                              outNbElement,                          
                              out1, out2 );
      *lookback = TA_HT_PHASOR_Lookback();
      break;
   case TA_HT_SINE_TEST:
      retCode = TA_HT_SINE( startIdx,
                            endIdx,
                            testParam->price,
                            outBegIdx,
                            outNbElement,                          
                            out1, out2 );
      *lookback = TA_HT_SINE_Lookback();
      break;
   default:
      retCode = TA_INTERNAL_ERROR(132);
   }

   TA_Free(dummyOutput);
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
   const double *referenceInput;

   /* Set to NAN all the elements of the gBuffers.  */
   clearAllBuffers();

   /* Build the input. */
   setInputBuffer( 0, history->close,  history->nbBars );
   setInputBuffer( 1, history->close,  history->nbBars );
   setInputBuffer( 2, history->close,  history->nbBars );

   /* Change the input to MEDPRICE for some tests. */
   switch( test->theFunction )
   {
   case TA_HT_PHASOR_TEST:
   case TA_HT_SINE_TEST:
      TA_MEDPRICE( 0, history->nbBars-1, history->high, history->low,
                   &outBegIdx, &outNbElement, gBuffer[0].in );

      TA_MEDPRICE( 0, history->nbBars-1, history->high, history->low,
                   &outBegIdx, &outNbElement, gBuffer[1].in );

      /* Will be use as reference */
      TA_MEDPRICE( 0, history->nbBars-1, history->high, history->low,
                   &outBegIdx, &outNbElement, gBuffer[2].in );

      referenceInput = gBuffer[2].in;
      break;
   default:
      referenceInput = history->close;
   }

   /* Make a simple first call. */
   switch( test->theFunction )
   {
   case TA_HT_PHASOR_TEST:
      retCode = TA_HT_PHASOR( test->startIdx,
                              test->endIdx,
                              gBuffer[0].in,
                              &outBegIdx,
                              &outNbElement,
                              gBuffer[0].out0,
                              gBuffer[0].out1 );
      break;
   case TA_HT_SINE_TEST:
      retCode = TA_HT_SINE( test->startIdx,
                            test->endIdx,
                            gBuffer[0].in,
                            &outBegIdx,
                            &outNbElement,
                            gBuffer[0].out0,
                            gBuffer[0].out1 );
      break;
   default:
      retCode = TA_INTERNAL_ERROR(133);
   }

   /* Check that the inputs were preserved. */
   errNb = checkDataSame( gBuffer[0].in, referenceInput, history->nbBars );
   if( errNb != TA_TEST_PASS )
      return errNb;

   errNb = checkDataSame( gBuffer[1].in, referenceInput, history->nbBars );
   if( errNb != TA_TEST_PASS )
      return errNb;

   CHECK_EXPECTED_VALUE( gBuffer[0].out0, 0 );
   CHECK_EXPECTED_VALUE( gBuffer[0].out1, 1 );

   outBegIdx = outNbElement = 0;

   /* Make another call where the input and the output 
    * are the same buffer.
    */
   switch( test->theFunction )
   {
   case TA_HT_PHASOR_TEST:
      retCode = TA_HT_PHASOR( test->startIdx,
                              test->endIdx,
                              gBuffer[0].in,
                              &outBegIdx,
                              &outNbElement,
                              gBuffer[0].in,
                              gBuffer[1].out1
                            );
      break;
   case TA_HT_SINE_TEST:
      retCode = TA_HT_SINE( test->startIdx,
                            test->endIdx,
                            gBuffer[0].in,
                            &outBegIdx,
                            &outNbElement,
                            gBuffer[0].in,
                            gBuffer[1].out1
                          );
      break;
   default:
      retCode = TA_INTERNAL_ERROR(134);
   }

   /* The previous call should have the same output
    * as this call.
    *
    * checkSameContent verify that all value different than NAN in
    * the first parameter is identical in the second parameter.
    */
   errNb = checkSameContent( gBuffer[0].out0, gBuffer[0].in );
   if( errNb != TA_TEST_PASS )
      return errNb;

   errNb = checkSameContent( gBuffer[0].out1, gBuffer[1].out1 );
   if( errNb != TA_TEST_PASS )
      return errNb;

   CHECK_EXPECTED_VALUE( gBuffer[0].in, 0 );
   CHECK_EXPECTED_VALUE( gBuffer[1].out1, 1 );

   /* Make another call where the input and the output 
    * are the same buffer.
    */
   switch( test->theFunction )
   {
   case TA_HT_PHASOR_TEST:
      retCode = TA_HT_PHASOR( test->startIdx,
                              test->endIdx,
                              gBuffer[1].in,
                              &outBegIdx,
                              &outNbElement,
                              gBuffer[1].out0,
                              gBuffer[1].in
                            );
      break;
   case TA_HT_SINE_TEST:
      retCode = TA_HT_SINE( test->startIdx,
                              test->endIdx,
                              gBuffer[1].in,
                              &outBegIdx,
                              &outNbElement,
                              gBuffer[1].out0,
                              gBuffer[1].in
                            );
      break;
   default:
      retCode = TA_INTERNAL_ERROR(134);
   }

   /* The previous call should have the same output
    * as this call.
    *
    * checkSameContent verify that all value different than NAN in
    * the first parameter is identical in the second parameter.
    */
   errNb = checkSameContent( gBuffer[0].out1, gBuffer[1].in );
   if( errNb != TA_TEST_PASS )
      return errNb;

   errNb = checkSameContent( gBuffer[0].out0, gBuffer[1].out0 );
   if( errNb != TA_TEST_PASS )
      return errNb;

   CHECK_EXPECTED_VALUE( gBuffer[1].in, 1 );
   CHECK_EXPECTED_VALUE( gBuffer[1].out0, 0 );

   /* Do a systematic test of most of the
    * possible startIdx/endIdx range.
    */
   testParam.test  = test;
   testParam.price = referenceInput;

   if( test->doRangeTestFlag )
   {
      switch( test->theFunction )
      {
      case TA_HT_PHASOR_TEST:
         errNb = doRangeTest( rangeTestFunction, 
                              TA_FUNC_UNST_HT_PHASOR,
                              (void *)&testParam, 1, 0 );
         break;
      case TA_HT_SINE_TEST:
         errNb = doRangeTest( rangeTestFunction, 
                              TA_FUNC_UNST_HT_SINE,
                              (void *)&testParam, 1, 10 );
         break;

      default:
         errNb = doRangeTest( rangeTestFunction, 
                              TA_FUNC_UNST_NONE,
                              (void *)&testParam, 1, 0 );
      }
      if( errNb != TA_TEST_PASS )
         return errNb;
   }

   return TA_TEST_PASS;
}

