
## `ta_func.h`

```cpp
/*
 * TA_BBANDS - Bollinger Bands
 * 
 * Input  = double
 * Output = double, double, double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * optInNbDevUp:(From TA_REAL_MIN to TA_REAL_MAX)
 *    Deviation multiplier for upper band
 * 
 * optInNbDevDn:(From TA_REAL_MIN to TA_REAL_MAX)
 *    Deviation multiplier for lower band
 * 
 * optInMAType:
 *    Type of Moving Average
 * 
 * 
 */
TA_RetCode TA_BBANDS( int    startIdx,
                      int    endIdx,
                      const double inReal[],
                      int           optInTimePeriod, /* From 2 to 100000 */
                      double        optInNbDevUp, /* From TA_REAL_MIN to TA_REAL_MAX */
                      double        optInNbDevDn, /* From TA_REAL_MIN to TA_REAL_MAX */
                      TA_MAType     optInMAType,
                      int          *outBegIdx,
                      int          *outNBElement,
                      double        outRealUpperBand[],
                      double        outRealMiddleBand[],
                      double        outRealLowerBand[] );

int TA_BBANDS_Lookback( int           optInTimePeriod, /* From 2 to 100000 */
                      double        optInNbDevUp, /* From TA_REAL_MIN to TA_REAL_MAX */
                      double        optInNbDevDn, /* From TA_REAL_MIN to TA_REAL_MAX */
                      TA_MAType     optInMAType ); 
```                      

## `ta_func`

`src\ta_func\ta_BBANDS.c`
