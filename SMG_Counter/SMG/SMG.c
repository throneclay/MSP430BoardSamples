
#include "../sys/sys.h"
#include "SMG.h"

//�������ʾ�� 0 ~ F
uchar SMG_Code[16] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};


void SMG_init()
{
  P3DIR |= BIT6;		//74hc573����λ
  P4DIR = 0xff;			//P4�������
  P5DIR = 0x0f;			//P5����λ�������������λ��������
  P3OUT |= BIT6;		//P3.6����ߵ�ƽ��ʹ74hc573����
  P4OUT = 0x00;			//P4���0
  P5OUT = 0x0f;			//P5���00001111�� ���������
}

void SMG_Display(uint number)
{
  unsigned char Ge, Shi ,Bai ,Qian;
  Ge = number % 10;
  Shi = (number / 10) % 10;
  Bai = (number / 100) % 10;
  Qian = number / 1000;
  
  P4OUT=0x00;
  //����ܸ�λ
  P5OUT = 0x0e;
  P4OUT = SMG_Code[Qian];
  Delay_us(200);
  P5OUT = 0X0f;
  
  P4OUT=0x00;
  //�����ʮλ
  P5OUT = 0x0d;
  P4OUT = SMG_Code[Bai];
  Delay_us(200);
  P5OUT = 0x0f;
  
  P4OUT=0x00;
  //����ܰ�λ
  P5OUT = 0x0b;
  P4OUT = SMG_Code[Shi];
  Delay_us(200);
  P5OUT = 0x0f;
  
  P4OUT=0x00;
  //�����ǧλ
  P5OUT = 0x07;
  P4OUT = SMG_Code[Ge];
  Delay_us(200);
  P5OUT = 0x0f;
}