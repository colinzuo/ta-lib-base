/**** Headers ****/
#ifdef _WIN32
   #include "windows.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ta_test_priv.h"
#include "ta_test_func.h"
#include "ta_utility.h"

/**** Global variables definitions.    ****/
int nbProfiledCall;
double timeInProfiledCall;
double worstProfiledCall;
int insufficientClockPrecision;
int doExtensiveProfiling;

/**** Local functions declarations.    ****/
static ErrorNumber testTAFunction_ALL( void );
static ErrorNumber test_with_simulator( void );
static void printUsage(void);

/**** Global functions definitions.   ****/
int main( int argc, char **argv )
{
#ifdef _WIN32
	LARGE_INTEGER QPFrequency;
#endif
   double freq;

   ErrorNumber retValue;

   (void)argv;

   insufficientClockPrecision = 0;
   timeInProfiledCall = 0.0;
   worstProfiledCall = 0.0;
   nbProfiledCall = 0;
   doExtensiveProfiling = 0;

   printf( "\n" );
   printf( "ta_regtest V%s - Regression Tests of TA-Lib code\n", TA_GetVersionString() );
   printf( "\n" );

   if ( argc == 2 )
   {
	   /* Detect option to perform extended profiling. */
	   if ( (argv[1][0] == '-') && (argv[1][1] == 'p') && (argv[1][2] == '\0'))
	   {
		   doExtensiveProfiling = 1;
	   }
	   else
	   {
		   printUsage();
		   return TA_REGTEST_BAD_USER_PARAM;
	   }
   }

   if ( argc > 2 )
   {
      printUsage();
      return TA_REGTEST_BAD_USER_PARAM;
   }

   /* Some tests are using randomness. */ 
   srand( (unsigned)time( NULL ) );

   /* Test utility like List/Stack/Dictionary/Memory Allocation etc... */
   retValue = test_internals();
   if ( retValue != TA_TEST_PASS )
   {
      printf( "\nFailed an internal test with code=%d\n", retValue );
      return retValue;
   }

   /* Perform all regresstions tests (except when ta_regtest is executed for profiling only). */
   if ( !doExtensiveProfiling )
   {
      retValue = test_with_simulator();
      if ( retValue != TA_TEST_PASS )
         return retValue;

      if ( insufficientClockPrecision != 0 )
      {
   	   printf( "\nWarning: Code profiling not supported for this platform.\n" );
      }
      else if ( nbProfiledCall > 0 )
      {
         printf( "\nNumber profiled function call       = %d function calls", nbProfiledCall );	  

#ifdef _WIN32
         QueryPerformanceFrequency(&QPFrequency);
         freq = (double)QPFrequency.QuadPart;
         printf( "\nTotal execution time                = %g milliseconds", (timeInProfiledCall/freq)*1000.0 );
         printf( "\nWorst single function call          = %g milliseconds", (worstProfiledCall/freq)*1000.0 );
         printf( "\nAverage execution time per function = %g microseconds\n", ((timeInProfiledCall/freq)*1000000.0)/((double)nbProfiledCall) );
#else
         freq = (double)CLOCKS_PER_SEC;
         printf( "\nTotal execution time                = %g milliseconds", timeInProfiledCall/freq/1000.0 );
         printf( "\nWorst single function call          = %g milliseconds", worstProfiledCall/freq/1000.0 );
         printf( "\nAverage execution time per function = %g microseconds\n", (timeInProfiledCall/freq/1000000.0)/((double)nbProfiledCall) );
#endif	  
      }   
      printf( "\n* All tests succeeded. Enjoy the library. *\n" );
   }


   return TA_TEST_PASS; /* Everything succeed !!! */
}

/**** Local functions definitions.     ****/
static ErrorNumber test_with_simulator( void )
{
   ErrorNumber retValue;

   /* Initialize the library. */
   retValue = allocLib();
   if ( retValue != TA_TEST_PASS )
      return retValue;

   /* Perform testing of each of the TA Functions. */
   retValue = testTAFunction_ALL();
   if ( retValue != TA_TEST_PASS )
   {
      return retValue;
   }

   /* Clean-up and exit. */

   retValue = freeLib( );
   if ( retValue != TA_TEST_PASS )
      return retValue;

   return TA_TEST_PASS; /* All test succeed. */
}

extern double      TA_SREF_open_daily_ref_0_PRIV[];
extern double      TA_SREF_high_daily_ref_0_PRIV[];
extern double      TA_SREF_low_daily_ref_0_PRIV[];
extern double      TA_SREF_close_daily_ref_0_PRIV[];
extern double      TA_SREF_volume_daily_ref_0_PRIV[];

static ErrorNumber testTAFunction_ALL( void )
{
   ErrorNumber retValue;
   TA_History history;

   history.nbBars = 252;
   history.open   = TA_SREF_open_daily_ref_0_PRIV;
   history.high   = TA_SREF_high_daily_ref_0_PRIV;
   history.low    = TA_SREF_low_daily_ref_0_PRIV;
   history.close  = TA_SREF_close_daily_ref_0_PRIV;
   history.volume = TA_SREF_volume_daily_ref_0_PRIV;

   printf( "Testing the TA functions\n" );

   initGlobalBuffer();

   /* Make tests for each TA functions. */
   #define DO_TEST(func,str) \
      { \
      printf( "%50s: Testing....", str ); \
      fflush(stdout); \
      showFeedback(); \
      TA_SetCompatibility( TA_COMPATIBILITY_DEFAULT ); \
      retValue = func( &history ); \
      if ( retValue != TA_TEST_PASS ) \
         return retValue; \
      hideFeedback(); \
      printf( "done.\n" ); \
      fflush(stdout); \
      }
   DO_TEST( test_func_1in_1out, "MATH,VECTOR,DCPERIOD/PHASE,TRENDLINE/MODE" );   
   DO_TEST( test_func_ma,       "All Moving Averages" );
   DO_TEST( test_func_per_hl,   "AROON,CORREL,BETA" );
   DO_TEST( test_func_per_hlc,  "CCI,WILLR,ULTOSC,NATR" );
   DO_TEST( test_func_per_ohlc, "BOP,AVGPRICE" );
   DO_TEST( test_func_minmax,   "MIN,MAX,MININDEX,MAXINDEX,MINMAX,MINMAXINDEX" );
   DO_TEST( test_func_po,       "PO,APO" );
   DO_TEST( test_func_adx,      "ADX,ADXR,DI,DM,DX" );
   DO_TEST( test_func_sar,      "SAR,SAREXT" );
   DO_TEST( test_func_stoch,    "STOCH,STOCHF,STOCHRSI" );
   DO_TEST( test_func_per_hlcv, "MFI,AD,ADOSC" );
   DO_TEST( test_func_1in_2out, "PHASOR,SINE" );   
   DO_TEST( test_func_per_ema,  "TRIX" );
   DO_TEST( test_func_macd,     "MACD,MACDFIX,MACDEXT" );
   DO_TEST( test_func_mom_roc,  "MOM,ROC,ROCP,ROCR,ROCR100" );
   DO_TEST( test_func_trange,   "TRANGE,ATR" );
   DO_TEST( test_func_stddev,   "STDDEV,VAR" );
   DO_TEST( test_func_bbands,   "BBANDS" );

   return TA_TEST_PASS; /* All tests succeeded. */
}

static void printUsage(void)
{
      printf( "Usage: ta_regtest [-p]\n" );
      printf( "\n" );
      printf( "   No parameter needed for regression testing.\n" );
      printf( "\n" );
      printf( "   This tool will execute a series of tests to\n" );
      printf( "   make sure that the library is behaving as\n" );
      printf( "   expected.\n");
	  printf( "\n" );
      printf( "   ** Must be run from the 'bin' directory.\n" );
      printf( "\n" );
	  printf( "   OPTION:\n" );
      printf( "    -p Only generate profiling data on stdout. This is\n" );
      printf( "       intended only for the TA-Lib developers. It is\n" );
      printf( "       not further documented for general use.\n" );
      printf( "\n" );
      printf( "   On success, the exit code is 0.\n" );
      printf( "   On failure, the exit code is a number that can be\n" );
      printf( "   found in c/src/tools/ta_regtest/ta_error_number.h\n" );
}
