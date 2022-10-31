//VS1003/1053 MP3模块与普中STM32开发板接线方式：
//5V---5V
//GND--GND
//XRST--PG8
//MISO--PA6
//MOSI--PA7
//SCLK--PA5
//DREQ--PG7
//XCS--PE6
//XDCS--PG6

//实验前请插入SD卡，在SD卡根目录下放入我们光盘内的SD卡文件夹内文件

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
	FRONT_COLOR=RED;//设置字体为红色
	LCD_ShowFont16Char(80, 0, "普中科技");
    LCD_ShowFont12Char(90, 21, "PRECHIN");
    LCD_ShowFont12Char(60, 42, "www.prechin.com");
    LCD_ShowFont12Char(76, 63, "MP3播放器实验");
    LCD_ShowFont12Char(0, 84, "音量设置为：230");
    LCD_ShowFont12Char(0, 105, "正在播放的歌曲为：");
    LCD_ShowFont12Char(0, 167, "KEY_UP：音量+");
    LCD_ShowFont12Char(0, 188, "KEY_WOWN：音量-");
    LCD_ShowFont12Char(0, 209, "KEY_RIGHT：下一曲");
}

FileNameTypeDef filename[30];

int main()
{
	FATFS fs;
	uint32_t sdCapacity, free;
    uint8_t dat[9] = {"0:/MUSIC"}, i=0, j=0, k=0;  //要显示的图片的文件地址
    uint8_t misicFile[30];
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	LED_Init();
	USART1_Init(9600);
	TFTLCD_Init();			//LCD初始化
	KEY_Init();
	EN25QXX_Init();				//初始化EN25Q128	  
	MP3_Init();
	
	my_mem_init(SRAMIN);		//初始化内部内存池
	
	FRONT_COLOR=RED;//设置字体为红色 
	while(SD_Init()!=0)
	{	
		LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,16,"SD Card Error!");
	}
	FATFS_Init();							//为fatfs相关变量申请内存				 
  	f_mount(&fs,"0:",1); 					//挂载SD卡 
	
	while(FATFS_GetFree("0:", &sdCapacity, &free) != 0)
    {
        LCD_ShowFont12Char(0, 21, "FATfs error!");   
    }
	
	/* 设置音量 */
    MP3_Volume = 230;
    MP3_AudioSetting(MP3_Volume);
    /* 初始化显示 */
	GUI_DisplayInit();
	
	/*  扫描文件地址里面所有的文件 */
    FATFS_ScanFiles(dat, filename);
	printf("%s%s\r\n",filename[0].name,filename[0].type);
	printf("%s%s\r\n",filename[1].name,filename[1].type);
	while(1)
	{	
		
		/* 判断是否是PM3图片文件 */
		if((filename[i].type[1] == 'm') && (filename[i].type[2] == 'p') &&
           (filename[i].type[3] == '3')) 
		{
			/* 处理文件路径,先添加文件路径 */
			k = 0;
			while(*(dat + k) != '\0')
			{
				*(misicFile + k) = *(dat + k);
				k++;
			}
			
            /* 路径之后加上一斜杠 */
			*(misicFile + k) = '/';
			k++;

            /* 添加文件名字名字 */
			j = 0;
			while(filename[i].name[j] != '\0')
			{
				*(misicFile + k) = filename[i].name[j];
				k++;
				j++;	
			}

            /* 添加文件后缀 */
			j = 0;
			while(filename[i].type[j] != '\0')
			{
				*(misicFile + k) = filename[i].type[j];
				k++;
				j++;	
			}

            /* 文件最后添加一个结束符号 */
            *(misicFile + k) = '\0';

			/* 显示播放的歌曲并播放歌曲 */
			LCD_Fill(0, 126, tftlcd_data.height, 165, WHITE); //清除显示位置
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
