#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"




int main()
{
	u8 i=0;
	u8 t;
	u8 len;	
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	LED_Init();
	USART3_Init(9600);
	
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			printf("\r\n�����͵���ϢΪ: ");
			for(t=0;t<len;t++)
			{
				USART_SendData(USART3, USART_RX_BUF[t]);         //�򴮿�1��������
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			printf("\r\n");//���뻻��
			USART_RX_STA=0;
		}
		else
		{
			i++;
			if(i%5000==0)
			{
				printf("����PZ6808L-F1������ USART3ͨ��ʵ��\r\n");
				printf("www.prechin.cn\r\n");
			}
			if(i%200==0)
				printf("\r\n����������,�Իس�������\r\n");  
			if(i%20==0)
				led1=!led1;//��˸LED,��ʾϵͳ��������.
			delay_ms(10);   
		}
	}
}
