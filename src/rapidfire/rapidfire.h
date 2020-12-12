/* 
 * Arduino Rapid Fire for NEOGEO
 * 
 * Arduino Leonardo 互換ボード
 * 
 * @license    MIT License : http://www.opensource.org/licenses/mit-license.html
 * @version    20201213
 * 
 * @link       https://github.com/xx20170204xx/ArduinoRapidFire_NEOGEO
 */

#ifndef __RAPIDFIRE_H__
#define __RAPIDFIRE_H__

/* 連射用の定数 */
#define RPD_SPD_30  (1)   /*  30連 */
#define RPD_SPD_20  (2)   /*  20連 */
#define RPD_SPD_15  (3)   /*  15連 */
#define RPD_SPD_12  (4)   /*  12連 */
#define RPD_SPD_10  (5)   /*  10連 */
#define RPD_SPD_7_5 (7)   /* 7.5連 */
#define RPD_SPD_MIN (59)  /*   1連 */

/* Bind用定数 */
#define BIND_BTN(n) (1 << n)
#define BIND_BTN1   (BIND_BTN(0))
#define BIND_BTN2   (BIND_BTN(1))
#define BIND_BTN3   (BIND_BTN(2))
#define BIND_BTN4   (BIND_BTN(3))
#define BIND_BTN5   (BIND_BTN(4))
#define BIND_BTN6   (BIND_BTN(5))
#define BIND_NONE   (0)

#define RPD_DIV (10)

/* 非同期連射時のディレイ時間(Micro Second) */
#define g_delay_us (1000000 / (60 * RPD_DIV))

typedef void (*MACRO_FUNC)(void);

typedef struct SBTNINFO
{
  /* 入力Pin */
  int inputPin;

  /*
   * 出力Pin
   * 
   * 出力するPinの番号を設定する
   * -1の場合、出力しない
  */
  int outputPin;

  /*
   * 連射速度
   * 
   * 連射する速度を設定することできる。
   * 1 : 30  = 60 / (1 + 1)
   * 2 : 20  = 60 / (2 + 1)
   * 3 : 15  = 60 / (3 + 1)
   * 4 : 12  = 60 / (4 + 1)
   * 5 : 10  = 60 / (5 + 1)
   * 6 : 8.6 = 60 / (6 + 1)
   * 7 : 7.5 = 60 / (7 + 1)
  */
  int timing;

  /* 
   *  割り当て指定
   *  
   *  連射専用のボタンを割り当てる場合にフラグを設定する。
   *  割り当てない場合、BIND_NONE を設定する
  */
  int bindFlags;

  /*
   * マクロ用関数
   * 
   * 使用しない場合、NULLを指定
  */
  MACRO_FUNC macroFunc;

  /****************************************************************************/

  /*
   * 連射有効フラグ
  */
  int enable;

  /*
  * 連射用カウンター
  */
  int counter;

  /* 
   *  連射用タイミング
  */
  int rapidTiming;

  /*
   * ボタンの出力内容
   * 
   * HIGH:未押下
   * LOW :押下
  */
  int outStatus;
} SBTNINFO;

void setupButton(void);
void showInfo(void);
int ASyncWait(void);
int VSyncWait(void);
void autoSetup(void);
void autoClear(void);
void oneStep(void);
void oneStepAuto(int num, int OUTpin, int INval, int BINDval, int BINDtiming);

#endif

/* EOF */
