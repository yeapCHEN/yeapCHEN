#ifndef _sta_tcpclent_test_H
#define _sta_tcpclent_test_H


#include "system.h"


#define User_ESP8266_BulitApSsid	  "PRECHIN"	  //Ҫ�������ȵ������
#define User_ESP8266_BulitApEcn	  	  OPEN            //Ҫ�������ȵ�ļ��ܷ�ʽ
#define User_ESP8266_BulitApPwd  	  "prechin"      //Ҫ�������ȵ����Կ


#define User_ESP8266_TCPServer_IP	  "192.168.1.119"	  //������������IP��ַ
#define User_ESP8266_TCPServer_PORT	  "8080"	  //�����������Ķ˿�


#define User_ESP8266_TCPServer_OverTime	  "1800"	  //��������ʱʱ�䣨��λ���룩


void ESP8266_STA_TCPClient_Test(void);
void TIM4_Init(u16 per,u16 psc);



#endif
