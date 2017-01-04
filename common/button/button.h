#ifndef __BUTTON_H__
#define __BUTTON_H__

#define KEY_IN (P1IN & 0x0f)   //P1…Ë÷√0x0f
#define KEY1 1
#define KEY2 2
#define KEY3 3
#define KEY4 4

extern uchar KEY_Value_int;

void button_init();
void button_init_int();
uchar button_scan();

#endif