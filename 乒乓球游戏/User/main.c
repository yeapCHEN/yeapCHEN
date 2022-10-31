#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"
#include "time.h"
#include "key.h"
#include "ball.h"



int main()
{
	u8 i;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	LED_Init();
	USART1_Init(9600);
	TFTLCD_Init();			//LCD��ʼ��
	KEY_Init();
	
	LCD_Clear(BLUE);
	LCD_ShowPictureEx(0, 0, 240, 400); 
	
	TIM5_Init(100,7199);
	while(button!= KEY_UP) //�ȴ�����K_UP����
	{
		FRONT_COLOR=RED;
		LCD_ShowString(20,10,tftlcd_data.width,tftlcd_data.height,16,"Press K_UP key to Enter...");
		delay_ms(200);
		LCD_ShowString(20,10,tftlcd_data.width,tftlcd_data.height,16,"                          ");
		delay_ms(200);
	}
	
	TIM_ITConfig(TIM5,TIM_IT_Update,DISABLE);
	TIM_Cmd(TIM5, DISABLE);  //ʧ��TIMx
	game_init_show();
	TIM3_Init(100,7199);
	TIM4_Init(50,7199);	
	while(1)
	{
		ball_play();
	}
}
