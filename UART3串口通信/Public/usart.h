#ifndef __usart_H
#define __usart_H

#include "system.h" 
#include "stdio.h" 

//#define USART1_PRINTF
#define USART3_PRINTF

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

void USART1_Init(u32 bound);
void USART3_Init(u32 bound);


#endif


