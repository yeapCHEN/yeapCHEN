#include "sta_tcpclent_test.h"
#include "SysTick.h"
#include "usart.h"
#include "esp8266_drive.h"
#include "ds18b20.h"
#include "led.h"
#include "beep.h"
#include <string.h> 



uint8_t ucId, ucLen;
char str[100]={0};
char cCh;
char * pCh, * pCh1;
float temp=0.0;

void ESP8266_STA_TCPClient_Test(void)
{
//	uint8_t ucId, ucLen;
//	char str[100]={0};
//	char cCh;
//	char * pCh, * pCh1;
//	float temp=0.0;
	
	printf ( "\r\n��������ESP8266�����ĵȴ�...\r\n" );

	ESP8266_CH_PD_Pin_SetH;

	ESP8266_AT_Test();
	
	ESP8266_Net_Mode_Choose(AP);
	
	while (!ESP8266_CIPAP(User_ESP8266_TCPServer_IP ));
	
	while (!ESP8266_BuildAP(User_ESP8266_BulitApSsid, User_ESP8266_BulitApPwd, User_ESP8266_BulitApEcn));	
	
	ESP8266_Enable_MultipleId ( ENABLE );
	
	while(!ESP8266_StartOrShutServer(ENABLE, User_ESP8266_TCPServer_PORT, User_ESP8266_TCPServer_OverTime ));
	
	ESP8266_Inquire_ApIp ( str, 20 );
	
	printf ( "\r\n����ESP8266 OK��\r\n" );
	printf ( "\r\n��ģ��WIFIΪ%s�����뿪��\r\nAP IP Ϊ��%s�������Ķ˿�Ϊ��%s\r\n�ֻ������������Ӹ� IP �Ͷ˿ڣ���������5���ͻ���\r\n",
           User_ESP8266_BulitApSsid, str, User_ESP8266_TCPServer_PORT );
	
	ESP8266_Fram_Record_Struct .InfBit .FramLength = 0;
	ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag = 0;
	
	while ( 1 )
	{		
		if(ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag )
		{
			USART_ITConfig ( USART2, USART_IT_RXNE, DISABLE ); //���ô��ڽ����ж�
			ESP8266_Fram_Record_Struct .Data_RX_BUF [ ESP8266_Fram_Record_Struct .InfBit .FramLength ]  = '\0';
			
			printf ( "\r\n%s\r\n", ESP8266_Fram_Record_Struct .Data_RX_BUF );//
			if ( ( pCh = strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "+IPD,0,6:LED_" ) ) != 0 )   
			{
				cCh = * ( pCh + 13 );
				
				switch ( cCh )
				{
					case '1':                //LED1����
						cCh = * ( pCh + 14 );
					  switch ( cCh )
					  {
							case '0':     //�ر�LED1
								led1=1;
								break;
							case '1':     //��LED1
								led1=0;
								break;
							default :
								break;
						}
						break;
						
					case '2':                   //LED2����
						cCh = * ( pCh + 14 );
					  switch ( cCh )
					  {
							case '0':
								led2=1;
								break;
							case '1':
								led2=0;
								break;
							default :
								break;
						}
						break;
					
					default :
						break;	
				}
				
			}
			
			else if ( ( pCh = strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "+IPD,0,8:BUZZER_" ) ) != 0 ) 
			{
				cCh = * ( pCh + 16 );
				
				switch ( cCh )
				{
					case '0':
						beep=1;
						break;
					case '1':
						beep=0;
						break;
					default:
						break;
				}
				
			}
				
//			temp=DS18B20_GetTemperture();
//			if ( ( pCh = strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "+IPD,0,6:Temp" ) ) != 0 ) 
//			{
//				sprintf ( str, "%0.2f",temp );
//				ucId = * ( pCh + strlen ( "+IPD," ) ) - '0';
//				ESP8266_SendString ( DISABLE, str, strlen ( str ), ( ENUM_ID_NO_TypeDef ) ucId );
//				printf("\r\nstr=%s\r\n",str);
//			}
			
			ESP8266_Fram_Record_Struct .InfBit .FramLength = 0;
			ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag = 0;	
			
			USART_ITConfig ( USART2, USART_IT_RXNE, ENABLE ); //ʹ�ܴ��ڽ����ж�
			
		}
	}
		
}


void TIM4_Init(u16 per,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//ʹ��TIM4ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period=per;   //�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //������ʱ���ж�
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//��ʱ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	
	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��	
}

void TIM4_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM4,TIM_IT_Update))
	{
		temp=DS18B20_GetTemperture();
		if ( ( pCh = strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "+IPD,0,6:Temp" ) ) != 0 ) 
		{
			sprintf ( str, "%0.2f",temp );
			ucId = * ( pCh + strlen ( "+IPD," ) ) - '0';
			ESP8266_SendString ( DISABLE, str, strlen ( str ), ( ENUM_ID_NO_TypeDef ) ucId );
			printf("\r\nstr=%s\r\n",str);
		}
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	
}


