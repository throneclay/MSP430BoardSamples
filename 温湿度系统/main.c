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
    BCSCTL2|= SELM_2+SELS;//SCLK��MCLK��ѡ��Ϊ�ⲿʱ��     
 
}

char receive(void)               //���ܺ���
{
  char temp,cnt=0;                      //��ʱ�������ڴ洢��������
  while(cnt<8)
  {
      while((P2IN&BIT1)==0);              //�ȴ�50us�ĵ͵�ƽ����
      Delay_us(35);  
      if(P2IN&BIT1)                       //����30us����Ϊ1
      {
        temp++;                       
        temp<<=1;
        while(P2IN&BIT1);             //�����ߵ�ƽ������һλ���ݵĵ���
      }
      else
      {
        temp<<=1;
      }
      if(cnt!=7) 
         while(!(P2IN&BIT1));              //���һ�θ�������������ʱ��
      cnt++;
  }
  return temp;
}
void work_data(void)
{
  P2DIR |= BIT1;    //����P2.1Ϊ���״̬
  HIGH;
  _NOP();_NOP();_NOP();
  LOW;
  Delay_us(18000);    //��ʼ�ź�
  HIGH;
  Delay_us(30);
  P2DIR&=~BIT1;    //����P2.1Ϊ����״̬,��⴫������Ӧ
  Delay_us(20);     //20US��  P2IN  �Ƿ�Ϊ�͵�ƽ����Ϊ�͵�ƽ ˵����λʧ�ܣ����¿�ʼ
  while((P2IN&BIT1)!=0)//���û�м�⵽��Ӧ�ź� �������Ϳ�ʼ�ź�
  {
      P2DIR |= BIT1;    //����P2.1Ϊ���״̬
      HIGH;
      _NOP();_NOP();_NOP();
      LOW;
      Delay_us(18000);    //��ʼ�ź�
      HIGH;
      Delay_us(30);
      P2DIR&=~BIT1;    //����P2.1Ϊ����״̬,��⴫������Ӧ
      Delay_us(20); 
  }
  while((P2IN&BIT1)==0);//�ȴ����ߣ�׼���������
  while(P2IN&BIT1);  //�ȴ��͵�ƽ���������
                     //���濪ʼ��������  
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
    Delay_ms(2000);          //Խ�����������ȶ�״̬  

    while(1)
    {
      led_out(0x55);
      Delay_ms(1000);
      led_out(0x00);
      work_data();
      led_out(humdh);
      Delay_ms(1000);//�ȴ��´β���
      led_out(humdl);
      Delay_ms(1000);
      led_out(temph);
      Delay_ms(1000);
      led_out(templ);
      Delay_ms(1000);
      
    }
}





