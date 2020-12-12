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

#include "rapidfire.h"
#include "rapidfire_Macro.h"
#include "config.h"

int g_autoPin =  INPIN_AUTO;
int g_clearPin = INPIN_CLEAR;
/* Warning : Sync Pin Is No Checked  */
int g_syncINPin = -1;

/******************************************************************************/

void setup() {
  delay(3000);
  setupButton();
  showInfo();
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
void setupButton(void)
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
} /* setupButton */
/******************************************************************************/
void showInfo( void )
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
  
} /* showInfo */
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
