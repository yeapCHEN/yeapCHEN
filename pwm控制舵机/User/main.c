

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "pwm.h"

int main()
 {		
 	u16 pwmval1=749;//1.5ms->90��   //500��1ms  750�� 1.5ms
	u8 t,PWM_Flag;
	//delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
 	TIM3_PWM_Init(9999,143);	 //����Ƶ��  ����=��arr+1��*��psc+1��/CLK 
	// PWM Ƶ��=72*10^6/(9999+1)/(143+1)=50Hz
	 
	 //10000��  20ms  �� 500��ʾ1ms  �ɵ�����0.5-2.5ms     ��250-1250
   	while(1)
	{
 		
		t++;
		if(t%100==0)//ÿ��100��ȡ��
		{
			PWM_Flag=!PWM_Flag;
			if(PWM_Flag==0)
				pwmval1=1249;//2.5ms 180��		
			else
				pwmval1=249;//0.5ms  0��
			
			
			
			TIM_SetCompare2(TIM3,pwmval1);
		}
		delay_ms(10);
		
	}	 
 }
 
 