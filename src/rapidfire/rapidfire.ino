/* 
 * Arduino Rapid Fire for NEOGEO
 * 
 * Arduino Leonardo
 * 
 * @license    http://www.opensource.org/licenses/mit-license.html  MIT License
 * @version    20200422
 * 
 * Git : https://github.com/xx20170204xx/ArduinoRapidFire_NEOGEO
 */

// ボタン数
#define BTN 6

#define g_delay1 (1000 / 60)

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
int VSyncWait(void);
void autoSetup(void);
void autoClear(void);
void oneStep(void);
void oneStepAuto(int num, int OUTpin, int INval, int BINDval);


/******************************************************************************/
int g_autoPin = A0;
int g_clearPin = A1;
int g_syncINPin = -1;

SBTNINFO g_BtnInfo[BTN] = {
  /*In Ot Ti  Bi  -  - */
  { A2, 2, 1, -1, 0, 0, },
  { A3, 3, 1, -1, 0, 0, },
  { A4, 4, 1, -1, 0, 0, },
  { A5, 5, 1, -1, 0, 0, },
  {  8, 6, 1, -1, 0, 0, },
  {  9, 7, 1, -1, 0, 0, },
};

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
    oneStep();
  }

  if( g_syncINPin != -1 ){
    int ret = VSyncWait();
    if( ret != 0 )
    {
      delay(g_delay1);
    }
  }else{
    delay(g_delay1);
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
    delay(1);
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
