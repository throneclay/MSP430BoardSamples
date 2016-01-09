#include <msp430x14x.h>
#include "sys.h"

/*******************************************
函数名称：Clock_Init
功    能：初始化时钟
参    数：无
返回值  ：无
********************************************/
void Clock_Init()
{
  uchar i;

  BCSCTL1 &= ~XT2OFF;                 //打开XT2振荡器  
  do
  {
    IFG1&=~OFIFG;                   //清楚振荡器错误标志
    for (i = 0xFF; i > 0; i--);     //等待8MHz晶体起振
  }
  while((IFG1&OFIFG)!=0);           //如果标志位1，则继续循环等待
  IFG1&=~OFIFG; 
  BCSCTL2 |= SELM1+SELS;              //MCLK为8MHZ，SMCLK为8MHZ
}

