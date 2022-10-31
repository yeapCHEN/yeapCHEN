//VS1003/1053 MP3ģ��������STM32��������߷�ʽ��
//5V---5V
//GND--GND
//XRST--PG8
//MISO--PA6
//MOSI--PA7
//SCLK--PA5
//DREQ--PG7
//XCS--PE6
//XDCS--PG6

//ʵ��ǰ�����SD������SD����Ŀ¼�·������ǹ����ڵ�SD���ļ������ļ�

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
#include "mp3player.h"


void GUI_DisplayInit()
{
	FRONT_COLOR=RED;//��������Ϊ��ɫ
	LCD_ShowFont16Char(80, 0, "���пƼ�");
    LCD_ShowFont12Char(90, 21, "PRECHIN");
    LCD_ShowFont12Char(60, 42, "www.prechin.com");
    LCD_ShowFont12Char(76, 63, "MP3������ʵ��");
    LCD_ShowFont12Char(0, 84, "��������Ϊ��230");
    LCD_ShowFont12Char(0, 105, "���ڲ��ŵĸ���Ϊ��");
    LCD_ShowFont12Char(0, 167, "KEY_UP������+");
    LCD_ShowFont12Char(0, 188, "KEY_WOWN������-");
    LCD_ShowFont12Char(0, 209, "KEY_RIGHT����һ��");
}

FileNameTypeDef filename[30];

int main()
{
	FATFS fs;
	uint32_t sdCapacity, free;
    uint8_t dat[9] = {"0:/MUSIC"}, i=0, j=0, k=0;  //Ҫ��ʾ��ͼƬ���ļ���ַ
    uint8_t misicFile[30];
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	LED_Init();
	USART1_Init(9600);
	TFTLCD_Init();			//LCD��ʼ��
	KEY_Init();
	EN25QXX_Init();				//��ʼ��EN25Q128	  
	MP3_Init();
	
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	
	FRONT_COLOR=RED;//��������Ϊ��ɫ 
	while(SD_Init()!=0)
	{	
		LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,16,"SD Card Error!");
	}
	FATFS_Init();							//Ϊfatfs��ر��������ڴ�				 
  	f_mount(&fs,"0:",1); 					//����SD�� 
	
	while(FATFS_GetFree("0:", &sdCapacity, &free) != 0)
    {
        LCD_ShowFont12Char(0, 21, "FATfs error!");   
    }
	
	/* �������� */
    MP3_Volume = 230;
    MP3_AudioSetting(MP3_Volume);
    /* ��ʼ����ʾ */
	GUI_DisplayInit();
	
	/*  ɨ���ļ���ַ�������е��ļ� */
    FATFS_ScanFiles(dat, filename);
	printf("%s%s\r\n",filename[0].name,filename[0].type);
	printf("%s%s\r\n",filename[1].name,filename[1].type);
	while(1)
	{	
		
		/* �ж��Ƿ���PM3ͼƬ�ļ� */
		if((filename[i].type[1] == 'm') && (filename[i].type[2] == 'p') &&
           (filename[i].type[3] == '3')) 
		{
			/* �����ļ�·��,������ļ�·�� */
			k = 0;
			while(*(dat + k) != '\0')
			{
				*(misicFile + k) = *(dat + k);
				k++;
			}
			
            /* ·��֮�����һб�� */
			*(misicFile + k) = '/';
			k++;

            /* ����ļ��������� */
			j = 0;
			while(filename[i].name[j] != '\0')
			{
				*(misicFile + k) = filename[i].name[j];
				k++;
				j++;	
			}

            /* ����ļ���׺ */
			j = 0;
			while(filename[i].type[j] != '\0')
			{
				*(misicFile + k) = filename[i].type[j];
				k++;
				j++;	
			}

            /* �ļ�������һ���������� */
            *(misicFile + k) = '\0';

			/* ��ʾ���ŵĸ��������Ÿ��� */
			LCD_Fill(0, 126, tftlcd_data.height, 165, WHITE); //�����ʾλ��
			LCD_ShowFont12Char(0, 126, misicFile); 
					
            MP3_PlaySong(misicFile);
		}
		
		i++;
        if(i > 30)
        {
            i = 0;
        }	
		
	}
	
}

void HardFault_Handler(void)
{
  while(1)
  {
	  led2=!led2;
	  delay_ms(100);
  }
}
