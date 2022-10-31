

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
#include "key.h"
#include "font_show.h"
#include "exti.h"
#include "time.h" 
#include "string.h"		
#include "math.h"	 
#include "ov7670.h"



extern u8 ov_sta;	//��exit.c���涨��
extern u8 ov_frame;	//��time.c���涨��



//����LCD��ʾ
void camera_refresh(void)
{
	u32 j;
	u16 i;
 	u16 color;
	u16 temp;
	if(ov_sta)//��֡�жϸ��£�
	{
		
		//LCD_Set_Window((tftlcd_data.width-320)/2,(tftlcd_data.height-240)/2,320,240-1);//����ʾ�������õ���Ļ����
		LCD_Set_Window(0,(tftlcd_data.height-240)/2,320-1,240-1);//����ʾ�������õ���Ļ����
		
		OV7670_RRST=0;				//��ʼ��λ��ָ�� 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				//��λ��ָ����� 
		OV7670_RCK_H;
		/*for(i=0;i<240;i++)   //���ַ�ʽ���Լ����κβ���,�����ٶȺ���
		{
			for(j=0;j<320;j++)
			{
				OV7670_RCK_L;
				color=GPIOF->IDR&0XFF;	//������
				OV7670_RCK_H; 
				color<<=8;  
				OV7670_RCK_L;
				color|=GPIOF->IDR&0XFF;	//������
				OV7670_RCK_H; 
				LCD_DrawFRONT_COLOR(j,i,color);
			}
		}*/
		for(j=0;j<76800;j++)   //���ַ�ʽ�����TFT�ڲ���ʾ������ƼĴ���ֵ �ٶȽϿ�
		{
			OV7670_RCK_L;
			color=GPIOF->IDR&0XFF;	//������
			OV7670_RCK_H; 
			color<<=8;  
			OV7670_RCK_L;
			color|=GPIOF->IDR&0XFF;	//������
			OV7670_RCK_H; 
			LCD_WriteData_Color(color); 
			//printf("%x  ",color);
			//if(j%20==0)printf("\r\n");
			//delay_us(50);
		}   							  
 		ov_sta=0;					//����֡�жϱ��
		ov_frame++; 

	} 
}

const u8*LMODE_TBL[5]={"Auto","Sunny","Cloudy","Office","Home"};
const u8*EFFECTS_TBL[7]={"Normal","Negative","B&W","Redish","Greenish","Bluish","Antique"};	//7����Ч 


int main()
{
	u8 i=0;
	u8 key;
	u8 lightmode=0,saturation=2,brightness=2,contrast=2;
	u8 effect=0;
	u8 sbuf[15];
	u8 count;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	LED_Init();
	USART1_Init(9600);
	TFTLCD_Init();			//LCD��ʼ��
	KEY_Init();
	EN25QXX_Init();				//��ʼ��EN25Q128	  
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	
	FRONT_COLOR=RED;//��������Ϊ��ɫ 
	
//	while(SD_Init()!=0)
//	{	
//		LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,16,"SD Card Error!");
//	}
//	FATFS_Init();							//Ϊfatfs��ر��������ڴ�				 
//  	f_mount(fs[0],"0:",1); 					//����SD�� 
// 	f_mount(fs[1],"1:",1); 				//����FLASH.
		
	LCD_ShowFont12Char(10, 10, "���пƼ�");
	LCD_ShowFont12Char(10, 30, "www.prechin.net");    
	LCD_ShowFont12Char(10, 50, "����ͷӦ��--OV7670");
	i=OV7670_Init();
	printf("i=%d\n",i);
	while(OV7670_Init())//��ʼ��OV7670
	{
		LCD_ShowString(10,80,tftlcd_data.width,tftlcd_data.height,16,"OV7670 Error!");
		delay_ms(200);
		LCD_Fill(10,80,239,206,WHITE);
		delay_ms(200);
	}
 	LCD_ShowString(10,80,tftlcd_data.width,tftlcd_data.height,16,"OV7670 OK!     ");
	delay_ms(1500);	 
	OV7670_Light_Mode(0);
	OV7670_Color_Saturation(2);
	OV7670_Brightness(2);
	OV7670_Contrast(2);
 	OV7670_Special_Effects(0);
		
	TIM4_Init(10000,7199);			//10Khz����Ƶ��,1�����ж�									  
	EXTI7_Init();			
	OV7670_Window_Set(12,176,240,320);	//���ô���	
  	OV7670_CS=0;	
	LCD_Clear(BLACK);
	while(1)
	{
		key=KEY_Scan(0);
		if(key)count=20;
		switch(key)
		{
			case KEY_UP:           //�ƹ�ģʽ����
				lightmode++;
				if(lightmode>4)lightmode=0;
				OV7670_Light_Mode(lightmode);
				sprintf((char*)sbuf,"%s",LMODE_TBL[lightmode]);
				break;
			case KEY_DOWN:         //���Ͷ�
				saturation++;
				if(saturation>4)saturation=0;
				OV7670_Color_Saturation(saturation);
				sprintf((char*)sbuf,"Saturation:%d",(char)saturation-2);
				break;	
			case KEY_LEFT:        //����
				brightness++;
				if(brightness>4)brightness=0;
				OV7670_Brightness(brightness);
				sprintf((char*)sbuf,"Brightness:%d",(char)brightness-2);
				break;
			case KEY_RIGHT:     //�Աȶ�
				contrast++;
				if(contrast>4)contrast=0;
				OV7670_Contrast(contrast);
				sprintf((char*)sbuf,"Contrast:%d",(char)contrast-2);
				break;
		}
		if(count)
		{
			count--;
			LCD_ShowString((tftlcd_data.width-240)/2+30,(tftlcd_data.height-320)/2+60,200,16,16,sbuf);
		}
		camera_refresh();//������ʾ
		i++;
		if(i%20==0)
		{
			led1=!led1;
		}
		//delay_ms(5);	
	}
	
}
