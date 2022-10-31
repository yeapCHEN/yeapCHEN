/* 
	��USBʵ���ʱ��һ��Ҫ��USB�˵Ķ̽�Ƭ�̽ӵ�USB�࣬���ʹ��CAN����Ҫ�̽ӵ�CAN�ࡣ
	ʹ��USB�߲��ڿ�����USBģ��ӿ��ϣ�ע�ⲻ��USB�������ؿڣ�����ʱ���Ի��Զ�ʶ��װ������
	���δʶ�����ֶ���װ�ڡ�\5--��������\4. ���ø�������������ļ����ڵ�STM32 USB���⴮������
	����ʹ�ø����̡�
*/

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"
#include "key.h"
#include "malloc.h" 



//��ֲUSBʱ���ͷ�ļ�
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"



int main()
{
	u16 len=0,times=0,t;
	u8 usbstatus=0;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	LED_Init();
	USART1_Init(9600);
	TFTLCD_Init();			//LCD��ʼ��
	
	delay_ms(1800);
	USB_Port_Set(0); 	//USB�ȶϿ�
	delay_ms(700);
	USB_Port_Set(1);	//USB�ٴ�����
	Set_USBClock();
	USB_Interrupts_Config();
	USB_Init();
	while(1)
	{
		if(usbstatus!=bDeviceState)//USB����״̬�����˸ı�.
		{
			usbstatus=bDeviceState;//��¼�µ�״̬
			if(usbstatus==CONFIGURED)
			{
				LCD_ShowString(10,90,tftlcd_data.width,tftlcd_data.height,16,"USB Connected    ");//��ʾUSB�����Ѿ�����
				usb_printf("USB Connected\r\n");
				led2=0;//D2��
			}
			else
			{
				LCD_ShowString(10,90,tftlcd_data.width,tftlcd_data.height,16,"USB DisConnected ");//��ʾUSB���γ���
				usb_printf("USB disConnected\r\n");
				led2=1;//D2��
			}
		}
		if(USB_USART_RX_STA&0x8000)
		{					   
			len=USB_USART_RX_STA&0x3FFF;//�õ��˴ν��յ������ݳ���
			usb_printf("\r\n�����͵���Ϣ����Ϊ:%d\r\n\r\n",len);
			for(t=0;t<len;t++)
			{
				USB_USART_SendData(USB_USART_RX_BUF[t]);//���ֽڷ�ʽ,���͸�USB 
			}
			usb_printf("\r\n\r\n");//���뻻��
			USB_USART_RX_STA=0;
		}
		else
		{
			times++;
			if(times%5000==0)
			{
				usb_printf("\r\n���пƼ�STM32������USB���⴮��ʵ��\r\n");
				usb_printf("www.prechin.com\r\n\r\n");
			}
			if(times%200==0)usb_printf("����������,�Իس�������\r\n");  
			if(times%30==0)led1=!led1;//��˸LED,��ʾϵͳ��������.
			delay_ms(10);   
		}					
	}	
}

