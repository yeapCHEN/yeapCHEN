/* ���س������Ҫ����SD������USBģ��̽�Ƭ�̽ӵ�USB�ˣ���USB������PC��USB�ں�STM32 USB�ڣ�
	�����Զ���װUSB����ʶ��SD����Flash�ڴ档��PC���ҵĵ����ڼ��ɿ�����2��Ӳ�̡�D1ָʾ����˸
	��ʾ�����������У�����SD������Flash��д��ʱ��D2����LCD��ʾ״̬*/

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"
#include "key.h"
#include "malloc.h" 
#include "sd.h"
#include "flash.h"
#include "ff.h" 
#include "fatfs_app.h"
#include "font_show.h"


//��ֲUSBʱ���ͷ�ļ�
#include "mass_mal.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "memory.h"	    
#include "usb_bot.h"


//USBʹ������/����
//enable:0,�Ͽ�
//       1,��������	   
void USB_Port_Set(u8 enable)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);    //ʹ��PORTAʱ��		 
	if(enable)_SetCNTR(_GetCNTR()&(~(1<<1)));//�˳��ϵ�ģʽ
	else
	{	  
		_SetCNTR(_GetCNTR()|(1<<1));  // �ϵ�ģʽ
		GPIOA->CRH&=0XFFF00FFF;
		GPIOA->CRH|=0X00033000;
		PAout(12)=0;	    		  
	}
} 

int main()
{
	u8 offline_cnt=0;
	u8 tct=0;
	u8 USB_STA;
	u8 Divece_STA;
	u32 sd_size;
	u8 sd_buf[6];
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	LED_Init();
	USART1_Init(9600);
	TFTLCD_Init();			//LCD��ʼ��
	KEY_Init();
	EN25QXX_Init();				//��ʼ��EN25Qxx  
	
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	
	FRONT_COLOR=RED;//��������Ϊ��ɫ 	
	LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,16,"USB Card Reader TEST");	
	LCD_ShowString(10,30,tftlcd_data.width,tftlcd_data.height,16,"www.prechin.net");
	
	if(SD_Init()!=0)
	{	
		LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,16,"SD Card Error!");	//���SD������
	}
	else //SD ������
	{   															  
		LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,16,"SD Card Size:     MB"); 
 		Mass_Memory_Size[0]=(long long)SD_GetSectorCount()*512;//�õ�SD���������ֽڣ�����SD����������4G��ʱ��,��Ҫ�õ�����u32����ʾ
	    Mass_Block_Size[0] =512;//��Ϊ������Init����������SD���Ĳ����ֽ�Ϊ512��,��������һ����512���ֽ�.
	    Mass_Block_Count[0]=Mass_Memory_Size[0]/Mass_Block_Size[0];
		sd_size=Mass_Block_Count[0]>>11;  //��ʾSD��������   MB
		sd_buf[0]=sd_size/10000+0x30;
		sd_buf[1]=sd_size%10000/1000+0x30;
		sd_buf[2]=sd_size%10000%1000/100+0x30;
		sd_buf[3]=sd_size%10000%1000%100/10+0x30;
		sd_buf[4]=sd_size%10000%1000%100%10+0x30;
		sd_buf[5]='\0';
		LCD_ShowString(10+13*8,50,tftlcd_data.width,tftlcd_data.height,16,sd_buf); 
 	}
	if(EN25QXX_ReadID()!=EN25Q64)
	{
		LCD_ShowString(10,70,tftlcd_data.width,tftlcd_data.height,16,"EN25QXX Error!          ");	//���EN25QXX����
	}
	else //SPI FLASH ����
	{   														 	 
		Mass_Memory_Size[1]=1024*1024*6;//ǰ6M�ֽ�
	    Mass_Block_Size[1] =512;//��Ϊ������Init����������SD���Ĳ����ֽ�Ϊ512��,��������һ����512���ֽ�.
	    Mass_Block_Count[1]=Mass_Memory_Size[1]/Mass_Block_Size[1];
		LCD_ShowString(10,70,tftlcd_data.width,tftlcd_data.height,16,"EN25QXX FLASH Size:6144KB");
	} 
	
 	delay_ms(1800);
 	USB_Port_Set(0); 	//USB�ȶϿ�
	delay_ms(300);
   	USB_Port_Set(1);	//USB�ٴ�����	   
 	LCD_ShowString(10,90,tftlcd_data.width,tftlcd_data.height,16,"USB Connecting...");//��ʾ���ڽ������� 	    
	printf("USB Connecting...\r\n");	 
   	//USB����
 	USB_Interrupts_Config();    
 	Set_USBClock();   
 	USB_Init();	    
	delay_ms(1800);
	
	while(1)
	{
		delay_ms(1);
		if(USB_STA!=USB_STATUS_REG)//״̬�ı��� 
		{	 						   			  	   
			if(USB_STATUS_REG&0x01)//����д		  
			{
				led2=0;
				LCD_ShowString(10,120,tftlcd_data.width,tftlcd_data.height,16,"USB Writing...");//��ʾUSB����д������	 
				printf("USB Writing...\r\n"); 
			}
			if(USB_STATUS_REG&0x02)//���ڶ�
			{
				led2=0;
				LCD_ShowString(10,120,tftlcd_data.width,tftlcd_data.height,16,"USB Reading...");//��ʾUSB���ڶ�������  		 
				printf("USB Reading...\r\n"); 		 
			}	 										  
			if(USB_STATUS_REG&0x04)
				LCD_ShowString(10,140,tftlcd_data.width,tftlcd_data.height,16,"USB Write Err ");//��ʾд�����
			else 
				LCD_Fill(10,140,tftlcd_data.width,140+16,WHITE);//�����ʾ	  
			if(USB_STATUS_REG&0x08)
				LCD_ShowString(10,140,tftlcd_data.width,tftlcd_data.height,16,"USB Read  Err ");//��ʾ��������
			else 
				LCD_Fill(10,140,tftlcd_data.width,140+16,WHITE);//�����ʾ      
			USB_STA=USB_STATUS_REG;//��¼����״̬
		}
		if(Divece_STA!=bDeviceState) 
		{
			if(bDeviceState==CONFIGURED)
			{
				LCD_ShowString(10,90,tftlcd_data.width,tftlcd_data.height,16,"USB Connected    ");//��ʾUSB�����Ѿ�����	
				printf("USB Connected\r\n");
			}
			else 
			{
				LCD_ShowString(10,90,tftlcd_data.width,tftlcd_data.height,16,"USB DisConnected ");//��ʾUSB���γ���
				printf("USB DisConnected\r\n");
			}
			Divece_STA=bDeviceState;
		}
		tct++;
		if(tct==200)
		{
			tct=0;
			led2=1;
			led1=!led1;//��ʾϵͳ������
			if(USB_STATUS_REG&0x10)
			{
				offline_cnt=0;//USB������,�����offline������
				bDeviceState=CONFIGURED;
			}
			else//û�еõ���ѯ 
			{
				offline_cnt++;  
				if(offline_cnt>10)bDeviceState=UNCONNECTED;//2s��û�յ����߱��,����USB���γ���
			}
			USB_STATUS_REG=0;
		}
	}	
}

