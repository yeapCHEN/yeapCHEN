

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"



/* 
	BT05����ģ��Ľ��߷�����
	STATE:PC1
	RXD:PA9
	TXD:PA10
	GND:GND
	VCC:3.3V
	EN:PC6
	�������ؽ�ȥ�����ʹ��ģ���ṩ�İ�׿APKӦ����������ģ��BT05�����Ӻú�
	ָʾ��D3����LCD��ʾ���ӣ�������LCD��ʾδ���ӡ�ͨ���ṩ��APK������з��ͺͽ��ղ���
	�ֻ����������String�ϻ���ʾhello world
	Ҳ����ʹ�ô��ڵ������ֹ۲���ԡ�
*/
	

int main()
{
	u8 t=0,k=0;
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	LED_Init();
	USART1_Init(9600);
	TFTLCD_Init();			//LCD��ʼ��
	
	
	FRONT_COLOR=GREEN;
	LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,16,"PRECHIN");
	LCD_ShowString(10,30,tftlcd_data.width,tftlcd_data.height,16,"www.prechin.com");
	LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,16,"DX-BT05 Test");
	
	FRONT_COLOR=RED;
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1))	 //�ж��Ƿ���ֻ���������  ���ӳɹ�D3ָʾ���� ������
		{
			led2=0;
			LCD_ShowString(10,110,tftlcd_data.width,tftlcd_data.height,16,"DX-BT05 Connect...");
		}
		else
		{
			led2=1;
			LCD_ShowString(10,110,tftlcd_data.width,tftlcd_data.height,16,"DX-BT05 Disconnect ");
		}
		
		t++;
		if(t==15)
		{
			t=0;
			k++;
			led1=!led1;
			LCD_ShowString(10,140,tftlcd_data.width,tftlcd_data.height,16,"hello world");
			printf("hello world\r\n");	 //BT05����ģ��һֱ��������
			if(k==5)
			{
				k=0;
				LCD_ShowString(10,140,tftlcd_data.width,tftlcd_data.height,16,"             ");	
			}
		}
		delay_ms(10);		
	}		
}
