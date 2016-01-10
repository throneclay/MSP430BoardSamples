#include "../sys/sys.h"
#include "serial.h"

uchar index=0;
uchar rx_buff[RXBUFFLEN]={0};

void serial1_init_x1()
{
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD������0
  ME1 |= URXE0 + UTXE0;                     // Enable USART0 T/RXD��ʹ�ܴ���0
  UCTL0 |= CHAR;                            // 8-bit character��8λ�ַ���ʽ
  UTCTL0 |= SSEL0;                          // UCLK = ACLK��ʱ��ѡ����ʱ���ź�
  UBR00 = 0x03;                             // 32k/9600 = 3.41
  UBR10 = 0x00;                             //
  UMCTL0 = 0x4A;                            // Modulation  01001010����������
  UCTL0 &= ~SWRST;                          // Initialize USART state 
  
  //IE1 |= URXIE0;                            // Enable USART0 RX interrupt
}

void serial1_init_x2()
{
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD   
  UCTL0 |= CHAR;                            // ѡ��8λ�ַ�
  ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
  UTCTL0 |= SSEL1;                          // UCLK= ACLK
  UBR00 = 0x41;                             // 8MHz 9600
  UBR10 = 0x03;                             // 8MHz 9600
  UMCTL0 = 0x00;                            // 8MHz 9600 modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  
  //IE1 |= URXIE0;                            // Enable USART0 RX interrupt
}

void serial2_init_x1()
{
  P3SEL |= 0xC0;                            // P3.6,7 = USART1 TXD/RXD
  ME2 |= UTXE1 + URXE1;                     // Enable USART1 TXD/RXD
  UCTL1 |= CHAR;                            // 8-bit character
  UTCTL1 |= SSEL0;                          // UCLK = ACLK
  UBR01 = 0x03;                             // 32k/9600 - 3.41
  UBR11 = 0x00;                             //
  UMCTL1 = 0x4A;                            // Modulation
  UCTL1 &= ~SWRST;                          // Initialize USART state machine
  //IE2 |= URXIE1;                            // Enable USART1 RX interrupt
}

void serial2_init_x2()
{
    P3SEL |= 0xC0;                            // P3.6,7 = USART1 option select
    ME2 |= UTXE1 + URXE1;                     // Enable USART1 TXD/RXD
    UCTL1 |= CHAR;                            // 8-bit character
    UTCTL1 |= SSEL1;                          // UCLK = SMCLK
    UBR01 = 0x45;                             // 8Mhz/115200 - 69.44
    UBR11 = 0x00;                             //
    UMCTL1 = 0x2C;                            // modulation
    UCTL1 &= ~SWRST;                          // Initialize USART state machine
    //IE2 |= URXIE1;                            // Enable USART1 RX interrupt
}


void serial_init(uchar en_rx,uchar x)
{//en_rx = enable rx function
   if(x==1){
     serial1_init_x1();
   }
   else{
     serial1_init_x2();
   }
   
   if(en_rx==1){
    IE1 |= URXIE0;                            // Enable USART0 RX interrupt
   }
}
// Send a string
void Send_string(uchar *ptr)
{
  while(*ptr != '\0')
  {
    while (!(IFG1 & UTXIFG0));             // �ȴ����ͼĴ�������
    TXBUF0 = *ptr++;                       // ��������
  }
  while (!(IFG1 & UTXIFG0));
}

// Send a char
void Send_char(uchar t)
{
  TXBUF0 = t;                       // ��������
  while (!(IFG1 & UTXIFG0));
}

uchar Read_string()
{
  uchar temp=index;
  if(temp!=0)
  {
    rx_buff[temp]='\0';
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
  //main       -->       _BIS_SR(LPM3_bits + GIE);   
  
  _BIC_SR_IRQ(LPM3_bits);                   // Clear LPM3 bits from 0(SR)
  rx_buff[index%RXBUFFLEN] = RXBUF0;                     // RXBUF0 to TXBUF0
  index++;
  while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
  TXBUF0 = index;  
}
