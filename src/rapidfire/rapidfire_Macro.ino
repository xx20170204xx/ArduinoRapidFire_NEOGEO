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

/* EOF */
