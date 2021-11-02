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
#include "control.h"
/**************************************************************************
Function: Control function
Input   : none
Output  : none
函数功能：所有的控制代码都在这里面
         5ms定时中断由MPU6050的INT引脚触发
         严格保证采样和数据处理的时间同步	
入口参数：无
返回  值：无				 
**************************************************************************/
int EXTI15_10_IRQHandler(void) 
{    
	 static u8 Flag_Target;						//控制函数相关变量，提供10ms基准
   int Balance_Pwm,Velocity_Pwm;		//平衡环PWM变量，速度环PWM变量
  int Motor_Left,Motor_Right;      //电机PWM变量 应是Motor的 向Moto致敬	
	 if(INT==0)
	 {  

	
			EXTI->PR=1<<12;                         //清除中断标志位   
			Flag_Target=!Flag_Target;
			Get_Angle();                            //更新姿态	5ms读取一次
			Encoder_Left=Read_Encoder(2);          //读取左轮编码器的值，前进为正，后退为负
			Encoder_Right=-Read_Encoder(4);         //读取右轮编码器的值，前进为正，后退为负
																							//左轮A相接TIM2_CH1,右轮A相接TIM4_CH2,故这里两个编码器的极性相同
			if(Flag_Target==1)                      //实际上10ms控制一次
				return 0;	                                               
			Key();                                  //扫描按键状态 单击可启停电机
			
			Balance_Pwm =Balance(Angle_Balance,Gyro_Balance); //平衡PID控制	  Gyro_Balance平衡角速度极性：前倾为正，后倾为负
    	Velocity_Pwm=Velocity(Encoder_Left,Encoder_Right);//速度环PID控制	 记住，速度反馈是正反馈，就是小车快的时候要慢下来就需要再跑快一点
			
			Motor_Left=Balance_Pwm+Velocity_Pwm;              //计算左轮电机最终PWM
			Motor_Right=Balance_Pwm+Velocity_Pwm;             //计算右轮电机最终PWM
//			printf("%d\r\n",Motor_Left);
			Motor_Left=PWM_Limit(Motor_Left,6900,-6900);
			Motor_Right=PWM_Limit(Motor_Right,6900,-6900);		//PWM限幅
//       printf("%d\r\n",Motor_Left);
			if(Turn_Off(Angle_Balance)==0)                    //如果不存在异常
				Set_Pwm(Motor_Left,Motor_Right);  			//赋值给PWM寄存器  
			
	 }       	
	 return 0;	  
} 

/**************************************************************************
Function: Vertical PD control
Input   : Angle:angle；Gyro：angular velocity
Output  : none
函数功能：直立PD控制		
入口参数：Angle:角度；Gyro：角速度
返回  值：无
**************************************************************************/	
int Balance(float Angle,float Gyro)
{  
	 float Balance_Kp=360,Balance_Kd=4.8;//直立环PD参数
   float Angle_bias,Gyro_bias;
	 int balance;
	 Angle_bias=Middle_angle-Angle;                       //求出平衡的角度中值 和机械相关
	 Gyro_bias=0-Gyro;
	 balance=-Balance_Kp*Angle_bias-Gyro_bias*Balance_Kd;   		//计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
	 return balance;
}

/**************************************************************************
Function: Speed PI control
Input   : encoder_left：Left wheel encoder reading；encoder_right：Right wheel encoder reading
Output  : Speed control PWM
函数功能：速度控制PWM		
入口参数：encoder_left：左轮编码器读数；encoder_right：右轮编码器读数
返回  值：速度控制PWM
**************************************************************************/
int Velocity(int encoder_left,int encoder_right)
{  
	  float Velocity_Kp=400,Velocity_Ki=2;//速度环PI参数
    static float velocity,Encoder_Least,Encoder_bias;
	  static float Encoder_Integral;
   //=============速度PI控制器=======================//	
		Encoder_Least =0-(encoder_left+encoder_right);                    //获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零） 
		Encoder_bias *= 0.8;		                                          //一阶低通滤波器       
		Encoder_bias += Encoder_Least*0.2;	                              //一阶低通滤波器  
																																			//相当于上次偏差的0.8 + 本次偏差的0.2，减缓速度差值，减少对直立的干扰  
		Encoder_Integral +=Encoder_bias;                                  //积分出位移 积分时间：10ms
		if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //积分限幅
		if(Encoder_Integral<-10000)	  Encoder_Integral=-10000;            //积分限幅	
		velocity=-Encoder_bias*Velocity_Kp-Encoder_Integral*Velocity_Ki;        //速度控制	
		if(Turn_Off(Angle_Balance)==1||Flag_Stop==1)   Encoder_Integral=0;      //电机关闭后清除积分
	  return velocity;
}

/**************************************************************************
Function: Assign to PWM register
Input   : motor_left：Left wheel PWM；motor_right：Right wheel PWM
Output  : none
函数功能：赋值给PWM寄存器
入口参数：左轮PWM、右轮PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int motor_left,int motor_right)	
{
	
  if(motor_left>0)	    BIN1=0,			BIN2=1; //前进 
	else           			  BIN1=1,			BIN2=0; //后退
	PWMB=myabs(motor_left);	
  if(motor_right>0)			AIN2=1,			AIN1=0;	//前进
	else 	        			  AIN2=0,			AIN1=1; //后退
	PWMA=myabs(motor_right);
}

/**************************************************************************
Function: PWM limiting range
Input   : IN：Input  max：Maximum value  min：Minimum value
Output  : Output
函数功能：限制PWM赋值 
入口参数：IN：输入参数  max：限幅最大值  min：限幅最小值
返回  值：限幅后的值
**************************************************************************/
int PWM_Limit(int IN,int max,int min)
{
	int OUT = IN;
	if(OUT>max) OUT = max;
	if(OUT<min) OUT = min;
	return OUT;
}
/**************************************************************************
Function: Press the key to modify the car running state
Input   : none
Output  : none
函数功能：按键修改小车运行状态 
入口参数：无
返回  值：无
**************************************************************************/
void Key(void)
{	
	u8 tmp;
	tmp=click(); 
	if(tmp==1) Flag_Stop=!Flag_Stop;	//单击控制小车的启停
}

/**************************************************************************
Function: If abnormal, turn off the motor
Input   : angle：Car inclination；voltage：Voltage
Output  : 1：abnormal；0：normal
函数功能：异常关闭电机		
入口参数：angle：小车倾角
返回  值：1：异常  0：正常
**************************************************************************/	
u8 Turn_Off(float angle)
{
	u8 temp;
	if(angle<-40||angle>40||1==Flag_Stop)//倾角大于40度关闭电机
	{	                                   //Flag_Stop置1关闭电机             
		temp=1;                                         
		AIN1=0;                                            
		AIN2=0;
		BIN1=0;
		BIN2=0;
		LED=0;
	}
	else
		temp=0;
	return temp;			
}
	
/**************************************************************************
Function: Get angle
Input   : way：The algorithm of getting angle 1：DMP  2：kalman  3：Complementary filtering
Output  : none
函数功能：获取角度	
入口参数：无
返回  值：无
**************************************************************************/	
void Get_Angle(void)
{ 
	Read_DMP();                      	 //读取加速度、角速度、倾角
	Angle_Balance=Pitch;             	 //更新平衡倾角,前倾为正，后倾为负
	Gyro_Balance=gyro[1];              //更新平衡角速度,前倾为正，后倾为负
}
/*******************************0*******************************************
Function: Absolute value function
Input   : a：Number to be converted
Output  : unsigned int
函数功能：绝对值函数
入口参数：a：需要计算绝对值的数
返回  值：无符号整型
**************************************************************************/	
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
