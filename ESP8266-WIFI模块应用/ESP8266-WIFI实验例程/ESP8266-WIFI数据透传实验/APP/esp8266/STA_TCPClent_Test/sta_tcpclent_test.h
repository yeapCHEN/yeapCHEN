#ifndef _sta_tcpclent_test_H
#define _sta_tcpclent_test_H


#include "system.h"


#define User_ESP8266_SSID	  "PUZHONG"	      //Ҫ���ӵ��ȵ������
#define User_ESP8266_PWD	  "puzhong168"	  //Ҫ���ӵ��ȵ������

#define User_ESP8266_TCPServer_IP	  "192.168.191.1"	  //Ҫ���ӵķ�������IP
#define User_ESP8266_TCPServer_PORT	  "8080"	  //Ҫ���ӵķ������Ķ˿�


extern volatile uint8_t TcpClosedFlag;  //����һ��ȫ�ֱ���


void ESP8266_STA_TCPClient_Test(void);



#endif
