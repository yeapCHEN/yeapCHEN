#ifndef _rtc_task_H
#define _rtc_task_H

#include "system.h"

void draw_circle(void);	 //��Բ
void draw_dotline(void);  //�����
void draw_hand(int hhour,int mmin,int ssec);  //��ָ��
void draw_hand_clear(int hhour,int mmin,int ssec);  //��ָ��
void RTC_task(void *p_arg);


#endif
