
#include "../sys/sys.h"
#include "SMG.h"

//数码管显示： 0 ~ F
uchar SMG_Code[16] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};


void SMG_init()
{
  P3DIR |= BIT6;		//74hc573控制位
  P4DIR = 0xff;			//P4设置输出
  P5DIR = 0x0f;			//P5第四位设置输出，高四位设置输入
  P3OUT |= BIT6;		//P3.6输出高电平，使74hc573工作
  P4OUT = 0x00;			//P4输出0
  P5OUT = 0x0f;			//P5输出00001111， 点亮数码管
}

void SMG_Display(uint number)
{
  unsigned char Ge, Shi ,Bai ,Qian;
  Ge = number % 10;
  Shi = (number / 10) % 10;
  Bai = (number / 100) % 10;
  Qian = number / 1000;
  
  P4OUT=0x00;
  //数码管个位
  P5OUT = 0x0e;
  P4OUT = SMG_Code[Qian];
  Delay_us(200);
  P5OUT = 0X0f;
  
  P4OUT=0x00;
  //数码管十位
  P5OUT = 0x0d;
  P4OUT = SMG_Code[Bai];
  Delay_us(200);
  P5OUT = 0x0f;
  
  P4OUT=0x00;
  //数码管百位
  P5OUT = 0x0b;
  P4OUT = SMG_Code[Shi];
  Delay_us(200);
  P5OUT = 0x0f;
  
  P4OUT=0x00;
  //数码管千位
  P5OUT = 0x07;
  P4OUT = SMG_Code[Ge];
  Delay_us(200);
  P5OUT = 0x0f;
}