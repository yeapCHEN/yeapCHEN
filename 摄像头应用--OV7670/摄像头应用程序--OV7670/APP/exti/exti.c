#include "exti.h"
#include "led.h"
#include "SysTick.h"
#include "key.h"
#include "ov7670.h"


/*******************************************************************************
* �� �� ��         : My_EXTI_Init
* ��������		   : �ⲿ�жϳ�ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void My_EXTI_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2);//ѡ��GPIO�ܽ������ⲿ�ж���·
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);//ѡ��GPIO�ܽ������ⲿ�ж���·
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);//ѡ��GPIO�ܽ������ⲿ�ж���·
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//ѡ��GPIO�ܽ������ⲿ�ж���·
	
	
	//EXTI0 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//EXTI0�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	
	//EXTI2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//EXTI2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	
	//EXTI3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//EXTI3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//EXTI4 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//EXTI4�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line2|EXTI_Line3|EXTI_Line4; 
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
}

/*******************************************************************************
* �� �� ��         : EXTI0_IRQHandler
* ��������		   : �ⲿ�ж�0����
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)==1)
	{
		delay_ms(10);
		if(K_UP==1)
		{
			led2=0;
		}	
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}

/*******************************************************************************
* �� �� ��         : EXTI3_IRQHandler
* ��������		   : �ⲿ�ж�3����
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3)==1)
	{
		delay_ms(10);
		if(K_DOWN==0)
		{	
			led2=1;
		}
		
	}
	EXTI_ClearITPendingBit(EXTI_Line3);
}

/*******************************************************************************
* �� �� ��         : EXTI2_IRQHandler
* ��������		   : �ⲿ�ж�2����
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2)==1)
	{
		delay_ms(10);
		if(K_LEFT==0)
		{
			led3=1;
		}
		
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}

/*******************************************************************************
* �� �� ��         : EXTI4_IRQHandler
* ��������		   : �ⲿ�ж�4����
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4)==1)
	{
		delay_ms(10);
		if(K_RIGHT==0)
		{
			led3=0;
		}
		
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}








u8 ov_sta;	//֡�жϱ��
 //�ⲿ�ж�5~9�������
void EXTI9_5_IRQHandler(void)
{		 		
	if(EXTI_GetITStatus(EXTI_Line7)==SET)	//��8�ߵ��ж�
	{      
		OV7670_WRST=0;	//��λдָ��		  		 
		OV7670_WRST=1;	
		OV7670_WREN=1;	//����д��FIFO 	 
		ov_sta++;		//֡�жϼ�1 
	}
	EXTI_ClearITPendingBit(EXTI_Line7);  //���EXTI8��·����λ						  
} 
//�ⲿ�ж�7��ʼ��
void EXTI7_Init(void)
{												  
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);
 	   	 
	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�0 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//�����ȼ�0 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���   
}

