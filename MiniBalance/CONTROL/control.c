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
#include "control.h"
/**************************************************************************
Function: Control function
Input   : none
Output  : none
�������ܣ����еĿ��ƴ��붼��������
         5ms��ʱ�ж���MPU6050��INT���Ŵ���
         �ϸ�֤���������ݴ����ʱ��ͬ��	
��ڲ�������
����  ֵ����				 
**************************************************************************/
int EXTI15_10_IRQHandler(void) 
{    
	 static u8 Flag_Target;						//���ƺ�����ر������ṩ10ms��׼
   int Balance_Pwm,Velocity_Pwm;		//ƽ�⻷PWM�������ٶȻ�PWM����
  int Motor_Left,Motor_Right;      //���PWM���� Ӧ��Motor�� ��Moto�¾�	
	 if(INT==0)
	 {  

	
			EXTI->PR=1<<12;                         //����жϱ�־λ   
			Flag_Target=!Flag_Target;
			Get_Angle();                            //������̬	5ms��ȡһ��
			Encoder_Left=Read_Encoder(2);          //��ȡ���ֱ�������ֵ��ǰ��Ϊ��������Ϊ��
			Encoder_Right=-Read_Encoder(4);         //��ȡ���ֱ�������ֵ��ǰ��Ϊ��������Ϊ��
																							//����A���TIM2_CH1,����A���TIM4_CH2,�����������������ļ�����ͬ
			if(Flag_Target==1)                      //ʵ����10ms����һ��
				return 0;	                                               
			Key();                                  //ɨ�谴��״̬ ��������ͣ���
			
			Balance_Pwm =Balance(Angle_Balance,Gyro_Balance); //ƽ��PID����	  Gyro_Balanceƽ����ٶȼ��ԣ�ǰ��Ϊ��������Ϊ��
    	Velocity_Pwm=Velocity(Encoder_Left,Encoder_Right);//�ٶȻ�PID����	 ��ס���ٶȷ�����������������С�����ʱ��Ҫ����������Ҫ���ܿ�һ��
			
			Motor_Left=Balance_Pwm+Velocity_Pwm;              //�������ֵ������PWM
			Motor_Right=Balance_Pwm+Velocity_Pwm;             //�������ֵ������PWM
//			printf("%d\r\n",Motor_Left);
			Motor_Left=PWM_Limit(Motor_Left,6900,-6900);
			Motor_Right=PWM_Limit(Motor_Right,6900,-6900);		//PWM�޷�
//       printf("%d\r\n",Motor_Left);
			if(Turn_Off(Angle_Balance)==0)                    //����������쳣
				Set_Pwm(Motor_Left,Motor_Right);  			//��ֵ��PWM�Ĵ���  
			
	 }       	
	 return 0;	  
} 

/**************************************************************************
Function: Vertical PD control
Input   : Angle:angle��Gyro��angular velocity
Output  : none
�������ܣ�ֱ��PD����		
��ڲ�����Angle:�Ƕȣ�Gyro�����ٶ�
����  ֵ����
**************************************************************************/	
int Balance(float Angle,float Gyro)
{  
	 float Balance_Kp=360,Balance_Kd=4.8;//ֱ����PD����
   float Angle_bias,Gyro_bias;
	 int balance;
	 Angle_bias=Middle_angle-Angle;                       //���ƽ��ĽǶ���ֵ �ͻ�е���
	 Gyro_bias=0-Gyro;
	 balance=-Balance_Kp*Angle_bias-Gyro_bias*Balance_Kd;   		//����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	 return balance;
}

/**************************************************************************
Function: Speed PI control
Input   : encoder_left��Left wheel encoder reading��encoder_right��Right wheel encoder reading
Output  : Speed control PWM
�������ܣ��ٶȿ���PWM		
��ڲ�����encoder_left�����ֱ�����������encoder_right�����ֱ���������
����  ֵ���ٶȿ���PWM
**************************************************************************/
int Velocity(int encoder_left,int encoder_right)
{  
	  float Velocity_Kp=400,Velocity_Ki=2;//�ٶȻ�PI����
    static float velocity,Encoder_Least,Encoder_bias;
	  static float Encoder_Integral;
   //=============�ٶ�PI������=======================//	
		Encoder_Least =0-(encoder_left+encoder_right);                    //��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩 
		Encoder_bias *= 0.8;		                                          //һ�׵�ͨ�˲���       
		Encoder_bias += Encoder_Least*0.2;	                              //һ�׵�ͨ�˲���  
																																			//�൱���ϴ�ƫ���0.8 + ����ƫ���0.2�������ٶȲ�ֵ�����ٶ�ֱ���ĸ���  
		Encoder_Integral +=Encoder_bias;                                  //���ֳ�λ�� ����ʱ�䣺10ms
		if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //�����޷�
		if(Encoder_Integral<-10000)	  Encoder_Integral=-10000;            //�����޷�	
		velocity=-Encoder_bias*Velocity_Kp-Encoder_Integral*Velocity_Ki;        //�ٶȿ���	
		if(Turn_Off(Angle_Balance)==1||Flag_Stop==1)   Encoder_Integral=0;      //����رպ��������
	  return velocity;
}

/**************************************************************************
Function: Assign to PWM register
Input   : motor_left��Left wheel PWM��motor_right��Right wheel PWM
Output  : none
�������ܣ���ֵ��PWM�Ĵ���
��ڲ���������PWM������PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int motor_left,int motor_right)	
{
	
  if(motor_left>0)	    BIN1=0,			BIN2=1; //ǰ�� 
	else           			  BIN1=1,			BIN2=0; //����
	PWMB=myabs(motor_left);	
  if(motor_right>0)			AIN2=1,			AIN1=0;	//ǰ��
	else 	        			  AIN2=0,			AIN1=1; //����
	PWMA=myabs(motor_right);
}

/**************************************************************************
Function: PWM limiting range
Input   : IN��Input  max��Maximum value  min��Minimum value
Output  : Output
�������ܣ�����PWM��ֵ 
��ڲ�����IN���������  max���޷����ֵ  min���޷���Сֵ
����  ֵ���޷����ֵ
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
�������ܣ������޸�С������״̬ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Key(void)
{	
	u8 tmp;
	tmp=click(); 
	if(tmp==1) Flag_Stop=!Flag_Stop;	//��������С������ͣ
}

/**************************************************************************
Function: If abnormal, turn off the motor
Input   : angle��Car inclination��voltage��Voltage
Output  : 1��abnormal��0��normal
�������ܣ��쳣�رյ��		
��ڲ�����angle��С�����
����  ֵ��1���쳣  0������
**************************************************************************/	
u8 Turn_Off(float angle)
{
	u8 temp;
	if(angle<-40||angle>40||1==Flag_Stop)//��Ǵ���40�ȹرյ��
	{	                                   //Flag_Stop��1�رյ��             
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
Input   : way��The algorithm of getting angle 1��DMP  2��kalman  3��Complementary filtering
Output  : none
�������ܣ���ȡ�Ƕ�	
��ڲ�������
����  ֵ����
**************************************************************************/	
void Get_Angle(void)
{ 
	Read_DMP();                      	 //��ȡ���ٶȡ����ٶȡ����
	Angle_Balance=Pitch;             	 //����ƽ�����,ǰ��Ϊ��������Ϊ��
	Gyro_Balance=gyro[1];              //����ƽ����ٶ�,ǰ��Ϊ��������Ϊ��
}
/*******************************0*******************************************
Function: Absolute value function
Input   : a��Number to be converted
Output  : unsigned int
�������ܣ�����ֵ����
��ڲ�����a����Ҫ�������ֵ����
����  ֵ���޷�������
**************************************************************************/	
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
