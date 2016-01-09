#ifndef __BUTTON_H__
#define __BUTTON_H__

#define KEY_IN (P1IN & 0x0f)   //P1…Ë÷√0x0f
#define KEY1 1
#define KEY2 2
#define KEY3 3
#define KEY4 4

void button_init();
uchar button_scan();

#endif