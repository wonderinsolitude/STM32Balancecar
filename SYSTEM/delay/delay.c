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
#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//�����Ҫʹ��OS,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"							//ucos ʹ��	  
#endif

static u8  fac_us=0;							//us��ʱ������			   
static u16 fac_ms=0;							//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��

#if SYSTEM_SUPPORT_OS							//���SYSTEM_SUPPORT_OS������,˵��Ҫ֧��OS��(������UCOS).
//��delay_us/delay_ms��Ҫ֧��OS��ʱ����Ҫ������OS��صĺ궨��ͺ�����֧��
//������3���궨��:
//delay_osrunning:���ڱ�ʾOS��ǰ�Ƿ���������,�Ծ����Ƿ����ʹ����غ���
//delay_ostickspersec:���ڱ�ʾOS�趨��ʱ�ӽ���,delay_init�����������������ʼ��systick
//delay_osintnesting:���ڱ�ʾOS�ж�Ƕ�׼���,��Ϊ�ж����治���Ե���,delay_msʹ�øò����������������
//Ȼ����3������:
//delay_osschedlock:��������OS�������,��ֹ����
//delay_osschedunlock:���ڽ���OS�������,���¿�������
//delay_ostimedly:����OS��ʱ,���������������.

//�����̽���UCOSII��UCOSIII��֧��,����OS,�����вο�����ֲ
//֧��UCOSII
#ifdef 	OS_CRITICAL_METHOD										//OS_CRITICAL_METHOD������,˵��Ҫ֧��UCOSII				
#define delay_osrunning		OSRunning						//OS�Ƿ����б��,0,������;1,������
#define delay_ostickspersec	OS_TICKS_PER_SEC	//OSʱ�ӽ���,��ÿ����ȴ���
#define delay_osintnesting 	OSIntNesting			//�ж�Ƕ�׼���,���ж�Ƕ�״���
#endif

//֧��UCOSIII
#ifdef 	CPU_CFG_CRITICAL_METHOD								//CPU_CFG_CRITICAL_METHOD������,˵��Ҫ֧��UCOSIII	
#define delay_osrunning		OSRunning						//OS�Ƿ����б��,0,������;1,������
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OSʱ�ӽ���,��ÿ����ȴ���
#define delay_osintnesting 	OSIntNestingCtr		//�ж�Ƕ�׼���,���ж�Ƕ�״���
#endif
/**************************************************************************
Function: Turn off task scheduling when us level delay occurs (to prevent interruption of us level delay)
Input   : none
Output  : none
�������ܣ�us����ʱʱ,�ر��������(��ֹ���us���ӳ�)
��ڲ�������	
����  ֵ����
**************************************************************************/
void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD   		//ʹ��UCOSIII
	OS_ERR err; 
	OSSchedLock(&err);							  //UCOSIII�ķ�ʽ,��ֹ���ȣ���ֹ���us��ʱ
#else											          //����UCOSII
	OSSchedLock();								    //UCOSII�ķ�ʽ,��ֹ���ȣ���ֹ���us��ʱ
#endif
}

/**************************************************************************
Function: Us level delay, resume task scheduling
Input   : none
Output  : none
�������ܣ�us����ʱ,�ָ��������
��ڲ�������	
����  ֵ����
**************************************************************************/
void delay_osschedunlock(void)
{	
#ifdef CPU_CFG_CRITICAL_METHOD   	//ʹ��UCOSIII
	OS_ERR err; 
	OSSchedUnlock(&err);						//UCOSIII�ķ�ʽ,�ָ�����
#else															//����UCOSII
	OSSchedUnlock();								//UCOSII�ķ�ʽ,�ָ�����
#endif
}

/**************************************************************************
Function: Call the delay function of OS to delay
Input   : ticks��Beat number of delay
Output  : none
�������ܣ�����OS�Դ�����ʱ������ʱ
��ڲ�����ticks:��ʱ�Ľ�����	
����  ֵ����
**************************************************************************/
void delay_ostimedly(u32 ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
	OS_ERR err; 
	OSTimeDly(ticks,OS_OPT_TIME_PERIODIC,&err);	//UCOSIII��ʱ��������ģʽ
#else
	OSTimeDly(ticks);														//UCOSII��ʱ
#endif 
}
 
/**************************************************************************
Function: SYSTICK interrupts the service function for use with ucos
Input   : none
Output  : none
�������ܣ�systick�жϷ�����,ʹ��ucosʱ�õ�
��ڲ�������	
����  ֵ����
**************************************************************************/
void SysTick_Handler(void)
{	
	if(delay_osrunning==1)						//OS��ʼ����,��ִ�������ĵ��ȴ���
	{
		OSIntEnter();										//�����ж�
		OSTimeTick();       						//����ucos��ʱ�ӷ������               
		OSIntExit();       	 						//���������л����ж�
	}
}
#endif

			   
/**************************************************************************
Function: Initialize the delay function
Input   : SYSCLK��System clock
Output  : none
�������ܣ���ʼ���ӳٺ���
��ڲ�����SYSCLK:ϵͳʱ��	
����  ֵ����
**************************************************************************/
//��ʹ��ucos��ʱ��,�˺������ʼ��ucos��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
void delay_init()
{
#if SYSTEM_SUPPORT_OS  								//�����Ҫ֧��OS.
	u32 reload;
#endif
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	fac_us=SystemCoreClock/8000000;				//Ϊϵͳʱ�ӵ�1/8  
#if SYSTEM_SUPPORT_OS  									//�����Ҫ֧��OS.
	reload=SystemCoreClock/8000000;				//ÿ���ӵļ������� ��λΪM  
	reload*=1000000/delay_ostickspersec;	//����delay_ostickspersec�趨���ʱ��
																				//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��1.86s����	
	fac_ms=1000/delay_ostickspersec;			//����OS������ʱ�����ٵ�λ	   

	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 											//ÿ1/delay_ostickspersec���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//����SYSTICK    

#else
	fac_ms=(u16)fac_us*1000;					//��OS��,����ÿ��ms��Ҫ��systickʱ����   
#endif
}								    

#if SYSTEM_SUPPORT_OS  							//�����Ҫ֧��OS.
/**************************************************************************
Function: Delay function��us��
Input   : nus��The number of us to delay
Output  : none
�������ܣ���ʱ������us��
��ڲ�����nus��Ҫ��ʱ��us��	
����  ֵ����
**************************************************************************/	    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;					//LOAD��ֵ	    	 
	ticks=nus*fac_us; 								//��Ҫ�Ľ�����	  		 
	tcnt=0;
	delay_osschedlock();							//��ֹOS���ȣ���ֹ���us��ʱ
	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;					//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};
	delay_osschedunlock();						//�ָ�OS����									    
}
/**************************************************************************
Function: Delay function��ms��
Input   : mus��The number of ms to delay
Output  : none
�������ܣ���ʱ������us��
��ڲ�����mus��Ҫ��ʱ��ms��	
����  ֵ����
**************************************************************************/
void delay_ms(u16 nms)
{	
	if(delay_osrunning&&delay_osintnesting==0)	//���OS�Ѿ�������,���Ҳ������ж�����(�ж����治���������)	    
	{		 
		if(nms>=fac_ms)														//��ʱ��ʱ�����OS������ʱ������ 
		{ 
   			delay_ostimedly(nms/fac_ms);					//OS��ʱ
		}	
		nms%=fac_ms;															//OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
	delay_us((u32)(nms*1000));					       //��ͨ��ʽ��ʱ  
}
#else 
//����ucosʱ
/**************************************************************************
Function: Delay function��us��
Input   : nus��The number of us to delay
Output  : none
�������ܣ���ʱ������us��
��ڲ�����nus��Ҫ��ʱ��us��	
����  ֵ����
**************************************************************************/			    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 								//ʱ�����	  		 
	SysTick->VAL=0x00;        								//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));			//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;      					 				//��ռ�����	 
}
/**************************************************************************
Function: Delay function��ms��
Input   : mus��The number of ms to delay
Output  : none
�������ܣ���ʱ������us��
��ڲ�����mus��Ҫ��ʱ��ms��	
����  ֵ����
**************************************************************************/
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;						//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;												//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));			//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;       								//��ռ�����	  	    
} 
#endif 







































