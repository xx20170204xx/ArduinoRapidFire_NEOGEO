/* 
 * Arduino Rapid Fire for NEOGEO
 * 
 * Arduino Leonardo 互換ボード
 * 
 * @license    http://www.opensource.org/licenses/mit-license.html  MIT License
 * @version    20200519
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

#define DEF_SET_CPS2_DDSOM  0
#define DEF_SET_CPS2_AVSP   0
#define DEF_SET_CPS2_PGEAR  0
#define DEF_SET_S32_DBZVRVS 0
#define DEF_SET_S32_GA2     0
#define DEF_SET_DEVTEST01   0
#define DEF_SET_DEVTEST02   0
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
void oneStep_AnBnC_Macro(void);
/******************************************************************************/
int g_autoPin =  INPIN_AUTO;
int g_clearPin = INPIN_CLEAR;
/* Warning : Sync Pin Is No Checked  */
int g_syncINPin = -1;

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
    /*InputPin    OutputPin    Timing      Bind　　　　　MacroFunc  -  - */
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
    /*InputPin    OutputPin    Timing      Bind  -  - */
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
    /*InputPin    OutputPin    Timing      Bind　　　　　MacroFunc  -  - */
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
    /*InputPin    OutputPin    Timing      Bind　　　　　MacroFunc  -  - */
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
    /*InputPin    OutputPin    Timing      Bind　　　　　MacroFunc  -  - */
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
    /*InputPin    OutputPin    Timing      Bind　　　　　MacroFunc  -  - */
    { INPIN_BTN1, OUTPIN_BTN1, RPD_SPD_30, BIND_BTN2 | 
                                           BIND_BTN3 | 
                                           BIND_BTN4 | 
                                           BIND_BTN5 | 
                                           BIND_BTN6 ,   NULL, 0, 0, },
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
                                           BIND_BTN6 ,   NULL, 0, 0, },
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
