/* ������ʹ�õ���RFID-RC522��Ƶģ����Ƶ�һ���Ž�ϵͳ������Ӧ���ŵ���Ƶģ�������ڻ��Ӧ��
	������������кźͳ������һ�¾ͻ���Ϊ����ȷ������D2ָʾ������LCD����ʾ������5�����Ժ�
	�Զ�������D2ָʾ���𡣵�������ʱ�򲻻���ʾ��D2Ҳ���������������к���Ψһ�ġ�
	
	�ܽŽ���ͼ��
	RST---PF4
	MISO---PF3
	MOSI---PF2
	SCK---PF1
	NSS(SDA)--PF0
	
	*/

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"
#include "RC522.h"
#include "time.h"


unsigned char data1[16] = {0x12,0x34,0x56,0x78,0xED,0xCB,0xA9,0x87,0x12,0x34,0x56,0x78,0x01,0xFE,0x01,0xFE};
//M1����ĳһ��дΪ���¸�ʽ����ÿ�ΪǮ�����ɽ��տۿ�ͳ�ֵ����
//4�ֽڽ����ֽ���ǰ����4�ֽڽ��ȡ����4�ֽڽ�1�ֽڿ��ַ��1�ֽڿ��ַȡ����1�ֽڿ��ַ��1�ֽڿ��ַȡ�� 
unsigned char data2[4]  = {0,0,0,0x01};
unsigned char DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 


unsigned char g_ucTempbuf[20];

int main()
{
	unsigned char status,i;
	unsigned int temp;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	LED_Init();
	USART1_Init(9600);
	TFTLCD_Init();			//LCD��ʼ��
	
	
	FRONT_COLOR=GREEN;
	LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,16,"PRECHIN STM32F1");
	LCD_ShowString(10,30,tftlcd_data.width,tftlcd_data.height,16,"www.prechin.net");
	LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,16,"RFID-RC522 Test");
	FRONT_COLOR=RED;
	LCD_ShowString(10,110,tftlcd_data.width,tftlcd_data.height,16,"Close Door...");
	
	RC522_Init();
	PcdReset();
 	PcdAntennaOff(); 
	delay_ms(10);
 	PcdAntennaOn();
	delay_ms(10);
	
	TIM4_Init(1000,7199);
	printf("Start \r\n");
	
	while ( 1 )
     {   
		 status = PcdRequest(PICC_REQALL, g_ucTempbuf);//Ѱ��
         if (status != MI_OK)
         {    
		     PcdReset();
		     PcdAntennaOff(); 
		     PcdAntennaOn(); 
			 continue;
         }		     
		printf("��������:");
	    for(i=0;i<2;i++)
		{
			temp=g_ucTempbuf[i];
			printf("%X",temp);
			
		}	
         status = PcdAnticoll(g_ucTempbuf);//����ײ
         if(status != MI_OK)
         {    continue;    }
		   
		////////����Ϊ�����ն˴�ӡ��������////////////////////////
	
		printf("�����кţ�");	//�����ն���ʾ,
		for(i=0;i<4;i++)
		{
			temp=g_ucTempbuf[i];
			printf("%X",temp);
			
		}

		if(g_ucTempbuf[0]==0xd4&&g_ucTempbuf[1]==0xd5&&g_ucTempbuf[2]==0x34&&g_ucTempbuf[3]==0x00)
		{
			led2=0;
			FRONT_COLOR=RED;
			LCD_ShowString(10,110,tftlcd_data.width,tftlcd_data.height,16,"Open Door...  ");//����
		}
		else
		{	
			led2=1;	
			FRONT_COLOR=RED;
			LCD_ShowString(10,110,tftlcd_data.width,tftlcd_data.height,16,"               ");
		}

		///////////////////////////////////////////////////////////

	     status = PcdSelect(g_ucTempbuf);//ѡ����Ƭ
	     if (status != MI_OK)
	     {    continue;    }
	     
	     status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf);//��֤��Ƭ����
	     if (status != MI_OK)
	     {    continue;    }
	     
	     status = PcdWrite(1, data1);//д��
         if (status != MI_OK)
         {    continue;    }

		while(1)
		{
         	status = PcdRequest(PICC_REQALL, g_ucTempbuf);//Ѱ��
         	if (status != MI_OK)
         	{   
 		 	
			     PcdReset();
			     PcdAntennaOff(); 
			     PcdAntennaOn(); 
			  	continue;
         	}
			 status = PcdAnticoll(g_ucTempbuf);//����ײ
	         if (status != MI_OK)
	         {    continue;    }

			status = PcdSelect(g_ucTempbuf);//ѡ����Ƭ
	         if (status != MI_OK)
	         {    continue;    }
	         
	         status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf);//��֤��Ƭ����
	         if (status != MI_OK)
	         {    continue;    }


	         status = PcdValue(PICC_DECREMENT,1,data2);//�ۿ�
	         if (status != MI_OK)
	         {    continue;    }
			 
	         status = PcdBakValue(1, 2);//�鱸��
	         if (status != MI_OK)
	         {    continue;    }
	         
	         status = PcdRead(2, g_ucTempbuf);//����
	         if (status != MI_OK)
	         {    continue;    }
        	printf("�����飺");	//�����ն���ʾ,
         	for(i=0;i<16;i++)
			{
				temp=g_ucTempbuf[i];
				printf("%X",temp);
				
			}

			printf("\n");				 		         
			PcdHalt();
		}	
    }
}
