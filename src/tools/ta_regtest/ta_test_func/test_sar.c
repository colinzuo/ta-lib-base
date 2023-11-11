

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
 *     Test Parabolic SAR function using the example given
 *     in Wilder's book.
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
   int useWilderData;
   int startIdx;
   int endIdx;

   double optInAcceleration;
   double optInMaximum;

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

static double wilderHigh[] =
{
51.12,
52.35,52.1,51.8,52.1,52.5,52.8,52.5,53.5,53.5,53.8,54.2,53.4,53.5,
54.4,55.2,55.7,57,57.5,58,57.7,58,57.5,57,56.7,57.5,
56.70,56.00,56.20,54.80,55.50,54.70,54.00,52.50,51.00,51.50,51.70,53.00
};

static double wilderLow[] =
{
50.0,
51.5,51,50.5,51.25,51.7,51.85,51.5,52.3,52.5,53,53.5,52.5,52.1,53,
54,55,56,56.5,57,56.5,57.3,56.7,56.3,56.2,56,
55.50,55.00,54.90,54.00,54.50,53.80,53.00,51.50,50.00,50.50,50.20,51.50
};

#define WILDER_NB_BAR (sizeof(wilderLow)/sizeof(double))

static TA_Test tableTest[] =
{
   /**************************************/
   /*      SAR TEST WITH WILDER DATA     */
   /**************************************/
   { 1, 0, (WILDER_NB_BAR-1), 0.02, 0.20, TA_SUCCESS,  0, 50.00,   1, (WILDER_NB_BAR-1) }, /* First Value */
   { 1, 0, (WILDER_NB_BAR-1), 0.02, 0.20, TA_SUCCESS,  1, 50.047,  1, (WILDER_NB_BAR-1) },
   { 1, 0, (WILDER_NB_BAR-1), 0.02, 0.20, TA_SUCCESS,  4, 50.182,  1, (WILDER_NB_BAR-1) }, 
   { 1, 0, (WILDER_NB_BAR-1), 0.02, 0.20, TA_SUCCESS, 35, 52.93,   1, (WILDER_NB_BAR-1) }, 
   { 1, 0, (WILDER_NB_BAR-1), 0.02, 0.20, TA_SUCCESS, 36, 50.00,   1, (WILDER_NB_BAR-1) } /* Last value */
};

#define NB_TEST (sizeof(tableTest)/sizeof(TA_Test))

/**** Global functions definitions.   ****/
ErrorNumber test_func_sar( TA_History *history )
{
   unsigned int i;
   ErrorNumber retValue;

   /* Set all the unstable period to a weird value. This is to make sure
    * that no unstable period affects the SAR.
    */
   TA_SetUnstablePeriod( TA_FUNC_UNST_ALL, 124 );

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

   /* Re-initialize all the unstable period to zero. */
   TA_SetUnstablePeriod( TA_FUNC_UNST_ALL, 0 );

   /* All test succeed. */
   return TA_TEST_PASS; 
}

/**** Local functions definitions.     ****/

static ErrorNumber do_test( const TA_History *history,
                            const TA_Test *test )
{
   TA_RetCode retCode;
   ErrorNumber errNb;
   int outBegIdx;
   int outNbElement;

   const double *highPtr;
   const double *lowPtr;
   int nbPriceBar;

   /* Set to NAN all the elements of the gBuffers.  */
   clearAllBuffers();

   /* Build the input. */
   if( test->useWilderData )
   {
      highPtr = wilderHigh;
      lowPtr  = wilderLow;
      nbPriceBar = WILDER_NB_BAR;
   }
   else
   {
      highPtr = history->high;
      lowPtr  = history->low;
      nbPriceBar = history->nbBars;
   }

   setInputBuffer( 0, highPtr,  nbPriceBar );
   setInputBuffer( 1, lowPtr,   nbPriceBar );

   /* Make a simple first call. */
   retCode = TA_SAR( test->startIdx,
                     test->endIdx,
                     gBuffer[0].in,
                     gBuffer[1].in,
                     test->optInAcceleration,
                     test->optInMaximum,
                     &outBegIdx,
                     &outNbElement,
                     gBuffer[0].out0 );

   errNb = checkDataSame( gBuffer[0].in, highPtr, nbPriceBar );
   if( errNb != TA_TEST_PASS )
      return errNb;
   errNb = checkDataSame( gBuffer[1].in, lowPtr, nbPriceBar );
   if( errNb != TA_TEST_PASS )
      return errNb;

   CHECK_EXPECTED_VALUE( gBuffer[0].out0, 0 );

   outBegIdx = outNbElement = 0;

   /* Make another call where the input and the output are the
    * same buffer.
    */
   retCode = TA_SAR( test->startIdx,
                     test->endIdx,
                     gBuffer[0].in,
                     gBuffer[1].in,
                     test->optInAcceleration,
                     test->optInMaximum,
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

   /* Make sure the other input is untouched. */
   errNb = checkDataSame( gBuffer[0].in, highPtr, nbPriceBar );
   if( errNb != TA_TEST_PASS )
      return errNb;

   /* Repeat that last test but with the first parameter this time. */

   /* Set to NAN all the elements of the gBuffers.  */
   clearAllBuffers();

   /* Build the input. */
   setInputBuffer( 0, highPtr,  nbPriceBar );
   setInputBuffer( 1, lowPtr,   nbPriceBar );

   /* Make another call where the input and the output are the
    * same buffer.
    */
   retCode = TA_SAR( test->startIdx,
                     test->endIdx,
                     gBuffer[0].in,
                     gBuffer[1].in,
                     test->optInAcceleration,
                     test->optInMaximum,
                     &outBegIdx,
                     &outNbElement,
                     gBuffer[0].in );

   /* The previous call should have the same output as this call.
    *
    * checkSameContent verify that all value different than NAN in
    * the first parameter is identical in the second parameter.
    */
   errNb = checkSameContent( gBuffer[0].out0, gBuffer[0].in );
   if( errNb != TA_TEST_PASS )
      return errNb;

   CHECK_EXPECTED_VALUE( gBuffer[0].in, 0 );

   if( errNb != TA_TEST_PASS )
      return errNb;

   /* Make sure the other input is untouched. */
   errNb = checkDataSame( gBuffer[1].in, lowPtr, nbPriceBar);
   if( errNb != TA_TEST_PASS )
      return errNb;

   return TA_TEST_PASS;
}

