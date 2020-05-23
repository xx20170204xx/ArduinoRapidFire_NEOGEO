/* 
 * Arduino Rapid Fire for NEOGEO
 * 
 * Arduino Leonardo 互換ボード
 * 
 * @license    MIT License : http://www.opensource.org/licenses/mit-license.html
 * @version    20200523
 * 
 * @link       https://github.com/xx20170204xx/ArduinoRapidFire_NEOGEO
 */

/**************************************/

// ボタン数
#define BTN 6

/***************************************
 * Pro Micro(5V 16MHz)
 *     +----[USB]----+ 
 * TXO |             | RAW  
 * RXI |             | GND  
 * GND |             | Reset
 * GND |             | VCC  
 *   2 |             | A3   
 *   3 |             | A2   
 *   4 |             | A1   
 *   5 |             | A0   
 *   6 |             | 15   
 *   7 |             | 14   
 *   8 |             | 16   
 *   9 |             | 10   
 *     +-------------+

/* 各ボタンに対応するPIN */
/*   Input Button pin */
#define INPIN_BTN1  10
#define INPIN_BTN2  16
#define INPIN_BTN3  14
#define INPIN_BTN4  15
#define INPIN_BTN5  A0
#define INPIN_BTN6  A1
/*   Output Button pin */
#define OUTPIN_BTN1  2
#define OUTPIN_BTN2  3
#define OUTPIN_BTN3  4
#define OUTPIN_BTN4  5
#define OUTPIN_BTN5  6
#define OUTPIN_BTN6  7
/*   Control Pin */
#define INPIN_AUTO  8
#define INPIN_CLEAR 9

#define DEF_SET_CPS2_DDSOM      0
#define DEF_SET_CPS2_AVSP       0
#define DEF_SET_CPS2_PGEAR      0
#define DEF_SET_S32_DBZVRVS     0
#define DEF_SET_S32_GA2         0
#define DEF_SET_DEVTEST01       0
#define DEF_SET_DEVTEST02       0
/******************************************************************************/
/* 各ボタンの設定 */
#if DEF_SET_CPS2_DDSOM == 1
  /*
   * Dungeon & Dragons Shadow over Mystara
   * ** ボタン配置
   * [1] [2] [3]
   * [4] [5] [6]
   * 
   * ** 各ボタンの設定
   * [1] …… ボタン4(Auto有効時 30連)
   * [2] …… ボタン5(Auto有効時 30連)
   * [3] …… ボタン6(Auto有効時 30連)
   * [4] …… ボタン1(Auto有効時 30連)
   * [5] …… ボタン2(Auto有効時 30連)
   * [6] …… ボタン3(Auto有効時 30連)
   */
  
  SBTNINFO g_BtnInfo[BTN] = {
    /*InputPin    OutputPin    Timing      Bind         MacroFunc  -  - */
    { INPIN_BTN4, OUTPIN_BTN1, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN5, OUTPIN_BTN2, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN6, OUTPIN_BTN3, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN1, OUTPIN_BTN4, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN2, OUTPIN_BTN5, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN3, OUTPIN_BTN6, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
  };

#elif DEF_SET_CPS2_AVSP == 1
  /*
   * Alien VS Predator
   * ** ボタン配置
   * [1] [2] [3]
   * [4] [5] [6]
   * 
   * ** 各ボタンの設定
   * [1] …… ボタン1(Auto有効時 30連)
   * [2] …… ボタン2(Auto有効時 30連)
   * [3] …… ボタン3(Auto有効時 30連)
   * [4] …… ボタン1(15連)
   * [5] …… ボタン2(30連)
   * [6] …… ボタン3(30連)
   */
  
  SBTNINFO g_BtnInfo[BTN] = {
    /*InputPin    OutputPin    Timing      Bind         MacroFunc  -  - */
    { INPIN_BTN1, OUTPIN_BTN1, RPD_SPD_30, BIND_BTN4,   NULL, 0, 0, },
    { INPIN_BTN2, OUTPIN_BTN2, RPD_SPD_30, BIND_BTN5,   NULL, 0, 0, },
    { INPIN_BTN3, OUTPIN_BTN3, RPD_SPD_30, BIND_BTN6,   NULL, 0, 0, },
    { INPIN_BTN4, OUTPIN_BTN4, RPD_SPD_15, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN5, OUTPIN_BTN5, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN6, OUTPIN_BTN6, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
  };

#elif DEF_SET_CPS2_PGEAR == 1
  /*
   * パワードギア
   * ** ボタン配置
   * [1] [2] [3]
   * [4] [5] [6]
   * 
   * ** 各ボタンの設定
   * [1] …… ボタン1(Auto有効時 30連)
   * [2] …… ボタン2(Auto有効時 30連)
   * [3] …… ボタン3(Auto有効時 30連)
   * [4] …… マクロ(ボタン1/ボタン2の同時押し)
   * [5] …… マクロ(ボタン2/ボタン3の同時押し)
   * [6] …… ボタン1
   */
  
  SBTNINFO g_BtnInfo[BTN] = {
    /*InputPin    OutputPin    Timing      Bind         MacroFunc  -  - */
    { INPIN_BTN1, OUTPIN_BTN1, RPD_SPD_30, BIND_BTN6,   NULL, 0, 0, },
    { INPIN_BTN2, OUTPIN_BTN2, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN3, OUTPIN_BTN3, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN4, OUTPIN_BTN4, RPD_SPD_15, BIND_NONE,   oneStep_AnB_Macro, 0, 0, },
    { INPIN_BTN5, OUTPIN_BTN5, RPD_SPD_30, BIND_NONE,   oneStep_BnC_Macro, 0, 0, },
    { INPIN_BTN6, OUTPIN_BTN6, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
  };

#elif DEF_SET_S32_DBZVRVS == 1
  /*
   * Dragon Ball Z V.R.V.S
   * ** ボタン配置
   * [1] [2] [3]
   * [4] [5] [6]
   * 
   * ** 各ボタンの設定
   * [1] …… ボタン1(Auto有効時 30連)
   * [2] …… ボタン2(Auto有効時 30連)
   * [3] …… ボタン3(Auto有効時 30連)
   * [4] …… マクロ(ボタン1/ボタン2の交互30連)
   * [5] …… マクロ(ボタン1/ボタン2の交互連)
   * [6] …… ボタン6
   */
  
  SBTNINFO g_BtnInfo[BTN] = {
    /*InputPin    OutputPin    Timing      Bind         MacroFunc  -  - */
    { INPIN_BTN1, OUTPIN_BTN1, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN2, OUTPIN_BTN2, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN3, OUTPIN_BTN3, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN4, OUTPIN_BTN4, RPD_SPD_15, BIND_NONE,   oneStep_DBZ_VRVS_Macro1, 0, 0, },
    { INPIN_BTN5, OUTPIN_BTN5, RPD_SPD_30, BIND_NONE,   oneStep_DBZ_VRVS_Macro2, 0, 0, },
    { INPIN_BTN6, OUTPIN_BTN6, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
  };

#elif DEF_SET_S32_GA2 == 1
  /*
   * ゴールデンアックス デスアダーの復讐
   * ** ボタン配置
   * [1] [2] [3]
   * [4] [5] [6]
   * 
   * ** 各ボタンの設定
   * [1] …… ボタン1(Auto有効時 30連)
   * [2] …… ボタン2(Auto有効時 30連)
   * [3] …… ボタン3(Auto有効時 30連)
   * [4] …… マクロ(ボタン1/ボタン2の同時押し)
   * [5] …… ボタン1
   * [6] …… ボタン1
   */
  
  SBTNINFO g_BtnInfo[BTN] = {
    /*InputPin    OutputPin    Timing      Bind         MacroFunc  -  - */
    { INPIN_BTN1, OUTPIN_BTN1, RPD_SPD_30, BIND_BTN5 | BIND_BTN6,   NULL, 0, 0, },
    { INPIN_BTN2, OUTPIN_BTN2, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN3, OUTPIN_BTN3, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN4, OUTPIN_BTN4, RPD_SPD_15, BIND_NONE,   oneStep_AnB_Macro, 0, 0, },
    { INPIN_BTN5, OUTPIN_BTN5, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN6, OUTPIN_BTN6, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
  };

#elif DEF_SET_DEVTEST01 == 1
  /*
   * Development Test 01
   * ** ボタン配置
   * [1] [2] [3]
   * [4] [5] [6]
   * 
   * ** 各ボタンの設定
   * [1] …… ボタン1(Auto有効時 30連)
   * [2] …… ボタン1(20連)
   * [3] …… ボタン1(15連)
   * [4] …… ボタン1(12連)
   * [5] …… ボタン1(10連)
   * [6] …… ボタン1(7.5連)
   */
  
  SBTNINFO g_BtnInfo[BTN] = {
    /*InputPin    OutputPin    Timing      Bind         MacroFunc  -  - */
    { INPIN_BTN1, OUTPIN_BTN1, RPD_SPD_30, BIND_BTN2 | 
                                           BIND_BTN3 | 
                                           BIND_BTN4 | 
                                           BIND_BTN5 | 
                                           BIND_BTN6 ,  NULL, 0, 0, },
    { INPIN_BTN2, OUTPIN_BTN2, RPD_SPD_20, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN3, OUTPIN_BTN3, RPD_SPD_15, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN4, OUTPIN_BTN4, RPD_SPD_12, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN5, OUTPIN_BTN5, RPD_SPD_10, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN6, OUTPIN_BTN6, RPD_SPD_7_5, BIND_NONE,   NULL, 0, 0, },
  };

#elif DEF_SET_DEVTEST02 == 1
  /*
   * Development Test 02
   * ** ボタン配置
   * [1] [2] [3]
   * [4] [5] [6]
   * 
   * ** 各ボタンの設定
   * [1] …… ボタン1(Auto有効時 30連)
   * [2] …… ボタン2(Auto有効時 30連)
   * [3] …… ボタン2(20連)
   * [4] …… ボタン2(15連)
   * [5] …… ボタン2(12連)
   * [6] …… ボタン2(10連)
   */
  
  SBTNINFO g_BtnInfo[BTN] = {
    /*InputPin    OutputPin    Timing      Bind         MacroFunc  -  - */
    { INPIN_BTN1, OUTPIN_BTN1, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN2, OUTPIN_BTN2, RPD_SPD_30, BIND_BTN3 | 
                                           BIND_BTN4 | 
                                           BIND_BTN5 | 
                                           BIND_BTN6 ,  NULL, 0, 0, },
    { INPIN_BTN3, OUTPIN_BTN3, RPD_SPD_20, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN4, OUTPIN_BTN4, RPD_SPD_15, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN5, OUTPIN_BTN5, RPD_SPD_12, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN6, OUTPIN_BTN6, RPD_SPD_10, BIND_NONE,   NULL, 0, 0, },
  };

#else
  /*
   * 通常設定
   * [1] [2] [3]
   * [4] [5] [6]
   * 
   * ** 各ボタンの設定
   * [1] …… ボタン1(Auto有効時 30連)
   * [2] …… ボタン2(Auto有効時 30連)
   * [3] …… ボタン3(Auto有効時 30連)
   * [4] …… ボタン4(Auto有効時 30連)
   * [5] …… ボタン5(Auto有効時 30連)
   * [6] …… ボタン6(Auto有効時 30連)
   */
  SBTNINFO g_BtnInfo[BTN] = {
    /*InputPin    OutputPin    Timing      Bind         MacroFunc  -  - */
    { INPIN_BTN1, OUTPIN_BTN1, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN2, OUTPIN_BTN2, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN3, OUTPIN_BTN3, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN4, OUTPIN_BTN4, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN5, OUTPIN_BTN5, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
    { INPIN_BTN6, OUTPIN_BTN6, RPD_SPD_30, BIND_NONE,   NULL, 0, 0, },
  };

#endif

/* EOF */
