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
#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
#define Middle_angle 0
int EXTI15_10_IRQHandler(void);
int Balance(float angle,float gyro);
int Velocity(int encoder_left,int encoder_right);
void Set_Pwm(int motor_left,int motor_right);
void Key(void);
int PWM_Limit(int IN,int max,int min);
u8 Turn_Off(float angle);
void Get_Angle(void);
int myabs(int a);
#endif
