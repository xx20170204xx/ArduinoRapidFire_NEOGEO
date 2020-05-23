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

/******************************************************************************/
void SetupButton(void);
void ShowInfo(void);
int ASyncWait(void);
int VSyncWait(void);
void autoSetup(void);
void autoClear(void);
void oneStep(void);
void oneStepAuto(int num, int OUTpin, int INval, int BINDval, int BINDtiming);

void oneStep_DBZ_VRVS_Macro1(void);
void oneStep_DBZ_VRVS_Macro2(void);

void oneStep_AnB_Macro(void);
void oneStep_BnC_Macro(void);
void oneStep_CnD_Macro(void);
void oneStep_DnE_Macro(void);
void oneStep_EnF_Macro(void);
void oneStep_AnD_Macro(void);
void oneStep_BnE_Macro(void);
void oneStep_CnF_Macro(void);
void oneStep_AnC_Macro(void);
void oneStep_AnBnC_Macro(void);
void oneStep_DnEnF_Macro(void);
/******************************************************************************/
#include "config.h"

int g_autoPin =  INPIN_AUTO;
int g_clearPin = INPIN_CLEAR;
/* Warning : Sync Pin Is No Checked  */
int g_syncINPin = -1;

/******************************************************************************/

void setup() {
  delay(3000);
  SetupButton();
  ShowInfo();
} /* setup */

/******************************************************************************/
void loop() {
  int autoStats = digitalRead(g_autoPin);
  int clearStats = digitalRead(g_clearPin);

  if( autoStats == LOW )
  {
    autoSetup();
  }else if( clearStats == LOW ){
    autoClear();
  }else{
    /* nop */
  }
  oneStep();

  if( g_syncINPin != -1 ){
    int ret = VSyncWait();
    if( ret != 0 )
    {
      ASyncWait();
    }
  }else{
    ASyncWait();
  }
} /* loop */

/******************************************************************************/
void SetupButton(void)
{
  // 連射設定ボタンの設定
  pinMode(g_autoPin, INPUT);
  digitalWrite(g_autoPin, HIGH);

  // 連射解除ボタンの設定
  pinMode(g_clearPin, INPUT);
  digitalWrite(g_clearPin, HIGH);

  // クロック入力
  if( g_syncINPin == -1 )
  {
    pinMode(g_syncINPin, INPUT);
    digitalWrite(g_syncINPin, LOW);
  }

  /* check bind setting start */
  /*
   * バインドの割り当てがあった場合、
   * 割当先の出力をなくす
  */
  for( int ii = 0; ii < BTN; ii++ )
  {
    if( g_BtnInfo[ii].bindFlags != BIND_NONE )
    {
      int bind = g_BtnInfo[ii].bindFlags;
      for( int jj = 0; jj < BTN; jj++ )
      {
        if( (bind & BIND_BTN(jj)) == BIND_BTN(jj) )
        {
          g_BtnInfo[jj].outputPin = -1;
        }
      }
    }
  }
  /* check bind setting end */
  
  for( int ii = 0; ii < BTN; ii++ )
  {
    int INpin = g_BtnInfo[ii].inputPin;
    int OUTpin = g_BtnInfo[ii].outputPin;
    int bind = g_BtnInfo[ii].bindFlags;

    if( g_BtnInfo[ii].timing > RPD_SPD_MIN )
    {
      g_BtnInfo[ii].timing = RPD_SPD_MIN;
    }

    g_BtnInfo[ii].counter = 0;
    g_BtnInfo[ii].enable = 0;
    g_BtnInfo[ii].outStatus = HIGH;
    g_BtnInfo[ii].rapidTiming = g_BtnInfo[ii].timing * RPD_DIV;

    pinMode(INpin, INPUT);
    digitalWrite(INpin, HIGH);

    if( bind != BIND_NONE )
    {
      for( int jj = 0; jj < BTN; jj++ )
      {
        if( (bind & BIND_BTN(jj) ) == BIND_BTN(jj) )
        {
          int BINDpin = g_BtnInfo[jj].inputPin;
          pinMode(BINDpin, INPUT);
          digitalWrite(BINDpin, HIGH);
        }
      }
    }
 
    if( OUTpin == -1 )
    {
      continue;
    }

    pinMode(OUTpin, OUTPUT);
    digitalWrite(OUTpin, HIGH);
  }
} /* SetupButton */
/******************************************************************************/
void ShowInfo( void )
{
  Serial.print( "Auto pin=[" );
  Serial.print( g_autoPin );
  Serial.println( "]" );

  Serial.print( "Clear pin=[" );
  Serial.print( g_clearPin );
  Serial.println( "]" );

  // クロック入力
  if( g_syncINPin == -1 )
  {
    Serial.print( "Sync Input pin=[" );
    Serial.print( g_syncINPin );
    Serial.println( "]" );
  }
  
  for( int ii = 0; ii < BTN; ii++ )
  {
    int INpin = g_BtnInfo[ii].inputPin;
    int OUTpin = g_BtnInfo[ii].outputPin;
    int bind = g_BtnInfo[ii].bindFlags;
    int timing = g_BtnInfo[ii].timing;
    int RpdSpeed = 60 / (timing + 1);

    if( OUTpin == -1 )
    {
      continue;
    }

    Serial.print( "button" );
    Serial.print( ii + 1 );
    Serial.print( " input=[" );
    Serial.print( INpin );
    Serial.print( "] output=[" );
    Serial.print( OUTpin );
    Serial.print( "]_speed=[" );
    Serial.print( RpdSpeed );

    if( bind != BIND_NONE )
    {
      for( int jj = 0; jj < BTN; jj++ )
      {
        if( (bind & BIND_BTN(jj) ) == BIND_BTN(jj) )
        {
          int BINDtiming = g_BtnInfo[jj].timing;
          int RpdSpeed = 60 / (BINDtiming + 1);
          Serial.print( "] bind=[" );
          Serial.print( jj + 1 );
          Serial.print( "]_speed=[" );
          Serial.print( RpdSpeed );
        }
      }
    }
    Serial.println( "]" );

  }
  
} /* ShowInfo */
/******************************************************************************/
void autoSetup(void)
{
  for( int ii = 0; ii < BTN; ii++ )
  {
    int INpin = g_BtnInfo[ii].inputPin;
    int OUTpin = g_BtnInfo[ii].outputPin;
    if( OUTpin == -1 )
    {
      continue;
    }
    int stats = digitalRead(INpin);
    if( stats == LOW && g_BtnInfo[ii].enable == 0 )
    {
      Serial.print( "button" );
      Serial.print( ii + 1 );
      Serial.println( " is AutoFire." );
      
      g_BtnInfo[ii].enable = 1;
    }
    
  }

} /* autoSetup */

/******************************************************************************/
int ASyncWait(void)
{
  delayMicroseconds(g_delay_us);
  return 0;
} /* ASyncWait */
/******************************************************************************/
int VSyncWait(void)
{
  //int s = millis();
  int st = LOW;
  int stbk = LOW;
  int flag = 2;
  if( g_syncINPin == -1 )
  {
    return 1;
  }
  
  stbk = digitalRead(g_syncINPin);
  while( flag > 0 )
  {
    delayMicroseconds(g_delay_us / 10);
    st = digitalRead(g_syncINPin);
    if( st != stbk )
    {
      flag -= 1;
    }
    stbk = st;
  }
  
  return 0;
} /* VSyncWait */

/******************************************************************************/
void autoClear(void)
{
  for( int ii = 0; ii < BTN; ii++ )
  {
      g_BtnInfo[ii].enable = 0;
  }
} /* autoClear */

/******************************************************************************/
void oneStep(void)
{
  for( int ii = 0; ii < BTN; ii++ )
  {
    int INpin = g_BtnInfo[ii].inputPin;
    int OUTpin = g_BtnInfo[ii].outputPin;
    int bind = g_BtnInfo[ii].bindFlags;
    int BINDpin = -1;
    int BINDtiming = RPD_SPD_MIN * RPD_DIV;
    int BINDval = HIGH;

    if( bind != BIND_NONE )
    {
      for( int jj = 0; jj < BTN; jj++ )
      {
        if( (bind & BIND_BTN(jj) ) == BIND_BTN(jj) )
        {
          int val = digitalRead(g_BtnInfo[jj].inputPin);
          if( val == LOW && g_BtnInfo[jj].rapidTiming < BINDtiming )
          {
            BINDval = LOW;
            BINDpin = g_BtnInfo[jj].inputPin;
            BINDtiming = g_BtnInfo[jj].rapidTiming;
          }
        }
      }
    }
    int INval = digitalRead(INpin);

    /* マクロ関数の指定がある場合、実行して次のボタンへ */
    if( INval == LOW && g_BtnInfo[ii].macroFunc != NULL )
    {
      g_BtnInfo[ii].macroFunc();
      continue;
    }
    
    if( BINDval == LOW || g_BtnInfo[ii].enable != 0 ){
      /* 連射の場合の動作 */
      oneStepAuto(ii, OUTpin, INval, BINDval, BINDtiming);
    }else{
      /* 現在の状態を設定する */
      g_BtnInfo[ii].outStatus = INval;
    }
  }
  for( int ii = 0; ii < BTN; ii++ )
  {
    int OUTpin = g_BtnInfo[ii].outputPin;
    int OUTstatus = g_BtnInfo[ii].outStatus;
    digitalWrite(OUTpin, OUTstatus);
  }
  
} /* oneStep */


/******************************************************************************/
void oneStepAuto(int num, int OUTpin, int INval, int BINDval, int BINDtiming)
{
  if( INval == LOW || BINDval == LOW )
  {
    /* ボタンが押されている状態 */
    /* 連射を行う */
    int papidTiming = g_BtnInfo[num].rapidTiming;
    if( BINDval == LOW )
    {
      /* バインド側のボタンが押されている場合、
       * バインドの設定を使う
       */
      papidTiming = BINDtiming;
    }
    
    g_BtnInfo[num].counter++;
    if( g_BtnInfo[num].counter >= papidTiming ) {
      g_BtnInfo[num].counter = 0;

      int val = digitalRead(OUTpin);
      g_BtnInfo[num].outStatus = (val==HIGH ? LOW : HIGH);
    }
  }else{
    /* ボタンが押されていない状態 */
    /* ボタンを押したタイミングでLOWになるように設定 */
    g_BtnInfo[num].counter = g_BtnInfo[num].rapidTiming;
    g_BtnInfo[num].outStatus = HIGH;
  }
} /* oneStepAuto */

/******************************************************************************/
/*
 * oneStep_DBZ_VRVS_Macro1
 * 
 * DBZ_VRVS用マクロ
 * ボタン1/ボタン2の交互連(秒間30連)
 * 
 * ボタン1:LHLHLHLH....
 * ボタン2:HLHLHLHL....
*/
void oneStep_DBZ_VRVS_Macro1(void)
{
  static int l_flag = 0;
  static int l_counter = 0;
  int l_papidTiming = RPD_SPD_30 * RPD_DIV;

  l_counter++;
  if( l_counter >= l_papidTiming ) {
    l_counter = 0;

    l_flag = (l_flag==0 ? 1 : 0);
  }

  g_BtnInfo[0].outStatus = (l_flag==0 ? LOW : HIGH);
  g_BtnInfo[1].outStatus = (l_flag==1 ? LOW : HIGH);

} /* oneStep_DBZ_VRVS_Macro1 */
/******************************************************************************/
/*
 * oneStep_DBZ_VRVS_Macro2
 * 
 * DBZ_VRVS用マクロ
 * ボタン1/ボタン2の交互連
 * 
 * ボタン1:LHHHLHHH....
 * ボタン2:HHLHHHLH....
*/
void oneStep_DBZ_VRVS_Macro2(void)
{
  static int l_flag = 0;
  static int l_counter = 0;
  int l_papidTiming = RPD_SPD_30 * RPD_DIV;

  l_counter++;
  if( l_counter >= l_papidTiming ) {
    l_counter = 0;

    l_flag++;
    if(l_flag >= 4 ) l_flag = 0;
  }

  g_BtnInfo[0].outStatus = (l_flag==0 ? LOW : HIGH);
  g_BtnInfo[1].outStatus = (l_flag==2 ? LOW : HIGH);

} /* oneStep_DBZ_VRVS_Macro2 */
/******************************************************************************/
/*
 * oneStep_AnB_Macro
 * 
 * ボタン1/ボタン2の同時押しボタン
 * 
*/
void oneStep_AnB_Macro(void)
{
  g_BtnInfo[0].outStatus = LOW;
  g_BtnInfo[1].outStatus = LOW;
} /* oneStep_AnB_Macro */
/******************************************************************************/
/*
 * oneStep_BnC_Macro
 * 
 * ボタン2/ボタン3の同時押しボタン
 * 
*/
void oneStep_BnC_Macro(void)
{
  g_BtnInfo[1].outStatus = LOW;
  g_BtnInfo[2].outStatus = LOW;
} /* oneStep_BnC_Macro */
/******************************************************************************/
/*
 * oneStep_CnD_Macro
 * 
 * ボタン3/ボタン4の同時押しボタン
 * 
*/
void oneStep_CnD_Macro(void)
{
  g_BtnInfo[2].outStatus = LOW;
  g_BtnInfo[3].outStatus = LOW;
} /* oneStep_CnD_Macro */
/******************************************************************************/
/*
 * oneStep_DnE_Macro
 * 
 * ボタン4/ボタン5の同時押しボタン
 * 
*/
void oneStep_DnE_Macro(void)
{
  g_BtnInfo[3].outStatus = LOW;
  g_BtnInfo[4].outStatus = LOW;
} /* oneStep_DnE_Macro */
/******************************************************************************/
/*
 * oneStep_EnF_Macro
 * 
 * ボタン5/ボタン6の同時押しボタン
 * 
*/
void oneStep_EnF_Macro(void)
{
  g_BtnInfo[4].outStatus = LOW;
  g_BtnInfo[5].outStatus = LOW;
} /* oneStep_EnF_Macro */
/******************************************************************************/
/*
 * oneStep_AnD_Macro
 * 
 * ボタン1/ボタン4の同時押しボタン
 * 
*/
void oneStep_AnD_Macro(void)
{
  g_BtnInfo[0].outStatus = LOW;
  g_BtnInfo[3].outStatus = LOW;
} /* oneStep_AnD_Macro */
/******************************************************************************/
/*
 * oneStep_BnE_Macro
 * 
 * ボタン2/ボタン5の同時押しボタン
 * 
*/
void oneStep_BnE_Macro(void)
{
  g_BtnInfo[1].outStatus = LOW;
  g_BtnInfo[4].outStatus = LOW;
} /* oneStep_BnE_Macro */
/******************************************************************************/
/*
 * oneStep_CnF_Macro
 * 
 * ボタン3/ボタン6の同時押しボタン
 * 
*/
void oneStep_CnF_Macro(void)
{
  g_BtnInfo[2].outStatus = LOW;
  g_BtnInfo[5].outStatus = LOW;
} /* oneStep_CnF_Macro */
/******************************************************************************/
/*
 * oneStep_AnC_Macro
 * 
 * ボタン1/ボタン3の同時押しボタン
 * 
*/
void oneStep_AnC_Macro(void)
{
  g_BtnInfo[0].outStatus = LOW;
  g_BtnInfo[2].outStatus = LOW;
} /* oneStep_AnC_Macro */
/******************************************************************************/
/*
 * oneStep_AnBnC_Macro
 * 
 * ボタン1/ボタン2/ボタン3の同時押しボタン
 * 
*/
void oneStep_AnBnC_Macro(void)
{
  g_BtnInfo[0].outStatus = LOW;
  g_BtnInfo[1].outStatus = LOW;
  g_BtnInfo[2].outStatus = LOW;
} /* oneStep_AnBnC_Macro */
/******************************************************************************/
/*
 * oneStep_DnEnF_Macro
 * 
 * ボタン4/ボタン5/ボタン6の同時押しボタン
 * 
*/
void oneStep_DnEnF_Macro(void)
{
  g_BtnInfo[3].outStatus = LOW;
  g_BtnInfo[4].outStatus = LOW;
  g_BtnInfo[5].outStatus = LOW;
} /* oneStep_DnEnF_Macro */
/******************************************************************************/
