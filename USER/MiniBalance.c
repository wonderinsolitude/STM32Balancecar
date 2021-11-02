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
#include "stm32f10x.h"
#include "sys.h"
u8 Flag_Stop=1;                 			//电机停止标志位，默认关闭
int Encoder_Left,Encoder_Right;       //左右编码器的脉冲计数
float Angle_Balance,Gyro_Balance; 		//平衡倾角 平衡陀螺仪
int Motor_Left;
int main(void)
{ 

	MY_NVIC_PriorityGroupConfig(2);	//设置中断分组
	delay_init();
	LED_Init();    	            //延时函数初始化	
//	JTAG_Set(JTAG_SWD_DISABLE);     //关闭JTAG接口
//	JTAG_Set(SWD_ENABLE);           //打开SWD接口 可以利用主板的SWD接口调试
	KEY_Init();                     //按键初始化
	MiniBalance_PWM_Init(7199,0);	//初始化PWM 10KHZ，用于驱动电机 如需初始化电调接口 改为MiniBalance_PWM_Init(9999,35) 200HZ
	Encoder_Init_TIM2();            //编码器接口
	Encoder_Init_TIM4();            //初始化编码器2
	uart_init(115200);
	IIC_Init();                     //IIC初始化
	MPU6050_initialize();           //MPU6050初始化	
	DMP_Init();                     //初始化DMP 
  MiniBalance_EXTI_Init();	//MPU6050 5ms定时中断初始化
  Lcd_Init();
	Lcd_Clear(WHITE);
 Lcd_Clear(BLACK);
 Lcd_Clear(RED);
 Lcd_Clear(GREEN);
 Lcd_Clear(BLUE);
 Lcd_Clear(WHITE);
//  Gui_DrawFont_GBK16(8,0,BLUE,GRAY0,"电子模块");
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
		
//		oled_show();                	//显示屏打开

	} 
}

