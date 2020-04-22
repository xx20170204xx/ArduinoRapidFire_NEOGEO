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

/******************************************************************************/
void autoSetup(void);
void autoClear(void);
void oneStep(void);
void oneStepAuto(int num, int OUTpin, int INval, int BINDval);


/******************************************************************************/
int g_autoPin = A0;
int g_clearPin = A1;

/* 入力Pin */
int g_inputs[BTN] = { A2, A3, A4, A5,  8,  9 }; // 入力Pin

/*
 * 出力Pin
 * 
 * 出力するPinの番号を設定する
 * -1の場合、出力しない
*/
int g_outputs[BTN] = { 2, 3, 4, 5, 6, 7 }; // 出力Pin

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
int g_timings[BTN] = { 1, 1, 1, 1, 1, 1 };

/* 
 *  割り当て指定
 *  
 *  連射専用のボタンを割り当てる場合にボタンの番号(0～)を設定する。
 *  割り当てない場合、-1を設定する
 *  
 *  以下の場合、4、5、6ボタンにそれぞれ1，2，3ボタンが連射で使えるようになる。
 *  ※連射速度は、押したボタンの設定を使う。
 *  int g_bind[BTN] = { 3, 4, 5, -1, -1, -1 };
*/
int g_bind[BTN] = { -1, -1, -1, -1, -1, -1 };

/*
 * 連射用カウンター
*/
int g_counter[BTN];

/*
 * 連射フラグ
*/
int g_flags[BTN];
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

  /* check bind setting start */
  /*
   * バインドの割り当てがあった場合、
   * 割当先の出力をなくす
  */
  for( int ii = 0; ii < BTN; ii++ )
  {
    if( g_bind[ii] != -1 )
    {
      g_outputs[g_bind[ii]] = -1;
    }
  }
  /* check bind setting end */
  
  for( int ii = 0; ii < BTN; ii++ )
  {
    int INpin = g_inputs[ii];
    int OUTpin = g_outputs[ii];
    int bind = g_bind[ii];
    int BINDpin = -1;

    g_counter[ii] = 0;
    g_flags[ii] = 0;

    pinMode(INpin, INPUT);
    digitalWrite(INpin, HIGH);

    if( bind != -1 )
    {
      BINDpin = g_inputs[bind];
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
  
  delay(g_delay1);
} /* loop */

/******************************************************************************/
void autoSetup(void)
{
  for( int ii = 0; ii < BTN; ii++ )
  {
    int INpin = g_inputs[ii];
    int OUTpin = g_outputs[ii];
    if( OUTpin == -1 )
    {
      continue;
    }
    int stats = digitalRead(INpin);
    if( stats == LOW && g_flags[ii] == 0 )
    {
      Serial.print( "button" );
      Serial.print( ii + 1 );
      Serial.println( " is AutoFire." );
      
      g_flags[ii] = 1;
    }
    
  }

} /* autoSetup */

/******************************************************************************/
void autoClear(void)
{
  for( int ii = 0; ii < BTN; ii++ )
  {
      g_flags[ii] = 0;
  }
} /* autoClear */

/******************************************************************************/
void oneStep(void)
{
  for( int ii = 0; ii < BTN; ii++ )
  {
    int INpin = g_inputs[ii];
    int OUTpin = g_outputs[ii];
    int bind = g_bind[ii];
    int BINDpin = -1;
    int BINDval = HIGH;
    if( bind != -1 )
    {
      BINDpin = g_inputs[bind];
      BINDval = digitalRead(BINDpin);
    }
    int INval = digitalRead(INpin);

    if( BINDval == LOW || g_flags[ii] != 0 ){
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
    int timing = g_timings[num];
    if( BINDval == LOW )
    {
      /* バインド側のボタンが押されている場合、
       * バインドの設定を使う
       */
      timing = g_timings[g_bind[num]];
    }
    
    g_counter[num]++;
    if( g_counter[num] >= timing ) {
      g_counter[num] = 0;

      int val = digitalRead(OUTpin);
      digitalWrite(OUTpin, (val==HIGH ? LOW : HIGH));
    }
  }else{
    /* ボタンが押されていない状態 */
    g_counter[num] = g_timings[num]; /* ボタンを押したタイミングでLOWになるように設定 */
    digitalWrite(OUTpin, HIGH);
  }
} /* oneStepAuto */

/******************************************************************************/
