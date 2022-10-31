

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "pwm.h"

int main()
 {		
 	u16 pwmval1=749;//1.5ms->90度   //500是1ms  750是 1.5ms
	u8 t,PWM_Flag;
	//delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
 	TIM3_PWM_Init(9999,143);	 //不分频。  周期=（arr+1）*（psc+1）/CLK 
	// PWM 频率=72*10^6/(9999+1)/(143+1)=50Hz
	 
	 //10000下  20ms  则 500表示1ms  可调区间0.5-2.5ms     即250-1250
   	while(1)
	{
 		
		t++;
		if(t%100==0)//每数100下取反
		{
			PWM_Flag=!PWM_Flag;
			if(PWM_Flag==0)
				pwmval1=1249;//2.5ms 180°		
			else
				pwmval1=249;//0.5ms  0°
			
			
			
			TIM_SetCompare2(TIM3,pwmval1);
		}
		delay_ms(10);
		
	}	 
 }
 
 