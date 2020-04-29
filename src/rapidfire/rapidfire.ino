/* 
 * Arduino Rapid Fire for NEOGEO
 * 
 * Arduino Leonardo 互換ボード
 * 
 * @license    http://www.opensource.org/licenses/mit-license.html  MIT License
 * @version    20200429
 * 
 * @link       https://github.com/xx20170204xx/ArduinoRapidFire_NEOGEO
 */

/**************************************/
// ボタン数
#define BTN 6

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

#define DEF_SET_CPS2_DDSOM  0
#define DEF_SET_CPS2_AVSP   0
/**************************************/

/* 連射用の定数 */
#define RPD_SPD_30  1   /*  30連 */
#define RPD_SPD_20  2   /*  20連 */
#define RPD_SPD_15  3   /*  15連 */
#define RPD_SPD_12  4   /*  12連 */
#define RPD_SPD_10  5   /*  10連 */
#define RPD_SPD_7_5 7   /* 7.5連 */

/* 非同期連射時のディレイ時間(Micro Second) */
#define g_delay_us (1000000 / 60)

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
   *  連射専用のボタンを割り当てる場合にボタンの番号(0～)を設定する。
   *  割り当てない場合、-1を設定する
  */
  int bind;

  /****************************************************************************/

  /*
   * 連射有効フラグ
  */
  int enable;

  /*
  * 連射用カウンター
  */
  int counter;
} SBTNINFO;

/******************************************************************************/
int ASyncWait(void);
int VSyncWait(void);
void autoSetup(void);
void autoClear(void);
void oneStep(void);
void oneStepAuto(int num, int OUTpin, int INval, int BINDval);


/******************************************************************************/
int g_autoPin =  INPIN_AUTO;
int g_clearPin = INPIN_CLEAR;
int g_syncINPin = -1;

/******************************************************************************/
/* 各ボタンの設定 */
#if DEF_SET_CPS2_DDSOM == 1
  /*
   * Dungeon & Dragons Shadow over Mystara
   * [1] [2] [3]
   * [4] [5] [6]
   * 
   * [1] …… ボタン4(30連)
   * [2] …… ボタン5(30連)
   * [3] …… ボタン6(30連)
   * [4] …… ボタン1(30連)
   * [5] …… ボタン2(30連)
   * [6] …… ボタン3(30連)
  */
  
  SBTNINFO g_BtnInfo[BTN] = {
    /*In           Ot            Ti          Bi  -  - */
    { INPIN_BTN4, OUTPIN_BTN1, RPD_SPD_30, -1, 0, 0, },
    { INPIN_BTN5, OUTPIN_BTN2, RPD_SPD_30, -1, 0, 0, },
    { INPIN_BTN6, OUTPIN_BTN3, RPD_SPD_30, -1, 0, 0, },
    { INPIN_BTN1, OUTPIN_BTN4, RPD_SPD_30, -1, 0, 0, },
    { INPIN_BTN2, OUTPIN_BTN5, RPD_SPD_30, -1, 0, 0, },
    { INPIN_BTN3, OUTPIN_BTN6, RPD_SPD_30, -1, 0, 0, },
  };

#elif DEF_SET_CPS2_AVSP == 1
  /*
   * Alien VS Predator
   * [1] [2] [3]
   * [4] [5] [6]
   * 
   * [1] …… ボタン1(30連)
   * [2] …… ボタン2(30連)
   * [3] …… ボタン3(30連)
   * [4] …… ボタン1(15連)
   * [5] …… ボタン2(30連)
   * [6] …… ボタン3(30連)
  */
  
  SBTNINFO g_BtnInfo[BTN] = {
    /*In           Ot            Ti          Bi  -  - */
    { INPIN_BTN1, OUTPIN_BTN1, RPD_SPD_30,  3, 0, 0, },
    { INPIN_BTN2, OUTPIN_BTN2, RPD_SPD_30,  4, 0, 0, },
    { INPIN_BTN3, OUTPIN_BTN3, RPD_SPD_30,  5, 0, 0, },
    { INPIN_BTN4, OUTPIN_BTN4, RPD_SPD_15, -1, 0, 0, },
    { INPIN_BTN5, OUTPIN_BTN5, RPD_SPD_30, -1, 0, 0, },
    { INPIN_BTN6, OUTPIN_BTN6, RPD_SPD_30, -1, 0, 0, },
  };

#else
  /*
   * 通常設定
   * [1] [2] [3]
   * [4] [5] [6]
   * 
   * [1] …… ボタン1(30連)
   * [2] …… ボタン2(30連)
   * [3] …… ボタン3(30連)
   * [4] …… ボタン4(30連)
   * [5] …… ボタン5(30連)
   * [6] …… ボタン6(30連)
  */
  SBTNINFO g_BtnInfo[BTN] = {
    /*In           Ot            Ti          Bi  -  - */
    { INPIN_BTN1, OUTPIN_BTN1, RPD_SPD_30, -1, 0, 0, },
    { INPIN_BTN2, OUTPIN_BTN2, RPD_SPD_30, -1, 0, 0, },
    { INPIN_BTN3, OUTPIN_BTN3, RPD_SPD_30, -1, 0, 0, },
    { INPIN_BTN4, OUTPIN_BTN4, RPD_SPD_30, -1, 0, 0, },
    { INPIN_BTN5, OUTPIN_BTN5, RPD_SPD_30, -1, 0, 0, },
    { INPIN_BTN6, OUTPIN_BTN6, RPD_SPD_30, -1, 0, 0, },
  };

#endif

/******************************************************************************/

void setup() {
  delay(3000);

  // 連射設定ボタンの設定
  pinMode(g_autoPin, INPUT);
  digitalWrite(g_autoPin, HIGH);
  Serial.print( "Auto pin=[" );
  Serial.print( g_autoPin );
  Serial.println( "]" );

  // 連射解除ボタンの設定
  pinMode(g_clearPin, INPUT);
  digitalWrite(g_clearPin, HIGH);
  Serial.print( "Clear pin=[" );
  Serial.print( g_clearPin );
  Serial.println( "]" );

  // クロック入力
  if( g_syncINPin == -1 )
  {
    pinMode(g_syncINPin, INPUT);
    digitalWrite(g_syncINPin, LOW);
    Serial.print( "Sync Input pin=[" );
    Serial.print( g_syncINPin );
    Serial.println( "]" );
  }

  /* check bind setting start */
  /*
   * バインドの割り当てがあった場合、
   * 割当先の出力をなくす
  */
  for( int ii = 0; ii < BTN; ii++ )
  {
    if( g_BtnInfo[ii].bind != -1 )
    {
      int bind = g_BtnInfo[ii].bind;
      g_BtnInfo[bind].outputPin = -1;
    }
  }
  /* check bind setting end */
  
  for( int ii = 0; ii < BTN; ii++ )
  {
    int INpin = g_BtnInfo[ii].inputPin;
    int OUTpin = g_BtnInfo[ii].outputPin;
    int bind = g_BtnInfo[ii].bind;
    int BINDpin = -1;

    g_BtnInfo[ii].counter = 0;
    g_BtnInfo[ii].enable = 0;

    pinMode(INpin, INPUT);
    digitalWrite(INpin, HIGH);

    if( bind != -1 )
    {
      BINDpin = g_BtnInfo[bind].inputPin;
      pinMode(BINDpin, INPUT);
      digitalWrite(BINDpin, HIGH);
    }
 
    if( OUTpin == -1 )
    {
      continue;
    }

    pinMode(OUTpin, OUTPUT);
    digitalWrite(OUTpin, HIGH);

    // Serial.print( "button%d input=[%d] output=[%d]\n", ii + 1, INpin, OUTpin );
    Serial.print( "button" );
    Serial.print( ii + 1 );
    Serial.print( " input=[" );
    Serial.print( INpin );
    if( BINDpin != -1 )
    {
      Serial.print( "] bind=[" );
      Serial.print( BINDpin );
    }
    Serial.print( "] output=[" );
    Serial.print( OUTpin );
    Serial.println( "]" );

  }
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
    int bind = g_BtnInfo[ii].bind;
    int BINDpin = -1;
    int BINDval = HIGH;
    if( bind != -1 )
    {
      BINDpin = g_BtnInfo[bind].inputPin;
      BINDval = digitalRead(BINDpin);
    }
    int INval = digitalRead(INpin);

    if( BINDval == LOW || g_BtnInfo[ii].enable != 0 ){
      /* 連射の場合の動作 */
      oneStepAuto(ii, OUTpin, INval, BINDval);
    }else{
      /* 現在の状態を設定する */
      digitalWrite(OUTpin, INval);
    }
  }
  
} /* oneStep */


/******************************************************************************/
void oneStepAuto(int num, int OUTpin, int INval, int BINDval)
{
  if( INval == LOW || BINDval == LOW )
  {
    /* ボタンが押されている状態 */
    /* 連射を行う */
    int timing = g_BtnInfo[num].timing;
    if( BINDval == LOW )
    {
      /* バインド側のボタンが押されている場合、
       * バインドの設定を使う
       */
      int bind = g_BtnInfo[num].bind;
      timing = g_BtnInfo[bind].timing;
    }
    
    g_BtnInfo[num].counter++;
    if( g_BtnInfo[num].counter >= timing ) {
      g_BtnInfo[num].counter = 0;

      int val = digitalRead(OUTpin);
      digitalWrite(OUTpin, (val==HIGH ? LOW : HIGH));
    }
  }else{
    /* ボタンが押されていない状態 */
    g_BtnInfo[num].counter = g_BtnInfo[num].timing; /* ボタンを押したタイミングでLOWになるように設定 */
    digitalWrite(OUTpin, HIGH);
  }
} /* oneStepAuto */

/******************************************************************************/
