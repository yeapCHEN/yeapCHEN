

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"



/* 
	BT05蓝牙模块的接线方法：
	STATE:PC1
	RXD:PA9
	TXD:PA10
	GND:GND
	VCC:3.3V
	EN:PC6
	程序下载进去后可以使用模块提供的安卓APK应用连接蓝牙模块BT05，连接好后
	指示灯D3亮，LCD显示连接，否则灭，LCD显示未连接。通过提供的APK软件进行发送和接收测试
	手机测试软件在String上会显示hello world
	也可以使用串口调试助手观察测试。
*/
	

int main()
{
	u8 t=0,k=0;
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	LED_Init();
	USART1_Init(9600);
	TFTLCD_Init();			//LCD初始化
	
	
	FRONT_COLOR=GREEN;
	LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,16,"PRECHIN");
	LCD_ShowString(10,30,tftlcd_data.width,tftlcd_data.height,16,"www.prechin.com");
	LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,16,"DX-BT05 Test");
	
	FRONT_COLOR=RED;
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1))	 //判断是否和手机蓝牙连接  连接成功D3指示灯亮 否则灭
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
			printf("hello world\r\n");	 //BT05蓝牙模块一直发送数据
			if(k==5)
			{
				k=0;
				LCD_ShowString(10,140,tftlcd_data.width,tftlcd_data.height,16,"             ");	
			}
		}
		delay_ms(10);		
	}		
}
