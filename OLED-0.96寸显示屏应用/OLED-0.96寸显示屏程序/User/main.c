

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "oled.h"
#include "picture.h"


//OLDE 0.9��4��SPIģ��������STM32��������߷�ʽ
//GND  �ӵ�Դ��
//VCC  ��5V��3.3v��Դ
//D0   ��PD6��SCL��
//D1   ��PD7��SDA��
//RES  ��PD4
//DC   ��PD5
//CS   ��PD3

int main()
{
	u8 i=0;
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	LED_Init();
	USART1_Init(9600);
	OLED_Init(); //OLED��ʼ��
	
	OLED_DrawBMP(0,0,128,8,(u8 *)pic1);	  //���Ҫ������ʾ����Ҫ��ȡģ��ʽ�޸�����ȡģ
	delay_ms(1500);
	OLED_Clear();

	OLED_ShowString(0,0,"PRECHIN",16);

	OLED_ShowFontHZ(16*0,20,0,16,1);	 //�������пƼ�
	OLED_ShowFontHZ(16*1,20,1,16,1);
	OLED_ShowFontHZ(16*2,20,2,16,1);
	OLED_ShowFontHZ(16*3,20,3,16,1);
	OLED_ShowFontHZ(16*4,20,4,16,1);
	OLED_ShowFontHZ(16*5,20,5,16,1);

	OLED_ShowString(0,40,"www.prechin.cn",16);
	OLED_Refresh_Gram();  //ˢ��GRAM����

	while(1)
	{
		i++;
		if(i%20==0)
		{
			led1=!led1;
		}
		delay_ms(10);		
	}
}
