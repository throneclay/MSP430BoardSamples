#ifndef __LED_H__
#define __LED_H__

extern uchar LED_CODE1[],LED_CODE2[],LED_CODE3[],LED_CODE4[];

void led_init();
void led1_on(int led);
void led1_off(int led);
void led_out(uchar code);
#endif