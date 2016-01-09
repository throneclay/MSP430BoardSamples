#include "../sys/sys.h"
#include "serial.h"

uchar index=0;
uchar rx_buff[RXBUFFLEN]={0};

void serial_init_x1()
{
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD，串口0
  ME1 |= URXE0 + UTXE0;                     // Enable USART0 T/RXD，使能串口0
  UCTL0 |= CHAR;                            // 8-bit character，8位字符格式
  UTCTL0 |= SSEL0;                          // UCLK = ACLK，时钟选择辅助时钟信号
  UBR00 = 0x03;                             // 32k/9600 = 3.41
  UBR10 = 0x00;                             //
  UMCTL0 = 0x4A;                            // Modulation  01001010，调整因子
  UCTL0 &= ~SWRST;                          // Initialize USART state 
  
  IE1 |= URXIE0;                            // Enable USART0 RX interrupt
}

void serial_init_x2()
{
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD   
  UCTL0 |= CHAR;                            // 选择8位字符
  ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
  UTCTL0 |= SSEL1;                          // UCLK= ACLK
  UBR00 = 0x41;                             // 8MHz 9600
  UBR10 = 0x03;                             // 8MHz 9600
  UMCTL0 = 0x00;                            // 8MHz 9600 modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  
  IE1 |= URXIE0;                            // Enable USART0 RX interrupt
}

void serial_init(uchar x)
{
 // if(x==1)
 // {
 //   serial_init_x1();
 // }
 // else
 // {
 //   serial_init_x2();
 // }
   P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD，串口0
  ME1 |= URXE0 + UTXE0;                     // Enable USART0 T/RXD，使能串口0
  UCTL0 |= CHAR;                            // 8-bit character，8位字符格式
  UTCTL0 |= SSEL0;                          // UCLK = ACLK，时钟选择辅助时钟信号
  UBR00 = 0x03;                             // 32k/9600 = 3.41
  UBR10 = 0x00;                             //
  UMCTL0 = 0x4A;                            // Modulation  01001010，调整因子
  UCTL0 &= ~SWRST;                          // Initialize USART state 
  
  IE1 |= URXIE0;                            // Enable USART0 RX interrupt
}

void Send_string(uchar *ptr)
{
      while(*ptr != '\0')
      {
            while (!(IFG1 & UTXIFG0));             // 等待发送寄存器空闲
            TXBUF0 = *ptr++;                       // 发送数据
      }
      while (!(IFG1 & UTXIFG0));
}

void Send_char(uchar t)
{
  TXBUF0 = t;                       // 发送数据
  while (!(IFG1 & UTXIFG0));
}

uchar Read_string()
{
  uchar temp=index;
  if(temp!=0)
  {
    index=0;
    return temp;
  }
  else
  {
    return 0;
  }
}

#pragma vector=UART0RX_VECTOR
__interrupt void usart0_rx (void)
{
  _BIC_SR_IRQ(LPM3_bits);                   // Clear LPM3 bits from 0(SR)
  rx_buff[index%RXBUFFLEN] = RXBUF0;                     // RXBUF0 to TXBUF0
  index++;
  while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
  TXBUF0 = index;  
}
