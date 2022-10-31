#include "tim.h"

u32 count = 0;
u32 frequency = 0;

void time2_init(void)
{					 
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseInitTypeStruct;		 

	TIM_TimeBaseInitTypeStruct.TIM_Prescaler = 7199;  //tim2  72000000 10us
	TIM_TimeBaseInitTypeStruct.TIM_CounterMode = TIM_CounterMode_Up;//������
	TIM_TimeBaseInitTypeStruct.TIM_Period =	9999+1;		//10000  100ms					  
	TIM_TimeBaseInitTypeStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitTypeStruct);	 		   

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);	 //TIM2��ʼ��	   
	TIM_Cmd(TIM2,DISABLE);
}

void time3_init(void)
{
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseInitTypeStruct;
	//���� TIMx �ⲿʱ��ģʽ 2
	TIM_ETRClockMode2Config(TIM3,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0);
	
	TIM_TimeBaseInitTypeStruct.TIM_Prescaler = 0;
	TIM_TimeBaseInitTypeStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitTypeStruct.TIM_Period =	65535;							  
	TIM_TimeBaseInitTypeStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitTypeStruct);

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3,DISABLE);	  //ʧ��

}

void time1_init(void)
{					 //��������adת����TIM1����ADC
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseInitTypeStruct;
	TIM_OCInitTypeDef TIM_OCInitTypeStruct;		 

	TIM_TimeBaseInitTypeStruct.TIM_Prescaler = 71; //72��Ƶ
	TIM_TimeBaseInitTypeStruct.TIM_CounterMode = TIM_CounterMode_Up; //������
	TIM_TimeBaseInitTypeStruct.TIM_Period =	1;	//�Զ���װֵ��1us����һ��						  
	TIM_TimeBaseInitTypeStruct.TIM_ClockDivision = TIM_CKD_DIV1;//ʱ�Ӳ���Ƶ
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitTypeStruct);	//TIM1��ʼ�� 		   	 	   
	TIM_Cmd(TIM1,DISABLE);	 //TIM1��ʹ��

	TIM_OCInitTypeStruct.TIM_OCMode = TIM_OCMode_PWM1; 					   //���Ҳ�Ǳ������ó�pwmģʽ 
	TIM_OCInitTypeStruct.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitTypeStruct.TIM_Pulse = 1;
	TIM_OCInitTypeStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM1, &TIM_OCInitTypeStruct);

	TIM_CtrlPWMOutputs(TIM1, ENABLE);				  //����Ǳ����е�
}

void time_init(void)
{
	time2_init();  
	time3_init();
	time1_init();
}
void time_enable(void)
{
	TIM_Cmd(TIM2,ENABLE);
	TIM_Cmd(TIM3,ENABLE);	
}


