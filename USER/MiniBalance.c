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
#include "stm32f10x.h"
#include "sys.h"
u8 Flag_Stop=1;                 			//���ֹͣ��־λ��Ĭ�Ϲر�
int Encoder_Left,Encoder_Right;       //���ұ��������������
float Angle_Balance,Gyro_Balance; 		//ƽ����� ƽ��������
int Motor_Left;
int main(void)
{ 

	MY_NVIC_PriorityGroupConfig(2);	//�����жϷ���
	delay_init();
	LED_Init();    	            //��ʱ������ʼ��	
//	JTAG_Set(JTAG_SWD_DISABLE);     //�ر�JTAG�ӿ�
//	JTAG_Set(SWD_ENABLE);           //��SWD�ӿ� �������������SWD�ӿڵ���
	KEY_Init();                     //������ʼ��
	MiniBalance_PWM_Init(7199,0);	//��ʼ��PWM 10KHZ������������� �����ʼ������ӿ� ��ΪMiniBalance_PWM_Init(9999,35) 200HZ
	Encoder_Init_TIM2();            //�������ӿ�
	Encoder_Init_TIM4();            //��ʼ��������2
	uart_init(115200);
	IIC_Init();                     //IIC��ʼ��
	MPU6050_initialize();           //MPU6050��ʼ��	
	DMP_Init();                     //��ʼ��DMP 
  MiniBalance_EXTI_Init();	//MPU6050 5ms��ʱ�жϳ�ʼ��
  Lcd_Init();
	Lcd_Clear(WHITE);
 Lcd_Clear(BLACK);
 Lcd_Clear(RED);
 Lcd_Clear(GREEN);
 Lcd_Clear(BLUE);
 Lcd_Clear(WHITE);
//  Gui_DrawFont_GBK16(8,0,BLUE,GRAY0,"����ģ��");
	while(1)
		{
		if(Angle_Balance<0)
		{
			Gui_DrawFont_char(25,0,'-');
			Angle_Balance = -Angle_Balance;
			}
		else
		Gui_DrawFont_char(25,0,' ');
    LCD_ShowNum(35,0,Angle_Balance,3);
		LED=1;
		printf("%d\r\n",Angle_Balance);
//		printf("%d\r\n",Flag_Stop);
//    LCD_ShowFloatNum1(0,0,Pitch,4,RED,WHITE,16)
		
//		oled_show();                	//��ʾ����

	} 
}

