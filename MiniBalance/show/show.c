/***********************************************
公司：东莞市微宏智能科技有限公司
品牌：WHEELTEC
官网：wheeltec.net
淘宝店铺：shop114407458.taobao.com 
速卖通: https://minibalance.aliexpress.com/store/4455017
版本：5.7
修改时间：2021-04-29

Company: WeiHong Co.Ltd
Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version:5.7
Update：2021-04-29

All rights reserved
***********************************************/
#include "show.h"
/**************************************************************************
Function: OLED display
Input   : none
Output  : none
函数功能：OLED显示
入口参数：无
返回  值：无
**************************************************************************/
void oled_show(void)
{
//		OLED_Display_On();  //显示屏打开
//		//=============第一行显示小车模式=======================//	
//		 OLED_ShowString(0,0,"DMP");
//     OLED_ShowString(60,0,"Normal");
//		//=============第三行显示编码器1=======================//	
//		                      OLED_ShowString(00,20,"EncoLEFT");
//		if( Encoder_Left<0)		OLED_ShowString(80,20,"-"),
//		                      OLED_ShowNumber(95,20,-Encoder_Left,3,12);
//		else                 	OLED_ShowString(80,20,"+"),
//		                      OLED_ShowNumber(95,20, Encoder_Left,3,12);
//  	//=============第四行显示编码器2=======================//		
//		                      OLED_ShowString(00,30,"EncoRIGHT");
//		if(Encoder_Right<0)		OLED_ShowString(80,30,"-"),
//		                      OLED_ShowNumber(95,30,-Encoder_Right,3,12);
//		else               		OLED_ShowString(80,30,"+"),
//		                      OLED_ShowNumber(95,30,Encoder_Right,3,12);	
//		//=============第六行显示角度与开关=======================//
//		                      OLED_ShowString(0,50,"Angle");
//		if(Angle_Balance<0)		OLED_ShowString(48,50,"-");
//		if(Angle_Balance>=0)	OLED_ShowString(48,50,"+");
//													OLED_ShowNumber(53,50,myabs((int)Angle_Balance),3,12);
//		if(Flag_Stop)		      OLED_ShowString(90,50,"OFF");
//		if(!Flag_Stop)	      OLED_ShowString(90,50,"ON ");
//		//=============刷新=======================//
//		OLED_Refresh_Gram();	
}
