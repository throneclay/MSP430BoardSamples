#include "../sys/sys.h"
#include "led.h"

uchar LED_CODE1[]={0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
uchar LED_CODE2[]={0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x81};
uchar LED_CODE3[]={0x07, 0x0E, 0x1C, 0x38, 0x70, 0xE0, 0xC1, 0x83};
uchar LED_CODE4[]={0x0F, 0x1E, 0x3C, 0x78, 0xF0, 0xE1, 0xC3, 0x87};

void led_init()
{
  P4DIR = 0xFF;   //设置P4为输出，74HC573数据口 
  P2DIR |= BIT5;  //P2.5设置为输出，P2与第5位（与1相与等于1，其他位不变）
  P3DIR |= BIT6;  //P3.6设置为输出，P3与第6位（与1相与等于1，其他位不变）
  
  P2OUT |= BIT5;  //P2.5 = 1
  P3OUT |= BIT6;  //P3.6 = 1
  P4OUT = 0x00;
}

void led1_on(int led)
{
  P4OUT |=LED_CODE1[led];
}
void led1_off(int led)
{
  P4OUT &=~LED_CODE1[led];
}