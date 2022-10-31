

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "time.h"



int main()
{
	u32 i=0;
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	LED_Init();
	USART1_Init(9600);
	TIM4_Init(1000,35999);  //��ʱʱ��0.5s ����1hz
	while(1)
	{
		i++;
		if(i>500000)
		{
			led1=1;	
			if(i>1000000)i=0;
		}
		else
		{
			led1=0;
		}	  		
	}
}
