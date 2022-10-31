/* 
  	ENC28J60��̫��ģ��������STM32��������߹ܽ����£�
	VCC ���� +3.3V
	GND ���� GND
	RST ���� PB9
	CS  ���� PB12 
	SCK ���� PB13
	SI  ���� PB15
	SO  ���� PB14
	INT ���� PB8

	�Ӻ��ߺ����س������ģ�����Ӻþͻ��ڳ�ʼ���ɹ���D1 D2������������������IE�������
	ʹ����������������ܻ���ִ򲻿�������
	����IP��ַ��192.168.1.16�Ϳ��Է��ʵ�data_index_html�ڵ�������վ������ͨ����ҳ
	�ϵİ�ť���ƿ������ϵ�ָʾ�Ƶȡ�

*/

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"

#include "rtc.h" 
#include "adc.h"
#include "adc_temp.h"
#include "enc28j60.h"
#include "uip.h"
#include "uip_arp.h"
#include "tapdev.h"
#include "timer.h"				   
#include "math.h" 	
#include "string.h"


void uip_polling(void);	 												
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])	 		  


//����->�ַ���ת������
//��num����(λ��Ϊlen)תΪ�ַ���,�����buf����
//num:����,����
//buf:�ַ�������
//len:����
void num2str(u16 num,u8 *buf,u8 len)
{
	u8 i;
	for(i=0;i<len;i++)
	{
		buf[i]=(num/LCD_Pow(10,len-i-1))%10+'0';
	}
}
//��ȡSTM32�ڲ��¶ȴ��������¶�
//temp:����¶��ַ������׵�ַ.��"28.3";
//temp,���ٵ���5���ֽڵĿռ�!
void get_temperature(u8 *temp)
{			  
	u16 t;
	float temperate;		   
	temperate=Get_ADC_Temp_Value(ADC_Channel_16,10);			 
	temperate=temperate*(3.3/4096);			    											    
	temperate=(1.43-temperate)/0.0043+25;	//�������ǰ�¶�ֵ
	t=temperate*10;//�õ��¶�
	num2str(t/10,temp,2);							   
	temp[2]='.';temp[3]=t%10+'0';temp[4]=0;	//�����ӽ�����
}
//��ȡRTCʱ��
//time:���ʱ���ַ���,����:"2012-09-27 12:33"
//time,���ٵ���17���ֽڵĿռ�!
void get_time(u8 *time)
{	
	RTC_Get();
	time[4]='-';time[7]='-';time[10]=' ';
	time[13]=':';time[16]=0;			//�����ӽ�����
	num2str(calendar.w_year,time,4);	//���->�ַ���
	num2str(calendar.w_month,time+5,2); //�·�->�ַ���	 
	num2str(calendar.w_date,time+8,2); 	//����->�ַ���
	num2str(calendar.hour,time+11,2); 	//Сʱ->�ַ���
	num2str(calendar.min,time+14,2); 	//����->�ַ���								   	  									  
}

int main()
{
	uip_ipaddr_t ipaddr;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	LED_Init();
	USART1_Init(9600);
	TFTLCD_Init();			//LCD��ʼ��
	
	FRONT_COLOR=RED;
	LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,16,"ENC28J60 Ethernet Test!");
	LCD_ShowString(10,30,tftlcd_data.width,tftlcd_data.height,16,"www.prechin.com");
	
	RTC_Init();
	ADCx_Init();	
	ADC_Temp_Init();
	while(tapdev_init())	//��ʼ��ENC28J60����
	{								   
		LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,16,"ENC28J60 Init Error!");	 
		printf("ENC28J60 Init Error!\r\n");
	};
	led1=0;
	led2=0;
	printf("ENC28J60 Init OK!\r\n");
	LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,16,"                    ");
	uip_init();				//uIP��ʼ��
	  	   						  	 
	LCD_ShowString(10,80,tftlcd_data.width,tftlcd_data.height,16,"IP:192.168.1.16");
	LCD_ShowString(10,100,tftlcd_data.width,tftlcd_data.height,16,"MASK:255.255.255.0");
	LCD_ShowString(10,120,tftlcd_data.width,tftlcd_data.height,16,"GATEWAY:192.168.1.1");
	
	uip_ipaddr(ipaddr, 192,168,1,16);	//���ñ�������IP��ַ
	uip_sethostaddr(ipaddr);					    
	uip_ipaddr(ipaddr, 192,168,1,1); 	//��������IP��ַ(��ʵ������·������IP��ַ)
	uip_setdraddr(ipaddr);						 
	uip_ipaddr(ipaddr, 255,255,255,0);	//������������
	uip_setnetmask(ipaddr);

	uip_listen(HTONS(1200));			//����1200�˿�,����TCP Server
	uip_listen(HTONS(80));				//����80�˿�,����Web Server
  	tcp_client_reconnect();	   		    //�������ӵ�TCP Server��,����TCP Client
	
	while (1)
	{
		uip_polling();	//����uip�¼���������뵽�û������ѭ������  
		delay_ms(1);   
	}  
}

//uip�¼�������
//���뽫�ú��������û���ѭ��,ѭ������.

void uip_polling(void)
{
	u8 i;
	static struct timer periodic_timer, arp_timer;
	static u8 timer_ok=0;	 
	if(timer_ok==0)//����ʼ��һ��
	{
		timer_ok = 1;
		timer_set(&periodic_timer,CLOCK_SECOND/2);  //����1��0.5��Ķ�ʱ�� 
		timer_set(&arp_timer,CLOCK_SECOND*10);	   	//����1��10��Ķ�ʱ�� 
	}				 
	uip_len=tapdev_read();	//�������豸��ȡһ��IP��,�õ����ݳ���.uip_len��uip.c�ж���
	if(uip_len>0) 			//������
	{   
		//����IP���ݰ�(ֻ��У��ͨ����IP���Żᱻ����) 
		if(BUF->type == htons(UIP_ETHTYPE_IP))//�Ƿ���IP��? 
		{
			uip_arp_ipin();	//ȥ����̫��ͷ�ṹ������ARP��
			uip_input();   	//IP������
			//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ��� uip_len > 0
			//��Ҫ���͵�������uip_buf, ������uip_len  (����2��ȫ�ֱ���)		    
			if(uip_len>0)//��Ҫ��Ӧ����
			{
				uip_arp_out();//����̫��ͷ�ṹ������������ʱ����Ҫ����ARP����
				tapdev_send();//�������ݵ���̫��
			}
		}else if (BUF->type==htons(UIP_ETHTYPE_ARP))//����arp����,�Ƿ���ARP�����?
		{
			uip_arp_arpin();
 			//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ���uip_len>0
			//��Ҫ���͵�������uip_buf, ������uip_len(����2��ȫ�ֱ���)
 			if(uip_len>0)tapdev_send();//��Ҫ��������,��ͨ��tapdev_send����	 
		}
	}else if(timer_expired(&periodic_timer))	//0.5�붨ʱ����ʱ
	{
		timer_reset(&periodic_timer);		//��λ0.5�붨ʱ�� 
		//��������ÿ��TCP����, UIP_CONNSȱʡ��40��  
		for(i=0;i<UIP_CONNS;i++)
		{
			uip_periodic(i);	//����TCPͨ���¼�  
	 		//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ���uip_len>0
			//��Ҫ���͵�������uip_buf, ������uip_len (����2��ȫ�ֱ���)
	 		if(uip_len>0)
			{
				uip_arp_out();//����̫��ͷ�ṹ������������ʱ����Ҫ����ARP����
				tapdev_send();//�������ݵ���̫��
			}
		}
#if UIP_UDP	//UIP_UDP 
		//��������ÿ��UDP����, UIP_UDP_CONNSȱʡ��10��
		for(i=0;i<UIP_UDP_CONNS;i++)
		{
			uip_udp_periodic(i);	//����UDPͨ���¼�
	 		//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ���uip_len>0
			//��Ҫ���͵�������uip_buf, ������uip_len (����2��ȫ�ֱ���)
			if(uip_len > 0)
			{
				uip_arp_out();//����̫��ͷ�ṹ������������ʱ����Ҫ����ARP����
				tapdev_send();//�������ݵ���̫��
			}
		}
#endif 
		//ÿ��10�����1��ARP��ʱ������ ���ڶ���ARP����,ARP��10�����һ�Σ��ɵ���Ŀ�ᱻ����
		if(timer_expired(&arp_timer))
		{
			timer_reset(&arp_timer);
			uip_arp_timer();
		}
	}
}
