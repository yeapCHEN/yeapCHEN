

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"
#include "key.h"
#include "nrf24l01.h"


void data_pros()	//���ݴ�����
{
	u8 key;
	static u8 mode=2;  //ģʽѡ��
	u8 rx_buf[33]="www.prechin.cn";
	static u16 t=0;
	while(1)		//�ȴ��������½���ѡ���ͻ��ǽ���
	{
		key=KEY_Scan(0);	
		if(key==KEY_UP)	   //����ģʽ
		{
			mode=0;
			LCD_ShowString(10,140,tftlcd_data.width,tftlcd_data.height,16,"RX_Mode");
			LCD_ShowString(10,160,tftlcd_data.width,tftlcd_data.height,16,"Received Data:");
			LCD_ShowString(120,160,tftlcd_data.width,tftlcd_data.height,16,"                ");
			break;
		}
		if(key==KEY_DOWN)	 //����ģʽ
		{
			mode=1;
			LCD_ShowString(10,140,tftlcd_data.width,tftlcd_data.height,16,"TX_Mode");
			LCD_ShowString(10,160,tftlcd_data.width,tftlcd_data.height,16,"Send Data:    ");
			LCD_ShowString(120,160,tftlcd_data.width,tftlcd_data.height,16,"              ");
			break;
		}	
	}
	
	if(mode==0)		//����ģʽ
	{	
		NRF24L01_RX_Mode();	
		while(1)
		{
			if(NRF24L01_RxPacket(rx_buf)==0) //���յ�������ʾ
			{
				rx_buf[32]='\0';
				LCD_ShowString(120,160,tftlcd_data.width,tftlcd_data.height,16,rx_buf);
				break;			
			}
			else
			{
				delay_ms(1);
			}
			t++;
			if(t==1000)
			{
				t=0;
				led2=~led2; //һ���Ӹı�һ��״̬
			}	
		}	
	}
	if(mode==1)		 //����ģʽ
	{
				
		NRF24L01_TX_Mode();
		while(1)
		{
			if(NRF24L01_TxPacket(rx_buf)==TX_OK)
			{
				LCD_ShowString(120,160,tftlcd_data.width,tftlcd_data.height,16,rx_buf);
				break;	
			}
			else
			{
				LCD_ShowString(120,160,tftlcd_data.width,tftlcd_data.height,16,"Send Data Failed  ");
		
			}	
		}	
	}
}

int main()
{
	u8 i=0;
	u16 rd=0;
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	LED_Init();
	USART1_Init(9600);
	TFTLCD_Init();			//LCD��ʼ��
	KEY_Init();
	NRF24L01_Init();
	
	
	FRONT_COLOR=BLACK;
	LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,16,"PRECHIN STM32F1");
	LCD_ShowString(10,30,tftlcd_data.width,tftlcd_data.height,16,"www.prechin.net");
	LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,16,"NRF24L01 Test");
	LCD_ShowString(10,70,tftlcd_data.width,tftlcd_data.height,16,"K_UP:RX_Mode  K_DOWN:TX_Mode");
	FRONT_COLOR=RED;
	
	while(NRF24L01_Check())	 //���NRF24L01�Ƿ����
	{
		LCD_ShowString(140,50,tftlcd_data.width,tftlcd_data.height,16,"Error   ");			
	}
	LCD_ShowString(140,50,tftlcd_data.width,tftlcd_data.height,16,"Success");
	
	while(1)
	{
		data_pros(); 
		i++;
		if(i%20==0)
		{
			led1=!led1;
		}
		
		delay_ms(10);
			
	}
}
