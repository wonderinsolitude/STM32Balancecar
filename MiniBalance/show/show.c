/***********************************************
��˾����ݸ��΢�����ܿƼ����޹�˾
Ʒ�ƣ�WHEELTEC
������wheeltec.net
�Ա����̣�shop114407458.taobao.com 
����ͨ: https://minibalance.aliexpress.com/store/4455017
�汾��5.7
�޸�ʱ�䣺2021-04-29

Company: WeiHong Co.Ltd
Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version:5.7
Update��2021-04-29

All rights reserved
***********************************************/
#include "show.h"
/**************************************************************************
Function: OLED display
Input   : none
Output  : none
�������ܣ�OLED��ʾ
��ڲ�������
����  ֵ����
**************************************************************************/
void oled_show(void)
{
//		OLED_Display_On();  //��ʾ����
//		//=============��һ����ʾС��ģʽ=======================//	
//		 OLED_ShowString(0,0,"DMP");
//     OLED_ShowString(60,0,"Normal");
//		//=============��������ʾ������1=======================//	
//		                      OLED_ShowString(00,20,"EncoLEFT");
//		if( Encoder_Left<0)		OLED_ShowString(80,20,"-"),
//		                      OLED_ShowNumber(95,20,-Encoder_Left,3,12);
//		else                 	OLED_ShowString(80,20,"+"),
//		                      OLED_ShowNumber(95,20, Encoder_Left,3,12);
//  	//=============��������ʾ������2=======================//		
//		                      OLED_ShowString(00,30,"EncoRIGHT");
//		if(Encoder_Right<0)		OLED_ShowString(80,30,"-"),
//		                      OLED_ShowNumber(95,30,-Encoder_Right,3,12);
//		else               		OLED_ShowString(80,30,"+"),
//		                      OLED_ShowNumber(95,30,Encoder_Right,3,12);	
//		//=============��������ʾ�Ƕ��뿪��=======================//
//		                      OLED_ShowString(0,50,"Angle");
//		if(Angle_Balance<0)		OLED_ShowString(48,50,"-");
//		if(Angle_Balance>=0)	OLED_ShowString(48,50,"+");
//													OLED_ShowNumber(53,50,myabs((int)Angle_Balance),3,12);
//		if(Flag_Stop)		      OLED_ShowString(90,50,"OFF");
//		if(!Flag_Stop)	      OLED_ShowString(90,50,"ON ");
//		//=============ˢ��=======================//
//		OLED_Refresh_Gram();	
}
