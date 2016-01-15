#include "../common/sys/sys.h"
#include <msp430x14x.h>	

#define uchar unsigned char
#define uint  unsigned int
#include "../common/led/led.h"

#define HIGH P2OUT|=BIT1;
#define LOW P2OUT&=~BIT1;

char temph,templ,humdh,humdl,check,cal;

void init_clk()
{
    int i;
    WDTCTL=WDTPW+WDTHOLD;
    BCSCTL1&=~XT2OFF;
    do
    {
      IFG1 &=~OFIFG;
      for(i=0xff;i>0;i--);
    }
    while(IFG1&OFIFG); 
    BCSCTL2|= SELM_2+SELS;//SCLK，MCLK都选择为外部时钟     
 
}

char receive(void)               //接受函数
{
  char temp,cnt=0;                      //临时变量用于存储接受数据
  while(cnt<8)
  {
      while((P2IN&BIT1)==0);              //等待50us的低电平结束
      Delay_us(35);  
      if(P2IN&BIT1)                       //长于30us定义为1
      {
        temp++;                       
        temp<<=1;
        while(P2IN&BIT1);             //结束高电平，即下一位数据的到来
      }
      else
      {
        temp<<=1;
      }
      if(cnt!=7) 
         while(!(P2IN&BIT1));              //最后一次给函数返回留下时间
      cnt++;
  }
  return temp;
}
void work_data(void)
{
  P2DIR |= BIT1;    //设置P2.1为输出状态
  HIGH;
  _NOP();_NOP();_NOP();
  LOW;
  Delay_us(18000);    //开始信号
  HIGH;
  Delay_us(30);
  P2DIR&=~BIT1;    //设置P2.1为输入状态,检测传感器响应
  Delay_us(20);     //20US后  P2IN  是否为低电平，不为低电平 说明复位失败，重新开始
  while((P2IN&BIT1)!=0)//如果没有检测到响应信号 继续发送开始信号
  {
      P2DIR |= BIT1;    //设置P2.1为输出状态
      HIGH;
      _NOP();_NOP();_NOP();
      LOW;
      Delay_us(18000);    //开始信号
      HIGH;
      Delay_us(30);
      P2DIR&=~BIT1;    //设置P2.1为输入状态,检测传感器响应
      Delay_us(20); 
  }
  while((P2IN&BIT1)==0);//等待拉高，准备输出数据
  while(P2IN&BIT1);  //等待低电平，输出数据
                     //下面开始接受数据  
  humdh=receive();
  humdl=receive();
  temph=receive();
  templ=receive();
  check=receive();
  cal=humdh+humdl+temph+templ;
  
}
void main()
{	     
    init_clk();
    led_init();
    Delay_ms(2000);          //越过传感器不稳定状态  

    while(1)
    {
      led_out(0x55);
      Delay_ms(1000);
      led_out(0x00);
      work_data();
      led_out(humdh);
      Delay_ms(1000);//等待下次采样
      led_out(humdl);
      Delay_ms(1000);
      led_out(temph);
      Delay_ms(1000);
      led_out(templ);
      Delay_ms(1000);
      
    }
}





