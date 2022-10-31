#include "time.h"
#include "led.h"
#include "tftlcd.h"
#include "key.h"
#include "ball.h"


/*******************************************************************************
* �� �� ��         : TIM4_Init
* ��������		   : TIM4��ʼ������
* ��    ��         : per:��װ��ֵ
					 psc:��Ƶϵ��
* ��    ��         : ��
*******************************************************************************/
void TIM4_Init(u16 per,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//ʹ��TIM4ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period=per;   //�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //������ʱ���ж�
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//��ʱ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	
	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��	
}


/*******************************************************************************
* �� �� ��         : TIM4_IRQHandler
* ��������		   : TIM4�жϺ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void TIM4_IRQHandler(void)
{
	static u8 i=0,j=0;
	if(TIM_GetITStatus(TIM4,TIM_IT_Update))
	{
		if(game.life)
		{
			GUI_DotP(pai.x,pai.y,BLACK);   //������ʾ
			button=KEY_Scan(1);
			if(button==KEY_LEFT)
			{
				i++;
				if(i>=3)
				{
					i=0;
					pai.x-=5;
					if(pai.x<=1)pai.x=1;
				}	
			}
			if(button==KEY_RIGHT)
			{
				j++;
				if(j>=3)
				{
					j=0;
					pai.x+=5;
					if(pai.x>=209)pai.x=209;
				}	
			}
			GUI_DotP(pai.x,pai.y,WHITE);   //��ʾ�µ�����
		}		
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	
}


void TIM3_Init(u16 per,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseInitStructure.TIM_Period=per;   //�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}

void TIM3_IRQHandler(void)
{
	static u8 i=0;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update))
	{
		if(game.life)
		{
			GUI_DotB(ball.x,ball.y,BLACK);	//�����һ����
				
			switch(ball.dir)		//���з����ж�
			{
				case 1: ball.x+=1,ball.y-=1;break;	  //���Ͻ� 
				case 2: ball.x-=1,ball.y-=1;break;	  //���Ͻ� 
				case 3: ball.x-=1,ball.y+=1;break;	  //���½� 
				case 4: ball.x+=1,ball.y+=1;break;	  //���½�
			}
			GUI_DotB(ball.x,ball.y,WHITE);	//���µ�λ����ʾ
			
			if(ball.y==1)  //���������ϱ߽� �����߽�Ҳ��2�ַ����ж�
			{
				if(ball.dir==1)		//���ϽǷ�������
				{
					ball.dir=4;  //��������	
				}
				else
				{
					ball.dir=3;
				}
			}
		
			if((ball.y==pai.y-5)&&(ball.x>=pai.x-5&&ball.x<=pai.x+30))  //���������±߽� �����߽�Ҳ��2�ַ����ж�
			{
				if(ball.dir==4)		//���½Ƿ�������
				{
					ball.dir=1;  //��������	
				}
				else
				{
					ball.dir=2;
				}
				game.sco=1;
				game.score+=10;  //�÷�	
			}
			if(ball.y>pai.y-5)
			{
				game.life=0; //��Ϸ����
			}
			if(ball.x==234)  //���������ұ߽� �����߽�Ҳ��2�ַ����ж�
			{
				if(ball.dir==1)		//���ϽǷ�������
				{
					ball.dir=2;  //��������	
				}
				else
				{
					ball.dir=3;
				}
			}
		
			if(ball.x==1)  //����������߽� �����߽�Ҳ��2�ַ����ж�
			{
				if(ball.dir==3)		//���½Ƿ�������
				{
					ball.dir=4;  //��������	
				}
				else
				{
					ball.dir=1;
				}
			}
			i++;
			if(i==30)
			{
				i=0;
				led2=!led2;
			}		
		}		
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}


void TIM5_Init(u16 per,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM��ʼ��
	TIM_TimeBaseInitStructure.TIM_Period=per;   //�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIMx					 
}

void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)!= RESET)
	{
		button=KEY_Scan(1);
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
}


