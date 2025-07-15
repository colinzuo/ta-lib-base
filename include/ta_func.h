
#pragma once

#include "ta_defs.h"

/* This header contains the prototype of all the Technical Analysis
 * function provided by TA-LIB.
 */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * TA_ACCBANDS - Acceleration Bands
 * 
 * Input  = High, Low, Close
 * Output = double, double, double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_ACCBANDS( int    startIdx,
                        int    endIdx,
                        const double inHigh[],
                        const double inLow[],
                        const double inClose[],
                        int           optInTimePeriod, /* From 2 to 100000 */
                        int          *outBegIdx,
                        int          *outNBElement,
                        double        outRealUpperBand[],
                        double        outRealMiddleBand[],
                        double        outRealLowerBand[] );
int TA_ACCBANDS_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */

/*
 * TA_ACOS - Vector Trigonometric ACos
 * 
 * Input  = double
 * Output = double
 * 
 */
TA_RetCode TA_ACOS( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );

int TA_ACOS_Lookback( void );


/*
 * TA_AD - Chaikin A/D Line
 * 
 * Input  = High, Low, Close, Volume
 * Output = double
 * 
 */
TA_RetCode TA_AD( int    startIdx,
                  int    endIdx,
                  const double inHigh[],
                  const double inLow[],
                  const double inClose[],
                  const double inVolume[],
                  int          *outBegIdx,
                  int          *outNBElement,
                  double        outReal[] );

int TA_AD_Lookback( void );


/*
 * TA_ADD - Vector Arithmetic Add
 * 
 * Input  = double, double
 * Output = double
 * 
 */
TA_RetCode TA_ADD( int    startIdx,
                   int    endIdx,
                   const double inReal0[],
                   const double inReal1[],
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_ADD_Lookback( void );


/*
 * TA_ADOSC - Chaikin A/D Oscillator
 * 
 * Input  = High, Low, Close, Volume
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInFastPeriod:(From 2 to 100000)
 *    Number of period for the fast MA
 * 
 * optInSlowPeriod:(From 2 to 100000)
 *    Number of period for the slow MA
 * 
 * 
 */
TA_RetCode TA_ADOSC( int    startIdx,
                     int    endIdx,
                     const double inHigh[],
                     const double inLow[],
                     const double inClose[],
                     const double inVolume[],
                     int           optInFastPeriod, /* From 2 to 100000 */
                     int           optInSlowPeriod, /* From 2 to 100000 */
                     int          *outBegIdx,
                     int          *outNBElement,
                     double        outReal[] );

int TA_ADOSC_Lookback( int           optInFastPeriod, /* From 2 to 100000 */
                     int           optInSlowPeriod );  /* From 2 to 100000 */


/*
 * TA_ADX - Average Directional Movement Index
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
TA_RetCode TA_ADX( int    startIdx,
                   int    endIdx,
                   const double inHigh[],
                   const double inLow[],
                   const double inClose[],
                   int           optInTimePeriod, /* From 2 to 100000 */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_ADX_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


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
                    double        outReal[] );

int TA_ADXR_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_APO - Absolute Price Oscillator
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInFastPeriod:(From 2 to 100000)
 *    Number of period for the fast MA
 * 
 * optInSlowPeriod:(From 2 to 100000)
 *    Number of period for the slow MA
 * 
 * optInMAType:
 *    Type of Moving Average
 * 
 * 
 */
TA_RetCode TA_APO( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInFastPeriod, /* From 2 to 100000 */
                   int           optInSlowPeriod, /* From 2 to 100000 */
                   TA_MAType     optInMAType,
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_APO_Lookback( int           optInFastPeriod, /* From 2 to 100000 */
                   int           optInSlowPeriod, /* From 2 to 100000 */
                   TA_MAType     optInMAType ); 

/*
 * TA_AROON - Aroon
 * 
 * Input  = High, Low
 * Output = double, double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_AROON( int    startIdx,
                     int    endIdx,
                     const double inHigh[],
                     const double inLow[],
                     int           optInTimePeriod, /* From 2 to 100000 */
                     int          *outBegIdx,
                     int          *outNBElement,
                     double        outAroonDown[],
                     double        outAroonUp[] );

int TA_AROON_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_AROONOSC - Aroon Oscillator
 * 
 * Input  = High, Low
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_AROONOSC( int    startIdx,
                        int    endIdx,
                        const double inHigh[],
                        const double inLow[],
                        int           optInTimePeriod, /* From 2 to 100000 */
                        int          *outBegIdx,
                        int          *outNBElement,
                        double        outReal[] );

int TA_AROONOSC_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_ASIN - Vector Trigonometric ASin
 * 
 * Input  = double
 * Output = double
 * 
 */
TA_RetCode TA_ASIN( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );

int TA_ASIN_Lookback( void );


/*
 * TA_ATAN - Vector Trigonometric ATan
 * 
 * Input  = double
 * Output = double
 * 
 */
TA_RetCode TA_ATAN( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );

int TA_ATAN_Lookback( void );


/*
 * TA_ATR - Average True Range
 * 
 * Input  = High, Low, Close
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_ATR( int    startIdx,
                   int    endIdx,
                   const double inHigh[],
                   const double inLow[],
                   const double inClose[],
                   int           optInTimePeriod, /* From 1 to 100000 */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_ATR_Lookback( int           optInTimePeriod );  /* From 1 to 100000 */


/*
 * TA_AVGPRICE - Average Price
 * 
 * Input  = Open, High, Low, Close
 * Output = double
 * 
 */
TA_RetCode TA_AVGPRICE( int    startIdx,
                        int    endIdx,
                        const double inOpen[],
                        const double inHigh[],
                        const double inLow[],
                        const double inClose[],
                        int          *outBegIdx,
                        int          *outNBElement,
                        double        outReal[] );

int TA_AVGPRICE_Lookback( void );

/*
 * TA_AVGDEV - Average Deviation
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_AVGDEV(   int    startIdx,
                        int    endIdx,
                        const double inReal[],
                        int           optInTimePeriod, /* From 2 to 100000 */
                        int          *outBegIdx,
                        int          *outNBElement,
                        double        outReal[] );

int TA_AVGDEV_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


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

/*
 * TA_BBDist - Bollinger Bands Dist
 *
 * Input  = double
 * Output = double
 *
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 *
 * optInNbDev:(From TA_REAL_MIN to TA_REAL_MAX)
 *    Deviation multiplier
 */
TA_RetCode TA_BBDist(int    startIdx,
    int    endIdx,
    const double inReal[],
    int           optInTimePeriod, /* From 2 to 100000 */
    double        optInNbDev, /* From TA_REAL_MIN to TA_REAL_MAX */
    int* outBegIdx,
    int* outNBElement,
    double        outRealUDist[],
    double        outRealLDist[]);

int TA_BBDist_Lookback(int           optInTimePeriod, /* From 2 to 100000 */
    double        optInNbDev);

/*
 * TA_BBWidth - Bollinger Bands Width
 *
 * Input  = double
 * Output = double
 *
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 *
 * optInNbDev:(From TA_REAL_MIN to TA_REAL_MAX)
 *    Deviation multiplier
 */
TA_RetCode TA_BBWidth(int    startIdx,
    int    endIdx,
    const double inReal[],
    int           optInTimePeriod, /* From 2 to 100000 */
    double        optInNbDev, /* From TA_REAL_MIN to TA_REAL_MAX */
    int* outBegIdx,
    int* outNBElement,
    double        outRealWidth[]);

int TA_BBWidth_Lookback(int           optInTimePeriod, /* From 2 to 100000 */
    double        optInNbDev);

/*
 * TA_BETA - Beta
 * 
 * Input  = double, double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_BETA( int    startIdx,
                    int    endIdx,
                    const double inReal0[],
                    const double inReal1[],
                    int           optInTimePeriod, /* From 1 to 100000 */
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );

int TA_BETA_Lookback( int           optInTimePeriod );  /* From 1 to 100000 */


/*
 * TA_BOP - Balance Of Power
 * 
 * Input  = Open, High, Low, Close
 * Output = double
 * 
 */
TA_RetCode TA_BOP( int    startIdx,
                   int    endIdx,
                   const double inOpen[],
                   const double inHigh[],
                   const double inLow[],
                   const double inClose[],
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_BOP_Lookback( void );


/*
 * TA_CCI - Commodity Channel Index
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
TA_RetCode TA_CCI( int    startIdx,
                   int    endIdx,
                   const double inHigh[],
                   const double inLow[],
                   const double inClose[],
                   int           optInTimePeriod, /* From 2 to 100000 */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_CCI_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_CDL2CROWS - Two Crows
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDL2CROWS( int    startIdx,
                         int    endIdx,
                         const double inOpen[],
                         const double inHigh[],
                         const double inLow[],
                         const double inClose[],
                         int          *outBegIdx,
                         int          *outNBElement,
                         int           outInteger[] );

int TA_CDL2CROWS_Lookback( void );


/*
 * TA_CDL3BLACKCROWS - Three Black Crows
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDL3BLACKCROWS( int    startIdx,
                              int    endIdx,
                              const double inOpen[],
                              const double inHigh[],
                              const double inLow[],
                              const double inClose[],
                              int          *outBegIdx,
                              int          *outNBElement,
                              int           outInteger[] );

int TA_CDL3BLACKCROWS_Lookback( void );


/*
 * TA_CDL3INSIDE - Three Inside Up/Down
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDL3INSIDE( int    startIdx,
                          int    endIdx,
                          const double inOpen[],
                          const double inHigh[],
                          const double inLow[],
                          const double inClose[],
                          int          *outBegIdx,
                          int          *outNBElement,
                          int           outInteger[] );

int TA_CDL3INSIDE_Lookback( void );


/*
 * TA_CDL3LINESTRIKE - Three-Line Strike 
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDL3LINESTRIKE( int    startIdx,
                              int    endIdx,
                              const double inOpen[],
                              const double inHigh[],
                              const double inLow[],
                              const double inClose[],
                              int          *outBegIdx,
                              int          *outNBElement,
                              int           outInteger[] );

int TA_CDL3LINESTRIKE_Lookback( void );


/*
 * TA_CDL3OUTSIDE - Three Outside Up/Down
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDL3OUTSIDE( int    startIdx,
                           int    endIdx,
                           const double inOpen[],
                           const double inHigh[],
                           const double inLow[],
                           const double inClose[],
                           int          *outBegIdx,
                           int          *outNBElement,
                           int           outInteger[] );

int TA_CDL3OUTSIDE_Lookback( void );


/*
 * TA_CDL3STARSINSOUTH - Three Stars In The South
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDL3STARSINSOUTH( int    startIdx,
                                int    endIdx,
                                const double inOpen[],
                                const double inHigh[],
                                const double inLow[],
                                const double inClose[],
                                int          *outBegIdx,
                                int          *outNBElement,
                                int           outInteger[] );

int TA_CDL3STARSINSOUTH_Lookback( void );


/*
 * TA_CDL3WHITESOLDIERS - Three Advancing White Soldiers
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDL3WHITESOLDIERS( int    startIdx,
                                 int    endIdx,
                                 const double inOpen[],
                                 const double inHigh[],
                                 const double inLow[],
                                 const double inClose[],
                                 int          *outBegIdx,
                                 int          *outNBElement,
                                 int           outInteger[] );

int TA_CDL3WHITESOLDIERS_Lookback( void );


/*
 * TA_CDLABANDONEDBABY - Abandoned Baby
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 * Optional Parameters
 * -------------------
 * optInPenetration:(From 0 to TA_REAL_MAX)
 *    Percentage of penetration of a candle within another candle
 * 
 * 
 */
TA_RetCode TA_CDLABANDONEDBABY( int    startIdx,
                                int    endIdx,
                                const double inOpen[],
                                const double inHigh[],
                                const double inLow[],
                                const double inClose[],
                                double        optInPenetration, /* From 0 to TA_REAL_MAX */
                                int          *outBegIdx,
                                int          *outNBElement,
                                int           outInteger[] );

int TA_CDLABANDONEDBABY_Lookback( double        optInPenetration );  /* From 0 to TA_REAL_MAX */


/*
 * TA_CDLADVANCEBLOCK - Advance Block
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLADVANCEBLOCK( int    startIdx,
                               int    endIdx,
                               const double inOpen[],
                               const double inHigh[],
                               const double inLow[],
                               const double inClose[],
                               int          *outBegIdx,
                               int          *outNBElement,
                               int           outInteger[] );

int TA_CDLADVANCEBLOCK_Lookback( void );


/*
 * TA_CDLBELTHOLD - Belt-hold
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLBELTHOLD( int    startIdx,
                           int    endIdx,
                           const double inOpen[],
                           const double inHigh[],
                           const double inLow[],
                           const double inClose[],
                           int          *outBegIdx,
                           int          *outNBElement,
                           int           outInteger[] );

int TA_CDLBELTHOLD_Lookback( void );


/*
 * TA_CDLBREAKAWAY - Breakaway
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLBREAKAWAY( int    startIdx,
                            int    endIdx,
                            const double inOpen[],
                            const double inHigh[],
                            const double inLow[],
                            const double inClose[],
                            int          *outBegIdx,
                            int          *outNBElement,
                            int           outInteger[] );

int TA_CDLBREAKAWAY_Lookback( void );


/*
 * TA_CDLCLOSINGMARUBOZU - Closing Marubozu
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLCLOSINGMARUBOZU( int    startIdx,
                                  int    endIdx,
                                  const double inOpen[],
                                  const double inHigh[],
                                  const double inLow[],
                                  const double inClose[],
                                  int          *outBegIdx,
                                  int          *outNBElement,
                                  int           outInteger[] );

int TA_CDLCLOSINGMARUBOZU_Lookback( void );


/*
 * TA_CDLCONCEALBABYSWALL - Concealing Baby Swallow
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLCONCEALBABYSWALL( int    startIdx,
                                   int    endIdx,
                                   const double inOpen[],
                                   const double inHigh[],
                                   const double inLow[],
                                   const double inClose[],
                                   int          *outBegIdx,
                                   int          *outNBElement,
                                   int           outInteger[] );

int TA_CDLCONCEALBABYSWALL_Lookback( void );


/*
 * TA_CDLCOUNTERATTACK - Counterattack
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLCOUNTERATTACK( int    startIdx,
                                int    endIdx,
                                const double inOpen[],
                                const double inHigh[],
                                const double inLow[],
                                const double inClose[],
                                int          *outBegIdx,
                                int          *outNBElement,
                                int           outInteger[] );

int TA_CDLCOUNTERATTACK_Lookback( void );


/*
 * TA_CDLDARKCLOUDCOVER - Dark Cloud Cover
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 * Optional Parameters
 * -------------------
 * optInPenetration:(From 0 to TA_REAL_MAX)
 *    Percentage of penetration of a candle within another candle
 * 
 * 
 */
TA_RetCode TA_CDLDARKCLOUDCOVER( int    startIdx,
                                 int    endIdx,
                                 const double inOpen[],
                                 const double inHigh[],
                                 const double inLow[],
                                 const double inClose[],
                                 double        optInPenetration, /* From 0 to TA_REAL_MAX */
                                 int          *outBegIdx,
                                 int          *outNBElement,
                                 int           outInteger[] );

int TA_CDLDARKCLOUDCOVER_Lookback( double        optInPenetration );  /* From 0 to TA_REAL_MAX */


/*
 * TA_CDLDOJI - Doji
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLDOJI( int    startIdx,
                       int    endIdx,
                       const double inOpen[],
                       const double inHigh[],
                       const double inLow[],
                       const double inClose[],
                       int          *outBegIdx,
                       int          *outNBElement,
                       int           outInteger[] );

int TA_CDLDOJI_Lookback( void );


/*
 * TA_CDLDOJISTAR - Doji Star
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLDOJISTAR( int    startIdx,
                           int    endIdx,
                           const double inOpen[],
                           const double inHigh[],
                           const double inLow[],
                           const double inClose[],
                           int          *outBegIdx,
                           int          *outNBElement,
                           int           outInteger[] );

int TA_CDLDOJISTAR_Lookback( void );


/*
 * TA_CDLDRAGONFLYDOJI - Dragonfly Doji
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLDRAGONFLYDOJI( int    startIdx,
                                int    endIdx,
                                const double inOpen[],
                                const double inHigh[],
                                const double inLow[],
                                const double inClose[],
                                int          *outBegIdx,
                                int          *outNBElement,
                                int           outInteger[] );
int TA_CDLDRAGONFLYDOJI_Lookback( void );


/*
 * TA_CDLENGULFING - Engulfing Pattern
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLENGULFING( int    startIdx,
                            int    endIdx,
                            const double inOpen[],
                            const double inHigh[],
                            const double inLow[],
                            const double inClose[],
                            int          *outBegIdx,
                            int          *outNBElement,
                            int           outInteger[] );

int TA_CDLENGULFING_Lookback( void );


/*
 * TA_CDLEVENINGDOJISTAR - Evening Doji Star
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 * Optional Parameters
 * -------------------
 * optInPenetration:(From 0 to TA_REAL_MAX)
 *    Percentage of penetration of a candle within another candle
 * 
 * 
 */
TA_RetCode TA_CDLEVENINGDOJISTAR( int    startIdx,
                                  int    endIdx,
                                  const double inOpen[],
                                  const double inHigh[],
                                  const double inLow[],
                                  const double inClose[],
                                  double        optInPenetration, /* From 0 to TA_REAL_MAX */
                                  int          *outBegIdx,
                                  int          *outNBElement,
                                  int           outInteger[] );

int TA_CDLEVENINGDOJISTAR_Lookback( double        optInPenetration );  /* From 0 to TA_REAL_MAX */


/*
 * TA_CDLEVENINGSTAR - Evening Star
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 * Optional Parameters
 * -------------------
 * optInPenetration:(From 0 to TA_REAL_MAX)
 *    Percentage of penetration of a candle within another candle
 * 
 * 
 */
TA_RetCode TA_CDLEVENINGSTAR( int    startIdx,
                              int    endIdx,
                              const double inOpen[],
                              const double inHigh[],
                              const double inLow[],
                              const double inClose[],
                              double        optInPenetration, /* From 0 to TA_REAL_MAX */
                              int          *outBegIdx,
                              int          *outNBElement,
                              int           outInteger[] );

int TA_CDLEVENINGSTAR_Lookback( double        optInPenetration );  /* From 0 to TA_REAL_MAX */


/*
 * TA_CDLGAPSIDESIDEWHITE - Up/Down-gap side-by-side white lines
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLGAPSIDESIDEWHITE( int    startIdx,
                                   int    endIdx,
                                   const double inOpen[],
                                   const double inHigh[],
                                   const double inLow[],
                                   const double inClose[],
                                   int          *outBegIdx,
                                   int          *outNBElement,
                                   int           outInteger[] );

int TA_CDLGAPSIDESIDEWHITE_Lookback( void );


/*
 * TA_CDLGRAVESTONEDOJI - Gravestone Doji
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLGRAVESTONEDOJI( int    startIdx,
                                 int    endIdx,
                                 const double inOpen[],
                                 const double inHigh[],
                                 const double inLow[],
                                 const double inClose[],
                                 int          *outBegIdx,
                                 int          *outNBElement,
                                 int           outInteger[] );

int TA_CDLGRAVESTONEDOJI_Lookback( void );


/*
 * TA_CDLHAMMER - Hammer
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLHAMMER( int    startIdx,
                         int    endIdx,
                         const double inOpen[],
                         const double inHigh[],
                         const double inLow[],
                         const double inClose[],
                         int          *outBegIdx,
                         int          *outNBElement,
                         int           outInteger[] );

int TA_CDLHAMMER_Lookback( void );


/*
 * TA_CDLHANGINGMAN - Hanging Man
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLHANGINGMAN( int    startIdx,
                             int    endIdx,
                             const double inOpen[],
                             const double inHigh[],
                             const double inLow[],
                             const double inClose[],
                             int          *outBegIdx,
                             int          *outNBElement,
                             int           outInteger[] );

int TA_CDLHANGINGMAN_Lookback( void );


/*
 * TA_CDLHARAMI - Harami Pattern
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLHARAMI( int    startIdx,
                         int    endIdx,
                         const double inOpen[],
                         const double inHigh[],
                         const double inLow[],
                         const double inClose[],
                         int          *outBegIdx,
                         int          *outNBElement,
                         int           outInteger[] );

int TA_CDLHARAMI_Lookback( void );


/*
 * TA_CDLHARAMICROSS - Harami Cross Pattern
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLHARAMICROSS( int    startIdx,
                              int    endIdx,
                              const double inOpen[],
                              const double inHigh[],
                              const double inLow[],
                              const double inClose[],
                              int          *outBegIdx,
                              int          *outNBElement,
                              int           outInteger[] );

int TA_CDLHARAMICROSS_Lookback( void );


/*
 * TA_CDLHIGHWAVE - High-Wave Candle
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLHIGHWAVE( int    startIdx,
                           int    endIdx,
                           const double inOpen[],
                           const double inHigh[],
                           const double inLow[],
                           const double inClose[],
                           int          *outBegIdx,
                           int          *outNBElement,
                           int           outInteger[] );

int TA_CDLHIGHWAVE_Lookback( void );


/*
 * TA_CDLHIKKAKE - Hikkake Pattern
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLHIKKAKE( int    startIdx,
                          int    endIdx,
                          const double inOpen[],
                          const double inHigh[],
                          const double inLow[],
                          const double inClose[],
                          int          *outBegIdx,
                          int          *outNBElement,
                          int           outInteger[] );

int TA_CDLHIKKAKE_Lookback( void );


/*
 * TA_CDLHIKKAKEMOD - Modified Hikkake Pattern
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLHIKKAKEMOD( int    startIdx,
                             int    endIdx,
                             const double inOpen[],
                             const double inHigh[],
                             const double inLow[],
                             const double inClose[],
                             int          *outBegIdx,
                             int          *outNBElement,
                             int           outInteger[] );

int TA_CDLHIKKAKEMOD_Lookback( void );


/*
 * TA_CDLHOMINGPIGEON - Homing Pigeon
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLHOMINGPIGEON( int    startIdx,
                               int    endIdx,
                               const double inOpen[],
                               const double inHigh[],
                               const double inLow[],
                               const double inClose[],
                               int          *outBegIdx,
                               int          *outNBElement,
                               int           outInteger[] );

int TA_CDLHOMINGPIGEON_Lookback( void );


/*
 * TA_CDLIDENTICAL3CROWS - Identical Three Crows
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLIDENTICAL3CROWS( int    startIdx,
                                  int    endIdx,
                                  const double inOpen[],
                                  const double inHigh[],
                                  const double inLow[],
                                  const double inClose[],
                                  int          *outBegIdx,
                                  int          *outNBElement,
                                  int           outInteger[] );

int TA_CDLIDENTICAL3CROWS_Lookback( void );


/*
 * TA_CDLINNECK - In-Neck Pattern
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLINNECK( int    startIdx,
                         int    endIdx,
                         const double inOpen[],
                         const double inHigh[],
                         const double inLow[],
                         const double inClose[],
                         int          *outBegIdx,
                         int          *outNBElement,
                         int           outInteger[] );

int TA_CDLINNECK_Lookback( void );


/*
 * TA_CDLINVERTEDHAMMER - Inverted Hammer
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLINVERTEDHAMMER( int    startIdx,
                                 int    endIdx,
                                 const double inOpen[],
                                 const double inHigh[],
                                 const double inLow[],
                                 const double inClose[],
                                 int          *outBegIdx,
                                 int          *outNBElement,
                                 int           outInteger[] );

int TA_CDLINVERTEDHAMMER_Lookback( void );


/*
 * TA_CDLKICKING - Kicking
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLKICKING( int    startIdx,
                          int    endIdx,
                          const double inOpen[],
                          const double inHigh[],
                          const double inLow[],
                          const double inClose[],
                          int          *outBegIdx,
                          int          *outNBElement,
                          int           outInteger[] );

int TA_CDLKICKING_Lookback( void );


/*
 * TA_CDLKICKINGBYLENGTH - Kicking - bull/bear determined by the longer marubozu
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLKICKINGBYLENGTH( int    startIdx,
                                  int    endIdx,
                                  const double inOpen[],
                                  const double inHigh[],
                                  const double inLow[],
                                  const double inClose[],
                                  int          *outBegIdx,
                                  int          *outNBElement,
                                  int           outInteger[] );

int TA_CDLKICKINGBYLENGTH_Lookback( void );


/*
 * TA_CDLLADDERBOTTOM - Ladder Bottom
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLLADDERBOTTOM( int    startIdx,
                               int    endIdx,
                               const double inOpen[],
                               const double inHigh[],
                               const double inLow[],
                               const double inClose[],
                               int          *outBegIdx,
                               int          *outNBElement,
                               int           outInteger[] );

int TA_CDLLADDERBOTTOM_Lookback( void );


/*
 * TA_CDLLONGLEGGEDDOJI - Long Legged Doji
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLLONGLEGGEDDOJI( int    startIdx,
                                 int    endIdx,
                                 const double inOpen[],
                                 const double inHigh[],
                                 const double inLow[],
                                 const double inClose[],
                                 int          *outBegIdx,
                                 int          *outNBElement,
                                 int           outInteger[] );

int TA_CDLLONGLEGGEDDOJI_Lookback( void );


/*
 * TA_CDLLONGLINE - Long Line Candle
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLLONGLINE( int    startIdx,
                           int    endIdx,
                           const double inOpen[],
                           const double inHigh[],
                           const double inLow[],
                           const double inClose[],
                           int          *outBegIdx,
                           int          *outNBElement,
                           int           outInteger[] );

int TA_CDLLONGLINE_Lookback( void );


/*
 * TA_CDLMARUBOZU - Marubozu
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLMARUBOZU( int    startIdx,
                           int    endIdx,
                           const double inOpen[],
                           const double inHigh[],
                           const double inLow[],
                           const double inClose[],
                           int          *outBegIdx,
                           int          *outNBElement,
                           int           outInteger[] );

int TA_CDLMARUBOZU_Lookback( void );


/*
 * TA_CDLMATCHINGLOW - Matching Low
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLMATCHINGLOW( int    startIdx,
                              int    endIdx,
                              const double inOpen[],
                              const double inHigh[],
                              const double inLow[],
                              const double inClose[],
                              int          *outBegIdx,
                              int          *outNBElement,
                              int           outInteger[] );

int TA_CDLMATCHINGLOW_Lookback( void );


/*
 * TA_CDLMATHOLD - Mat Hold
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 * Optional Parameters
 * -------------------
 * optInPenetration:(From 0 to TA_REAL_MAX)
 *    Percentage of penetration of a candle within another candle
 * 
 * 
 */
TA_RetCode TA_CDLMATHOLD( int    startIdx,
                          int    endIdx,
                          const double inOpen[],
                          const double inHigh[],
                          const double inLow[],
                          const double inClose[],
                          double        optInPenetration, /* From 0 to TA_REAL_MAX */
                          int          *outBegIdx,
                          int          *outNBElement,
                          int           outInteger[] );

int TA_CDLMATHOLD_Lookback( double        optInPenetration );  /* From 0 to TA_REAL_MAX */


/*
 * TA_CDLMORNINGDOJISTAR - Morning Doji Star
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 * Optional Parameters
 * -------------------
 * optInPenetration:(From 0 to TA_REAL_MAX)
 *    Percentage of penetration of a candle within another candle
 * 
 * 
 */
TA_RetCode TA_CDLMORNINGDOJISTAR( int    startIdx,
                                  int    endIdx,
                                  const double inOpen[],
                                  const double inHigh[],
                                  const double inLow[],
                                  const double inClose[],
                                  double        optInPenetration, /* From 0 to TA_REAL_MAX */
                                  int          *outBegIdx,
                                  int          *outNBElement,
                                  int           outInteger[] );

int TA_CDLMORNINGDOJISTAR_Lookback( double        optInPenetration );  /* From 0 to TA_REAL_MAX */


/*
 * TA_CDLMORNINGSTAR - Morning Star
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 * Optional Parameters
 * -------------------
 * optInPenetration:(From 0 to TA_REAL_MAX)
 *    Percentage of penetration of a candle within another candle
 * 
 * 
 */
TA_RetCode TA_CDLMORNINGSTAR( int    startIdx,
                              int    endIdx,
                              const double inOpen[],
                              const double inHigh[],
                              const double inLow[],
                              const double inClose[],
                              double        optInPenetration, /* From 0 to TA_REAL_MAX */
                              int          *outBegIdx,
                              int          *outNBElement,
                              int           outInteger[] );

int TA_CDLMORNINGSTAR_Lookback( double        optInPenetration );  /* From 0 to TA_REAL_MAX */


/*
 * TA_CDLONNECK - On-Neck Pattern
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLONNECK( int    startIdx,
                         int    endIdx,
                         const double inOpen[],
                         const double inHigh[],
                         const double inLow[],
                         const double inClose[],
                         int          *outBegIdx,
                         int          *outNBElement,
                         int           outInteger[] );

int TA_CDLONNECK_Lookback( void );


/*
 * TA_CDLPIERCING - Piercing Pattern
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLPIERCING( int    startIdx,
                           int    endIdx,
                           const double inOpen[],
                           const double inHigh[],
                           const double inLow[],
                           const double inClose[],
                           int          *outBegIdx,
                           int          *outNBElement,
                           int           outInteger[] );

int TA_CDLPIERCING_Lookback( void );


/*
 * TA_CDLRICKSHAWMAN - Rickshaw Man
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLRICKSHAWMAN( int    startIdx,
                              int    endIdx,
                              const double inOpen[],
                              const double inHigh[],
                              const double inLow[],
                              const double inClose[],
                              int          *outBegIdx,
                              int          *outNBElement,
                              int           outInteger[] );

int TA_CDLRICKSHAWMAN_Lookback( void );


/*
 * TA_CDLRISEFALL3METHODS - Rising/Falling Three Methods
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLRISEFALL3METHODS( int    startIdx,
                                   int    endIdx,
                                   const double inOpen[],
                                   const double inHigh[],
                                   const double inLow[],
                                   const double inClose[],
                                   int          *outBegIdx,
                                   int          *outNBElement,
                                   int           outInteger[] );

int TA_CDLRISEFALL3METHODS_Lookback( void );


/*
 * TA_CDLSEPARATINGLINES - Separating Lines
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLSEPARATINGLINES( int    startIdx,
                                  int    endIdx,
                                  const double inOpen[],
                                  const double inHigh[],
                                  const double inLow[],
                                  const double inClose[],
                                  int          *outBegIdx,
                                  int          *outNBElement,
                                  int           outInteger[] );

int TA_CDLSEPARATINGLINES_Lookback( void );


/*
 * TA_CDLSHOOTINGSTAR - Shooting Star
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLSHOOTINGSTAR( int    startIdx,
                               int    endIdx,
                               const double inOpen[],
                               const double inHigh[],
                               const double inLow[],
                               const double inClose[],
                               int          *outBegIdx,
                               int          *outNBElement,
                               int           outInteger[] );

int TA_CDLSHOOTINGSTAR_Lookback( void );


/*
 * TA_CDLSHORTLINE - Short Line Candle
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLSHORTLINE( int    startIdx,
                            int    endIdx,
                            const double inOpen[],
                            const double inHigh[],
                            const double inLow[],
                            const double inClose[],
                            int          *outBegIdx,
                            int          *outNBElement,
                            int           outInteger[] );

int TA_CDLSHORTLINE_Lookback( void );


/*
 * TA_CDLSPINNINGTOP - Spinning Top
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLSPINNINGTOP( int    startIdx,
                              int    endIdx,
                              const double inOpen[],
                              const double inHigh[],
                              const double inLow[],
                              const double inClose[],
                              int          *outBegIdx,
                              int          *outNBElement,
                              int           outInteger[] );

int TA_CDLSPINNINGTOP_Lookback( void );


/*
 * TA_CDLSTALLEDPATTERN - Stalled Pattern
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLSTALLEDPATTERN( int    startIdx,
                                 int    endIdx,
                                 const double inOpen[],
                                 const double inHigh[],
                                 const double inLow[],
                                 const double inClose[],
                                 int          *outBegIdx,
                                 int          *outNBElement,
                                 int           outInteger[] );

int TA_CDLSTALLEDPATTERN_Lookback( void );


/*
 * TA_CDLSTICKSANDWICH - Stick Sandwich
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLSTICKSANDWICH( int    startIdx,
                                int    endIdx,
                                const double inOpen[],
                                const double inHigh[],
                                const double inLow[],
                                const double inClose[],
                                int          *outBegIdx,
                                int          *outNBElement,
                                int           outInteger[] );

int TA_CDLSTICKSANDWICH_Lookback( void );


/*
 * TA_CDLTAKURI - Takuri (Dragonfly Doji with very long lower shadow)
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLTAKURI( int    startIdx,
                         int    endIdx,
                         const double inOpen[],
                         const double inHigh[],
                         const double inLow[],
                         const double inClose[],
                         int          *outBegIdx,
                         int          *outNBElement,
                         int           outInteger[] );

int TA_CDLTAKURI_Lookback( void );


/*
 * TA_CDLTASUKIGAP - Tasuki Gap
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLTASUKIGAP( int    startIdx,
                            int    endIdx,
                            const double inOpen[],
                            const double inHigh[],
                            const double inLow[],
                            const double inClose[],
                            int          *outBegIdx,
                            int          *outNBElement,
                            int           outInteger[] );

int TA_CDLTASUKIGAP_Lookback( void );


/*
 * TA_CDLTHRUSTING - Thrusting Pattern
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLTHRUSTING( int    startIdx,
                            int    endIdx,
                            const double inOpen[],
                            const double inHigh[],
                            const double inLow[],
                            const double inClose[],
                            int          *outBegIdx,
                            int          *outNBElement,
                            int           outInteger[] );

int TA_CDLTHRUSTING_Lookback( void );


/*
 * TA_CDLTRISTAR - Tristar Pattern
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLTRISTAR( int    startIdx,
                          int    endIdx,
                          const double inOpen[],
                          const double inHigh[],
                          const double inLow[],
                          const double inClose[],
                          int          *outBegIdx,
                          int          *outNBElement,
                          int           outInteger[] );

int TA_CDLTRISTAR_Lookback( void );


/*
 * TA_CDLUNIQUE3RIVER - Unique 3 River
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLUNIQUE3RIVER( int    startIdx,
                               int    endIdx,
                               const double inOpen[],
                               const double inHigh[],
                               const double inLow[],
                               const double inClose[],
                               int          *outBegIdx,
                               int          *outNBElement,
                               int           outInteger[] );

int TA_CDLUNIQUE3RIVER_Lookback( void );


/*
 * TA_CDLUPSIDEGAP2CROWS - Upside Gap Two Crows
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLUPSIDEGAP2CROWS( int    startIdx,
                                  int    endIdx,
                                  const double inOpen[],
                                  const double inHigh[],
                                  const double inLow[],
                                  const double inClose[],
                                  int          *outBegIdx,
                                  int          *outNBElement,
                                  int           outInteger[] );

int TA_CDLUPSIDEGAP2CROWS_Lookback( void );


/*
 * TA_CDLXSIDEGAP3METHODS - Upside/Downside Gap Three Methods
 * 
 * Input  = Open, High, Low, Close
 * Output = int
 * 
 */
TA_RetCode TA_CDLXSIDEGAP3METHODS( int    startIdx,
                                   int    endIdx,
                                   const double inOpen[],
                                   const double inHigh[],
                                   const double inLow[],
                                   const double inClose[],
                                   int          *outBegIdx,
                                   int          *outNBElement,
                                   int           outInteger[] );

int TA_CDLXSIDEGAP3METHODS_Lookback( void );


/*
 * TA_CEIL - Vector Ceil
 * 
 * Input  = double
 * Output = double
 * 
 */
TA_RetCode TA_CEIL( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );

int TA_CEIL_Lookback( void );


/*
 * TA_CMO - Chande Momentum Oscillator
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_CMO( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInTimePeriod, /* From 2 to 100000 */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_CMO_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_CORREL - Pearson's Correlation Coefficient (r)
 * 
 * Input  = double, double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_CORREL( int    startIdx,
                      int    endIdx,
                      const double inReal0[],
                      const double inReal1[],
                      int           optInTimePeriod, /* From 1 to 100000 */
                      int          *outBegIdx,
                      int          *outNBElement,
                      double        outReal[] );

int TA_CORREL_Lookback( int           optInTimePeriod );  /* From 1 to 100000 */


/*
 * TA_COS - Vector Trigonometric Cos
 * 
 * Input  = double
 * Output = double
 * 
 */
TA_RetCode TA_COS( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_COS_Lookback( void );


/*
 * TA_COSH - Vector Trigonometric Cosh
 * 
 * Input  = double
 * Output = double
 * 
 */
TA_RetCode TA_COSH( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );

int TA_COSH_Lookback( void );


/*
 * TA_DEMA - Double Exponential Moving Average
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_DEMA( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int           optInTimePeriod, /* From 2 to 100000 */
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );

int TA_DEMA_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_DIV - Vector Arithmetic Div
 * 
 * Input  = double, double
 * Output = double
 * 
 */
TA_RetCode TA_DIV( int    startIdx,
                   int    endIdx,
                   const double inReal0[],
                   const double inReal1[],
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_DIV_Lookback( void );


/*
 * TA_DX - Directional Movement Index
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
TA_RetCode TA_DX( int    startIdx,
                  int    endIdx,
                  const double inHigh[],
                  const double inLow[],
                  const double inClose[],
                  int           optInTimePeriod, /* From 2 to 100000 */
                  int          *outBegIdx,
                  int          *outNBElement,
                  double        outReal[] );

int TA_DX_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_EMA - Exponential Moving Average
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_EMA( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInTimePeriod, /* From 2 to 100000 */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_EMA_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_EXP - Vector Arithmetic Exp
 * 
 * Input  = double
 * Output = double
 * 
 */
TA_RetCode TA_EXP( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_EXP_Lookback( void );


/*
 * TA_FLOOR - Vector Floor
 * 
 * Input  = double
 * Output = double
 * 
 */
TA_RetCode TA_FLOOR( int    startIdx,
                     int    endIdx,
                     const double inReal[],
                     int          *outBegIdx,
                     int          *outNBElement,
                     double        outReal[] );

int TA_FLOOR_Lookback( void );


/*
 * TA_HT_DCPERIOD - Hilbert Transform - Dominant Cycle Period
 * 
 * Input  = double
 * Output = double
 * 
 */
TA_RetCode TA_HT_DCPERIOD( int    startIdx,
                           int    endIdx,
                           const double inReal[],
                           int          *outBegIdx,
                           int          *outNBElement,
                           double        outReal[] );

int TA_HT_DCPERIOD_Lookback( void );


/*
 * TA_HT_DCPHASE - Hilbert Transform - Dominant Cycle Phase
 * 
 * Input  = double
 * Output = double
 * 
 */
TA_RetCode TA_HT_DCPHASE( int    startIdx,
                          int    endIdx,
                          const double inReal[],
                          int          *outBegIdx,
                          int          *outNBElement,
                          double        outReal[] );

int TA_HT_DCPHASE_Lookback( void );


/*
 * TA_HT_PHASOR - Hilbert Transform - Phasor Components
 * 
 * Input  = double
 * Output = double, double
 * 
 */
TA_RetCode TA_HT_PHASOR( int    startIdx,
                         int    endIdx,
                         const double inReal[],
                         int          *outBegIdx,
                         int          *outNBElement,
                         double        outInPhase[],
                         double        outQuadrature[] );

int TA_HT_PHASOR_Lookback( void );


/*
 * TA_HT_SINE - Hilbert Transform - SineWave
 * 
 * Input  = double
 * Output = double, double
 * 
 */
TA_RetCode TA_HT_SINE( int    startIdx,
                       int    endIdx,
                       const double inReal[],
                       int          *outBegIdx,
                       int          *outNBElement,
                       double        outSine[],
                       double        outLeadSine[] );

int TA_HT_SINE_Lookback( void );


/*
 * TA_HT_TRENDLINE - Hilbert Transform - Instantaneous Trendline
 * 
 * Input  = double
 * Output = double
 * 
 */
TA_RetCode TA_HT_TRENDLINE( int    startIdx,
                            int    endIdx,
                            const double inReal[],
                            int          *outBegIdx,
                            int          *outNBElement,
                            double        outReal[] );

int TA_HT_TRENDLINE_Lookback( void );


/*
 * TA_HT_TRENDMODE - Hilbert Transform - Trend vs Cycle Mode
 * 
 * Input  = double
 * Output = int
 * 
 */
TA_RetCode TA_HT_TRENDMODE( int    startIdx,
                            int    endIdx,
                            const double inReal[],
                            int          *outBegIdx,
                            int          *outNBElement,
                            int           outInteger[] );

int TA_HT_TRENDMODE_Lookback( void );


/*
 * TA_IMI - Intraday Momentum Index
 * 
 * Input  = Open, Close
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_IMI(  int    startIdx,
                    int    endIdx,
                    const double inOpen[],
                    const double inClose[],
                    int           optInTimePeriod, /* From 2 to 100000 */
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );

int TA_IMI_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_KAMA - Kaufman Adaptive Moving Average
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_KAMA( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int           optInTimePeriod, /* From 2 to 100000 */
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );

int TA_KAMA_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_LINEARREG - Linear Regression
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_LINEARREG( int    startIdx,
                         int    endIdx,
                         const double inReal[],
                         int           optInTimePeriod, /* From 2 to 100000 */
                         int          *outBegIdx,
                         int          *outNBElement,
                         double        outReal[] );

int TA_LINEARREG_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_LINEARREG_ANGLE - Linear Regression Angle
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_LINEARREG_ANGLE( int    startIdx,
                               int    endIdx,
                               const double inReal[],
                               int           optInTimePeriod, /* From 2 to 100000 */
                               int          *outBegIdx,
                               int          *outNBElement,
                               double        outReal[] );

int TA_LINEARREG_ANGLE_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_LINEARREG_INTERCEPT - Linear Regression Intercept
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_LINEARREG_INTERCEPT( int    startIdx,
                                   int    endIdx,
                                   const double inReal[],
                                   int           optInTimePeriod, /* From 2 to 100000 */
                                   int          *outBegIdx,
                                   int          *outNBElement,
                                   double        outReal[] );

int TA_LINEARREG_INTERCEPT_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_LINEARREG_SLOPE - Linear Regression Slope
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_LINEARREG_SLOPE( int    startIdx,
                               int    endIdx,
                               const double inReal[],
                               int           optInTimePeriod, /* From 2 to 100000 */
                               int          *outBegIdx,
                               int          *outNBElement,
                               double        outReal[] );

int TA_LINEARREG_SLOPE_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_LN - Vector Log Natural
 * 
 * Input  = double
 * Output = double
 * 
 */
TA_RetCode TA_LN( int    startIdx,
                  int    endIdx,
                  const double inReal[],
                  int          *outBegIdx,
                  int          *outNBElement,
                  double        outReal[] );

int TA_LN_Lookback( void );


/*
 * TA_LOG10 - Vector Log10
 * 
 * Input  = double
 * Output = double
 * 
 */
TA_RetCode TA_LOG10( int    startIdx,
                     int    endIdx,
                     const double inReal[],
                     int          *outBegIdx,
                     int          *outNBElement,
                     double        outReal[] );

int TA_LOG10_Lookback( void );


/*
 * TA_MA - Moving average
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * optInMAType:
 *    Type of Moving Average
 * 
 * 
 */
TA_RetCode TA_MA( int    startIdx,
                  int    endIdx,
                  const double inReal[],
                  int           optInTimePeriod, /* From 1 to 100000 */
                  TA_MAType     optInMAType,
                  int          *outBegIdx,
                  int          *outNBElement,
                  double        outReal[] );

int TA_MA_Lookback( int           optInTimePeriod, /* From 1 to 100000 */
                  TA_MAType     optInMAType ); 

/*
 * TA_MACD - Moving Average Convergence/Divergence
 * 
 * Input  = double
 * Output = double, double, double
 * 
 * Optional Parameters
 * -------------------
 * optInFastPeriod:(From 2 to 100000)
 *    Number of period for the fast MA
 * 
 * optInSlowPeriod:(From 2 to 100000)
 *    Number of period for the slow MA
 * 
 * optInSignalPeriod:(From 1 to 100000)
 *    Smoothing for the signal line (nb of period)
 * 
 * 
 */
TA_RetCode TA_MACD( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int           optInFastPeriod, /* From 2 to 100000 */
                    int           optInSlowPeriod, /* From 2 to 100000 */
                    int           optInSignalPeriod, /* From 1 to 100000 */
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outMACD[],
                    double        outMACDSignal[],
                    double        outMACDHist[] );

int TA_MACD_Lookback( int           optInFastPeriod, /* From 2 to 100000 */
                    int           optInSlowPeriod, /* From 2 to 100000 */
                    int           optInSignalPeriod );  /* From 1 to 100000 */


/*
 * TA_MACDEXT - MACD with controllable MA type
 * 
 * Input  = double
 * Output = double, double, double
 * 
 * Optional Parameters
 * -------------------
 * optInFastPeriod:(From 2 to 100000)
 *    Number of period for the fast MA
 * 
 * optInFastMAType:
 *    Type of Moving Average for fast MA
 * 
 * optInSlowPeriod:(From 2 to 100000)
 *    Number of period for the slow MA
 * 
 * optInSlowMAType:
 *    Type of Moving Average for slow MA
 * 
 * optInSignalPeriod:(From 1 to 100000)
 *    Smoothing for the signal line (nb of period)
 * 
 * optInSignalMAType:
 *    Type of Moving Average for signal line
 * 
 * 
 */
TA_RetCode TA_MACDEXT( int    startIdx,
                       int    endIdx,
                       const double inReal[],
                       int           optInFastPeriod, /* From 2 to 100000 */
                       TA_MAType     optInFastMAType,
                       int           optInSlowPeriod, /* From 2 to 100000 */
                       TA_MAType     optInSlowMAType,
                       int           optInSignalPeriod, /* From 1 to 100000 */
                       TA_MAType     optInSignalMAType,
                       int          *outBegIdx,
                       int          *outNBElement,
                       double        outMACD[],
                       double        outMACDSignal[],
                       double        outMACDHist[] );

int TA_MACDEXT_Lookback( int           optInFastPeriod, /* From 2 to 100000 */
                       TA_MAType     optInFastMAType,
                       int           optInSlowPeriod, /* From 2 to 100000 */
                       TA_MAType     optInSlowMAType,
                       int           optInSignalPeriod, /* From 1 to 100000 */
                       TA_MAType     optInSignalMAType ); 

/*
 * TA_MACDFIX - Moving Average Convergence/Divergence Fix 12/26
 * 
 * Input  = double
 * Output = double, double, double
 * 
 * Optional Parameters
 * -------------------
 * optInSignalPeriod:(From 1 to 100000)
 *    Smoothing for the signal line (nb of period)
 * 
 * 
 */
TA_RetCode TA_MACDFIX( int    startIdx,
                       int    endIdx,
                       const double inReal[],
                       int           optInSignalPeriod, /* From 1 to 100000 */
                       int          *outBegIdx,
                       int          *outNBElement,
                       double        outMACD[],
                       double        outMACDSignal[],
                       double        outMACDHist[] );

int TA_MACDFIX_Lookback( int           optInSignalPeriod );  /* From 1 to 100000 */


/*
 * TA_MAMA - MESA Adaptive Moving Average
 * 
 * Input  = double
 * Output = double, double
 * 
 * Optional Parameters
 * -------------------
 * optInFastLimit:(From 0.01 to 0.99)
 *    Upper limit use in the adaptive algorithm
 * 
 * optInSlowLimit:(From 0.01 to 0.99)
 *    Lower limit use in the adaptive algorithm
 * 
 * 
 */
TA_RetCode TA_MAMA( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    double        optInFastLimit, /* From 0.01 to 0.99 */
                    double        optInSlowLimit, /* From 0.01 to 0.99 */
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outMAMA[],
                    double        outFAMA[] );

int TA_MAMA_Lookback( double        optInFastLimit, /* From 0.01 to 0.99 */
                    double        optInSlowLimit );  /* From 0.01 to 0.99 */


/*
 * TA_MAVP - Moving average with variable period
 * 
 * Input  = double, double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInMinPeriod:(From 2 to 100000)
 *    Value less than minimum will be changed to Minimum period
 * 
 * optInMaxPeriod:(From 2 to 100000)
 *    Value higher than maximum will be changed to Maximum period
 * 
 * optInMAType:
 *    Type of Moving Average
 * 
 * 
 */
TA_RetCode TA_MAVP( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    const double inPeriods[],
                    int           optInMinPeriod, /* From 2 to 100000 */
                    int           optInMaxPeriod, /* From 2 to 100000 */
                    TA_MAType     optInMAType,
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );

int TA_MAVP_Lookback( int           optInMinPeriod, /* From 2 to 100000 */
                    int           optInMaxPeriod, /* From 2 to 100000 */
                    TA_MAType     optInMAType ); 

/*
 * TA_MAX - Highest value over a specified period
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_MAX( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInTimePeriod, /* From 2 to 100000 */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_MAX_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_MAXINDEX - Index of highest value over a specified period
 * 
 * Input  = double
 * Output = int
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_MAXINDEX( int    startIdx,
                        int    endIdx,
                        const double inReal[],
                        int           optInTimePeriod, /* From 2 to 100000 */
                        int          *outBegIdx,
                        int          *outNBElement,
                        int           outInteger[] );

int TA_MAXINDEX_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_MEDPRICE - Median Price
 * 
 * Input  = High, Low
 * Output = double
 * 
 */
TA_RetCode TA_MEDPRICE( int    startIdx,
                        int    endIdx,
                        const double inHigh[],
                        const double inLow[],
                        int          *outBegIdx,
                        int          *outNBElement,
                        double        outReal[] );

int TA_MEDPRICE_Lookback( void );


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
                   double        outReal[] );

int TA_MFI_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_MIDPOINT - MidPoint over period
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_MIDPOINT( int    startIdx,
                        int    endIdx,
                        const double inReal[],
                        int           optInTimePeriod, /* From 2 to 100000 */
                        int          *outBegIdx,
                        int          *outNBElement,
                        double        outReal[] );

int TA_MIDPOINT_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_MIDPRICE - Midpoint Price over period
 * 
 * Input  = High, Low
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_MIDPRICE( int    startIdx,
                        int    endIdx,
                        const double inHigh[],
                        const double inLow[],
                        int           optInTimePeriod, /* From 2 to 100000 */
                        int          *outBegIdx,
                        int          *outNBElement,
                        double        outReal[] );

int TA_MIDPRICE_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_MIN - Lowest value over a specified period
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_MIN( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInTimePeriod, /* From 2 to 100000 */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_MIN_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_MININDEX - Index of lowest value over a specified period
 * 
 * Input  = double
 * Output = int
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_MININDEX( int    startIdx,
                        int    endIdx,
                        const double inReal[],
                        int           optInTimePeriod, /* From 2 to 100000 */
                        int          *outBegIdx,
                        int          *outNBElement,
                        int           outInteger[] );

int TA_MININDEX_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_MINMAX - Lowest and highest values over a specified period
 * 
 * Input  = double
 * Output = double, double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_MINMAX( int    startIdx,
                      int    endIdx,
                      const double inReal[],
                      int           optInTimePeriod, /* From 2 to 100000 */
                      int          *outBegIdx,
                      int          *outNBElement,
                      double        outMin[],
                      double        outMax[] );

int TA_MINMAX_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_MINMAXINDEX - Indexes of lowest and highest values over a specified period
 * 
 * Input  = double
 * Output = int, int
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_MINMAXINDEX( int    startIdx,
                           int    endIdx,
                           const double inReal[],
                           int           optInTimePeriod, /* From 2 to 100000 */
                           int          *outBegIdx,
                           int          *outNBElement,
                           int           outMinIdx[],
                           int           outMaxIdx[] );

int TA_MINMAXINDEX_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_MINUS_DI - Minus Directional Indicator
 * 
 * Input  = High, Low, Close
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_MINUS_DI( int    startIdx,
                        int    endIdx,
                        const double inHigh[],
                        const double inLow[],
                        const double inClose[],
                        int           optInTimePeriod, /* From 1 to 100000 */
                        int          *outBegIdx,
                        int          *outNBElement,
                        double        outReal[] );

int TA_MINUS_DI_Lookback( int           optInTimePeriod );  /* From 1 to 100000 */


/*
 * TA_MINUS_DM - Minus Directional Movement
 * 
 * Input  = High, Low
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_MINUS_DM( int    startIdx,
                        int    endIdx,
                        const double inHigh[],
                        const double inLow[],
                        int           optInTimePeriod, /* From 1 to 100000 */
                        int          *outBegIdx,
                        int          *outNBElement,
                        double        outReal[] );

int TA_MINUS_DM_Lookback( int           optInTimePeriod );  /* From 1 to 100000 */


/*
 * TA_MOM - Momentum
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_MOM( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInTimePeriod, /* From 1 to 100000 */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_MOM_Lookback( int           optInTimePeriod );  /* From 1 to 100000 */


/*
 * TA_MULT - Vector Arithmetic Mult
 * 
 * Input  = double, double
 * Output = double
 * 
 */
TA_RetCode TA_MULT( int    startIdx,
                    int    endIdx,
                    const double inReal0[],
                    const double inReal1[],
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );

int TA_MULT_Lookback( void );


/*
 * TA_NATR - Normalized Average True Range
 * 
 * Input  = High, Low, Close
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_NATR( int    startIdx,
                    int    endIdx,
                    const double inHigh[],
                    const double inLow[],
                    const double inClose[],
                    int           optInTimePeriod, /* From 1 to 100000 */
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );

int TA_NATR_Lookback( int           optInTimePeriod );  /* From 1 to 100000 */


/*
 * TA_OBV - On Balance Volume
 * 
 * Input  = double, Volume
 * Output = double
 * 
 */
TA_RetCode TA_OBV( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   const double inVolume[],
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_OBV_Lookback( void );


/*
 * TA_PLUS_DI - Plus Directional Indicator
 * 
 * Input  = High, Low, Close
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_PLUS_DI( int    startIdx,
                       int    endIdx,
                       const double inHigh[],
                       const double inLow[],
                       const double inClose[],
                       int           optInTimePeriod, /* From 1 to 100000 */
                       int          *outBegIdx,
                       int          *outNBElement,
                       double        outReal[] );

int TA_PLUS_DI_Lookback( int           optInTimePeriod );  /* From 1 to 100000 */


/*
 * TA_PLUS_DM - Plus Directional Movement
 * 
 * Input  = High, Low
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_PLUS_DM( int    startIdx,
                       int    endIdx,
                       const double inHigh[],
                       const double inLow[],
                       int           optInTimePeriod, /* From 1 to 100000 */
                       int          *outBegIdx,
                       int          *outNBElement,
                       double        outReal[] );

int TA_PLUS_DM_Lookback( int           optInTimePeriod );  /* From 1 to 100000 */


/*
 * TA_PPO - Percentage Price Oscillator
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInFastPeriod:(From 2 to 100000)
 *    Number of period for the fast MA
 * 
 * optInSlowPeriod:(From 2 to 100000)
 *    Number of period for the slow MA
 * 
 * optInMAType:
 *    Type of Moving Average
 * 
 * 
 */
TA_RetCode TA_PPO( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInFastPeriod, /* From 2 to 100000 */
                   int           optInSlowPeriod, /* From 2 to 100000 */
                   TA_MAType     optInMAType,
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_PPO_Lookback( int           optInFastPeriod, /* From 2 to 100000 */
                   int           optInSlowPeriod, /* From 2 to 100000 */
                   TA_MAType     optInMAType ); 

/*
 * TA_ROC - Rate of change : ((price/prevPrice)-1)*100
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_ROC( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInTimePeriod, /* From 1 to 100000 */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_ROC_Lookback( int           optInTimePeriod );  /* From 1 to 100000 */


/*
 * TA_ROCP - Rate of change Percentage: (price-prevPrice)/prevPrice
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_ROCP( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int           optInTimePeriod, /* From 1 to 100000 */
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );

int TA_ROCP_Lookback( int           optInTimePeriod );  /* From 1 to 100000 */


/*
 * TA_ROCR - Rate of change ratio: (price/prevPrice)
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_ROCR( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int           optInTimePeriod, /* From 1 to 100000 */
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );

int TA_ROCR_Lookback( int           optInTimePeriod );  /* From 1 to 100000 */


/*
 * TA_ROCR100 - Rate of change ratio 100 scale: (price/prevPrice)*100
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_ROCR100( int    startIdx,
                       int    endIdx,
                       const double inReal[],
                       int           optInTimePeriod, /* From 1 to 100000 */
                       int          *outBegIdx,
                       int          *outNBElement,
                       double        outReal[] );

int TA_ROCR100_Lookback( int           optInTimePeriod );  /* From 1 to 100000 */


/*
 * TA_RSI - Relative Strength Index
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_RSI( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInTimePeriod, /* From 2 to 100000 */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_RSI_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_SAR - Parabolic SAR
 * 
 * Input  = High, Low
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInAcceleration:(From 0 to TA_REAL_MAX)
 *    Acceleration Factor used up to the Maximum value
 * 
 * optInMaximum:(From 0 to TA_REAL_MAX)
 *    Acceleration Factor Maximum value
 * 
 * 
 */
TA_RetCode TA_SAR( int    startIdx,
                   int    endIdx,
                   const double inHigh[],
                   const double inLow[],
                   double        optInAcceleration, /* From 0 to TA_REAL_MAX */
                   double        optInMaximum, /* From 0 to TA_REAL_MAX */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_SAR_Lookback( double        optInAcceleration, /* From 0 to TA_REAL_MAX */
                   double        optInMaximum );  /* From 0 to TA_REAL_MAX */


/*
 * TA_SAREXT - Parabolic SAR - Extended
 * 
 * Input  = High, Low
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInStartValue:(From TA_REAL_MIN to TA_REAL_MAX)
 *    Start value and direction. 0 for Auto, >0 for Long, <0 for Short
 * 
 * optInOffsetOnReverse:(From 0 to TA_REAL_MAX)
 *    Percent offset added/removed to initial stop on short/long reversal
 * 
 * optInAccelerationInitLong:(From 0 to TA_REAL_MAX)
 *    Acceleration Factor initial value for the Long direction
 * 
 * optInAccelerationLong:(From 0 to TA_REAL_MAX)
 *    Acceleration Factor for the Long direction
 * 
 * optInAccelerationMaxLong:(From 0 to TA_REAL_MAX)
 *    Acceleration Factor maximum value for the Long direction
 * 
 * optInAccelerationInitShort:(From 0 to TA_REAL_MAX)
 *    Acceleration Factor initial value for the Short direction
 * 
 * optInAccelerationShort:(From 0 to TA_REAL_MAX)
 *    Acceleration Factor for the Short direction
 * 
 * optInAccelerationMaxShort:(From 0 to TA_REAL_MAX)
 *    Acceleration Factor maximum value for the Short direction
 * 
 * 
 */
TA_RetCode TA_SAREXT( int    startIdx,
                      int    endIdx,
                      const double inHigh[],
                      const double inLow[],
                      double        optInStartValue, /* From TA_REAL_MIN to TA_REAL_MAX */
                      double        optInOffsetOnReverse, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationInitLong, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationLong, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationMaxLong, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationInitShort, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationShort, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationMaxShort, /* From 0 to TA_REAL_MAX */
                      int          *outBegIdx,
                      int          *outNBElement,
                      double        outReal[] );

int TA_SAREXT_Lookback( double        optInStartValue, /* From TA_REAL_MIN to TA_REAL_MAX */
                      double        optInOffsetOnReverse, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationInitLong, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationLong, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationMaxLong, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationInitShort, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationShort, /* From 0 to TA_REAL_MAX */
                      double        optInAccelerationMaxShort );  /* From 0 to TA_REAL_MAX */


/*
 * TA_SIN - Vector Trigonometric Sin
 * 
 * Input  = double
 * Output = double
 * 
 */
TA_RetCode TA_SIN( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_SIN_Lookback( void );


/*
 * TA_SINH - Vector Trigonometric Sinh
 * 
 * Input  = double
 * Output = double
 * 
 */
TA_RetCode TA_SINH( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );

int TA_SINH_Lookback( void );


/*
 * TA_SMA - Simple Moving Average
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_SMA( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInTimePeriod, /* From 2 to 100000 */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_SMA_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_SQRT - Vector Square Root
 * 
 * Input  = double
 * Output = double
 * 
 */
TA_RetCode TA_SQRT( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );

int TA_SQRT_Lookback( void );


/*
 * TA_STDDEV - Standard Deviation
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * optInNbDev:(From TA_REAL_MIN to TA_REAL_MAX)
 *    Nb of deviations
 * 
 * 
 */
TA_RetCode TA_STDDEV( int    startIdx,
                      int    endIdx,
                      const double inReal[],
                      int           optInTimePeriod, /* From 2 to 100000 */
                      double        optInNbDev, /* From TA_REAL_MIN to TA_REAL_MAX */
                      int          *outBegIdx,
                      int          *outNBElement,
                      double        outReal[] );

int TA_STDDEV_Lookback( int           optInTimePeriod, /* From 2 to 100000 */
                      double        optInNbDev );  /* From TA_REAL_MIN to TA_REAL_MAX */


/*
 * TA_STOCH - Stochastic
 * 
 * Input  = High, Low, Close
 * Output = double, double
 * 
 * Optional Parameters
 * -------------------
 * optInFastK_Period:(From 1 to 100000)
 *    Time period for building the Fast-K line
 * 
 * optInSlowK_Period:(From 1 to 100000)
 *    Smoothing for making the Slow-K line. Usually set to 3
 * 
 * optInSlowK_MAType:
 *    Type of Moving Average for Slow-K
 * 
 * optInSlowD_Period:(From 1 to 100000)
 *    Smoothing for making the Slow-D line
 * 
 * optInSlowD_MAType:
 *    Type of Moving Average for Slow-D
 * 
 * 
 */
TA_RetCode TA_STOCH( int    startIdx,
                     int    endIdx,
                     const double inHigh[],
                     const double inLow[],
                     const double inClose[],
                     int           optInFastK_Period, /* From 1 to 100000 */
                     int           optInSlowK_Period, /* From 1 to 100000 */
                     TA_MAType     optInSlowK_MAType,
                     int           optInSlowD_Period, /* From 1 to 100000 */
                     TA_MAType     optInSlowD_MAType,
                     int          *outBegIdx,
                     int          *outNBElement,
                     double        outSlowK[],
                     double        outSlowD[] );

int TA_STOCH_Lookback( int           optInFastK_Period, /* From 1 to 100000 */
                     int           optInSlowK_Period, /* From 1 to 100000 */
                     TA_MAType     optInSlowK_MAType,
                     int           optInSlowD_Period, /* From 1 to 100000 */
                     TA_MAType     optInSlowD_MAType ); 

/*
 * TA_STOCHF - Stochastic Fast
 * 
 * Input  = High, Low, Close
 * Output = double, double
 * 
 * Optional Parameters
 * -------------------
 * optInFastK_Period:(From 1 to 100000)
 *    Time period for building the Fast-K line
 * 
 * optInFastD_Period:(From 1 to 100000)
 *    Smoothing for making the Fast-D line. Usually set to 3
 * 
 * optInFastD_MAType:
 *    Type of Moving Average for Fast-D
 * 
 * 
 */
TA_RetCode TA_STOCHF( int    startIdx,
                      int    endIdx,
                      const double inHigh[],
                      const double inLow[],
                      const double inClose[],
                      int           optInFastK_Period, /* From 1 to 100000 */
                      int           optInFastD_Period, /* From 1 to 100000 */
                      TA_MAType     optInFastD_MAType,
                      int          *outBegIdx,
                      int          *outNBElement,
                      double        outFastK[],
                      double        outFastD[] );

int TA_STOCHF_Lookback( int           optInFastK_Period, /* From 1 to 100000 */
                      int           optInFastD_Period, /* From 1 to 100000 */
                      TA_MAType     optInFastD_MAType ); 

/*
 * TA_STOCHRSI - Stochastic Relative Strength Index
 * 
 * Input  = double
 * Output = double, double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * optInFastK_Period:(From 1 to 100000)
 *    Time period for building the Fast-K line
 * 
 * optInFastD_Period:(From 1 to 100000)
 *    Smoothing for making the Fast-D line. Usually set to 3
 * 
 * optInFastD_MAType:
 *    Type of Moving Average for Fast-D
 * 
 * 
 */
TA_RetCode TA_STOCHRSI( int    startIdx,
                        int    endIdx,
                        const double inReal[],
                        int           optInTimePeriod, /* From 2 to 100000 */
                        int           optInFastK_Period, /* From 1 to 100000 */
                        int           optInFastD_Period, /* From 1 to 100000 */
                        TA_MAType     optInFastD_MAType,
                        int          *outBegIdx,
                        int          *outNBElement,
                        double        outFastK[],
                        double        outFastD[] );

int TA_STOCHRSI_Lookback( int           optInTimePeriod, /* From 2 to 100000 */
                        int           optInFastK_Period, /* From 1 to 100000 */
                        int           optInFastD_Period, /* From 1 to 100000 */
                        TA_MAType     optInFastD_MAType ); 

/*
 * TA_SUB - Vector Arithmetic Substraction
 * 
 * Input  = double, double
 * Output = double
 * 
 */
TA_RetCode TA_SUB( int    startIdx,
                   int    endIdx,
                   const double inReal0[],
                   const double inReal1[],
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_SUB_Lookback( void );


/*
 * TA_SUM - Summation
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_SUM( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInTimePeriod, /* From 2 to 100000 */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_SUM_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_T3 - Triple Exponential Moving Average (T3)
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * optInVFactor:(From 0 to 1)
 *    Volume Factor
 * 
 * 
 */
TA_RetCode TA_T3( int    startIdx,
                  int    endIdx,
                  const double inReal[],
                  int           optInTimePeriod, /* From 2 to 100000 */
                  double        optInVFactor, /* From 0 to 1 */
                  int          *outBegIdx,
                  int          *outNBElement,
                  double        outReal[] );

int TA_T3_Lookback( int           optInTimePeriod, /* From 2 to 100000 */
                  double        optInVFactor );  /* From 0 to 1 */


/*
 * TA_TAN - Vector Trigonometric Tan
 * 
 * Input  = double
 * Output = double
 * 
 */
TA_RetCode TA_TAN( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_TAN_Lookback( void );


/*
 * TA_TANH - Vector Trigonometric Tanh
 * 
 * Input  = double
 * Output = double
 * 
 */
TA_RetCode TA_TANH( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );

int TA_TANH_Lookback( void );


/*
 * TA_TEMA - Triple Exponential Moving Average
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_TEMA( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int           optInTimePeriod, /* From 2 to 100000 */
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );

int TA_TEMA_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


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
                      double        outReal[] );

int TA_TRANGE_Lookback( void );


/*
 * TA_TRIMA - Triangular Moving Average
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_TRIMA( int    startIdx,
                     int    endIdx,
                     const double inReal[],
                     int           optInTimePeriod, /* From 2 to 100000 */
                     int          *outBegIdx,
                     int          *outNBElement,
                     double        outReal[] );

int TA_TRIMA_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_TRIX - 1-day Rate-Of-Change (ROC) of a Triple Smooth EMA
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_TRIX( int    startIdx,
                    int    endIdx,
                    const double inReal[],
                    int           optInTimePeriod, /* From 1 to 100000 */
                    int          *outBegIdx,
                    int          *outNBElement,
                    double        outReal[] );

int TA_TRIX_Lookback( int           optInTimePeriod );  /* From 1 to 100000 */


/*
 * TA_TSF - Time Series Forecast
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_TSF( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInTimePeriod, /* From 2 to 100000 */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_TSF_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_TYPPRICE - Typical Price
 * 
 * Input  = High, Low, Close
 * Output = double
 * 
 */
TA_RetCode TA_TYPPRICE( int    startIdx,
                        int    endIdx,
                        const double inHigh[],
                        const double inLow[],
                        const double inClose[],
                        int          *outBegIdx,
                        int          *outNBElement,
                        double        outReal[] );

int TA_TYPPRICE_Lookback( void );


/*
 * TA_ULTOSC - Ultimate Oscillator
 * 
 * Input  = High, Low, Close
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod1:(From 1 to 100000)
 *    Number of bars for 1st period.
 * 
 * optInTimePeriod2:(From 1 to 100000)
 *    Number of bars fro 2nd period
 * 
 * optInTimePeriod3:(From 1 to 100000)
 *    Number of bars for 3rd period
 * 
 * 
 */
TA_RetCode TA_ULTOSC( int    startIdx,
                      int    endIdx,
                      const double inHigh[],
                      const double inLow[],
                      const double inClose[],
                      int           optInTimePeriod1, /* From 1 to 100000 */
                      int           optInTimePeriod2, /* From 1 to 100000 */
                      int           optInTimePeriod3, /* From 1 to 100000 */
                      int          *outBegIdx,
                      int          *outNBElement,
                      double        outReal[] );

int TA_ULTOSC_Lookback( int           optInTimePeriod1, /* From 1 to 100000 */
                      int           optInTimePeriod2, /* From 1 to 100000 */
                      int           optInTimePeriod3 );  /* From 1 to 100000 */


/*
 * TA_VAR - Variance
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 1 to 100000)
 *    Number of period
 * 
 * optInNbDev:(From TA_REAL_MIN to TA_REAL_MAX)
 *    Nb of deviations
 * 
 * 
 */
TA_RetCode TA_VAR( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInTimePeriod, /* From 1 to 100000 */
                   double        optInNbDev, /* From TA_REAL_MIN to TA_REAL_MAX */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_VAR_Lookback( int           optInTimePeriod, /* From 1 to 100000 */
                   double        optInNbDev );  /* From TA_REAL_MIN to TA_REAL_MAX */


/*
 * TA_WCLPRICE - Weighted Close Price
 * 
 * Input  = High, Low, Close
 * Output = double
 * 
 */
TA_RetCode TA_WCLPRICE( int    startIdx,
                        int    endIdx,
                        const double inHigh[],
                        const double inLow[],
                        const double inClose[],
                        int          *outBegIdx,
                        int          *outNBElement,
                        double        outReal[] );

int TA_WCLPRICE_Lookback( void );


/*
 * TA_WILLR - Williams' %R
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
TA_RetCode TA_WILLR( int    startIdx,
                     int    endIdx,
                     const double inHigh[],
                     const double inLow[],
                     const double inClose[],
                     int           optInTimePeriod, /* From 2 to 100000 */
                     int          *outBegIdx,
                     int          *outNBElement,
                     double        outReal[] );

int TA_WILLR_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */


/*
 * TA_WMA - Weighted Moving Average
 * 
 * Input  = double
 * Output = double
 * 
 * Optional Parameters
 * -------------------
 * optInTimePeriod:(From 2 to 100000)
 *    Number of period
 * 
 * 
 */
TA_RetCode TA_WMA( int    startIdx,
                   int    endIdx,
                   const double inReal[],
                   int           optInTimePeriod, /* From 2 to 100000 */
                   int          *outBegIdx,
                   int          *outNBElement,
                   double        outReal[] );

int TA_WMA_Lookback( int           optInTimePeriod );  /* From 2 to 100000 */

#ifdef __cplusplus
}
#endif

/***************/
/* End of File */
/***************/

