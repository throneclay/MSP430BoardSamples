#ifndef __SYS_H__
#define __SYS_H__

#include <msp430x14x.h>

#define uint unsigned int
#define uchar unsigned char
#define ulong unsigned long int


#define CPU_F ((double)8000000)           //ϵͳʱ��8Mhz

#define Delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) //��ʱ1us
#define Delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))    //��ʱ1ms

void Clock_Init();

#endif