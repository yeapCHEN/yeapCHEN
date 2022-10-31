#ifndef _tim_H
#define _tim_H
#include "sys.h"
#include "delay.h"

extern u32 count;
extern u32 frequency;

void time_init(void);
void time_enable(void);
void time1_init(void);



#endif
