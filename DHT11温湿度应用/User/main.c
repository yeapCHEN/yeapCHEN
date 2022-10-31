

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"
#include "dht11.h"


void data_pros()	//���ݴ�����
{
	u8 temp;  	    
	u8 humi;
	u8 temp_buf[3],humi_buf[3];
	DHT11_Read_Data(&temp,&humi);
	temp_buf[0]=temp/10+0x30;	
	temp_buf[1]=temp%10+0x30;
	temp_buf[2]='\0';
	LCD_ShowString(55,100,tftlcd_data.width,tftlcd_data.height,16,temp_buf);
		
	humi_buf[0]=humi/10+0x30;	
	humi_buf[1]=humi%10+0x30;
	humi_buf[2]='\0';
	LCD_ShowString(55,130,tftlcd_data.width,tftlcd_data.height,16,humi_buf);		
}

int main()
{
	u8 i=0;
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	LED_Init();
	USART1_Init(9600);
	TFTLCD_Init();			//LCD��ʼ��
	
	
	FRONT_COLOR=BLACK;
	LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,16,"PRECHIN STM32F1");
	LCD_ShowString(10,30,tftlcd_data.width,tftlcd_data.height,16,"www.prechin.net");
	LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,16,"DHT11 Test");
	LCD_ShowString(10,100,tftlcd_data.width,tftlcd_data.height,16,"Temp:   C");
	LCD_ShowString(10,130,tftlcd_data.width,tftlcd_data.height,16,"Humi:   %RH");
	FRONT_COLOR=RED;
	
	while(DHT11_Init())	//���DS18B20�Ƿ���
	{
		LCD_ShowString(130,50,tftlcd_data.width,tftlcd_data.height,16,"Error   ");	
		delay_ms(500);		
	}
	LCD_ShowString(130,50,tftlcd_data.width,tftlcd_data.height,16,"Success");
	
	while(1)
	{
		
		i++;
		if(i%20==0)
		{
			led1=!led1;
			data_pros();  	 //��ȡһ��DHT11��������Ҫ����100ms
		}
		
		delay_ms(10);
			
	}
}
