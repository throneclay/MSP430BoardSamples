
#include "sys.h"

void Clock_Init()
{
  uchar i;

  BCSCTL1 &= ~XT2OFF;                 //��XT2����  
  do
  {
    IFG1&=~OFIFG;                   //������������־
    for (i = 0xFF; i > 0; i--);     //�ȴ�8MHz��������
  }
  while((IFG1&OFIFG)!=0);           //�����־λ1�������ѭ���ȴ�
  IFG1&=~OFIFG; 
  BCSCTL2 |= SELM1+SELS;              //MCLKΪ8MHZ��SMCLKΪ8MHZ
}

