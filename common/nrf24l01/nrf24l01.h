#ifndef __NRF24L01_H__
#define __NRF24L01_H__
#include "../sys/sys.h"

// define the port and pins

#define NRF_CE   BIT0
#define NRF_CSN  BIT7
#define NRF_SCK  BIT3
#define NRF_MOSI BIT1
#define NRF_MISO BIT2
#define NRF_IRQ  BIT6

#define NRF_CE_IN    P3DIR &= ~NRF_CE
#define NRF_CE_OUT   P3DIR |=  NRF_CE
#define NRF_CE0      P3OUT &= ~NRF_CE
#define NRF_CE1      P3OUT |=  NRF_CE
#define NRF_CSN_IN   P2DIR &= ~NRF_CSN
#define NRF_CSN_OUT  P2DIR |=  NRF_CSN
#define NRF_CSN0     P2OUT &= ~NRF_CSN
#define NRF_CSN1     P2OUT |=  NRF_CSN
#define NRF_SCK_IN   P3DIR &= ~NRF_SCK
#define NRF_SCK_OUT  P3DIR |=  NRF_SCK
#define NRF_SCK0     P3OUT &= ~NRF_SCK
#define NRF_SCK1     P3OUT |=  NRF_SCK
#define NRF_MOSI_IN  P3DIR &= ~NRF_MOSI
#define NRF_MOSI_OUT P3DIR |=  NRF_MOSI
#define NRF_MOSI0    P3OUT &= ~NRF_MOSI
#define NRF_MOSI1    P3OUT |=  NRF_MOSI
//#define NRF_MISO_IN  P3DIR &= ~NRF_MISO
//#define NRF_MISO_OUT P3DIR |=  NRF_MISO
//#define NRF_MISO0    P3OUT &= ~NRF_MISO
//#define NRF_MISO1    P3OUT |=  NRF_MISO
//#define NRF_IRQ_IN   P2DIR &= ~NRF_IRQ
//#define NRF_IRQ_OUT  P2DIR |=  NRF_IRQ
//#define NRF_IRQ0     P2OUT &= ~NRF_IRQ
//#define NRF_IRQ1     P2OUT |=  NRF_IRQ

#define NRF_MISO_READ P3IN & NRF_MISO
#define NRF_IRQ_READ P2IN & NRF_IRQ

#define TX_ADR_WIDTH    5   //5字节地址宽度
#define RX_ADR_WIDTH    5   //5字节地址宽度
#define TX_PLOAD_WIDTH  32  //32字节有效数据宽度
#define RX_PLOAD_WIDTH  32  //32字节有效数据宽度

// SPI(nRF24L01) commands
#define READ_REG        0x00  // Define read command to register
#define WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP             0xFF  // Define No Operation, might be used to read status register

// SPI(nRF24L01) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address

//   STATUS寄存器bit位定义    
#define MAX_TX  	0x10  //达到最大发送次数中断
#define TX_OK   	0x20  //TX发送完成中断
#define RX_OK   	0x40  //接收到数据中断

// ....我的模式定义
#define MODERX          0xf1
#define MODETX          0xf2
#define STANDBYII       0xf4
#define POWEROFF        0xf5

extern uchar rece_buf[];

void nrf24l01_init_pins();
uchar NRF24L01_RxPacket(uchar *rxbuf);
uchar NRF24L01_TxPacket(uchar *txbuf);
void NRF24L01_ConfigReg();
void NRF24L01_ConfigMode(uchar mode);
void NRF24L01_init(uchar interrupt,uchar check);
uchar NRF24L01_Check(void);
void nrf24l01_int_en();
void nrf24l01_int_dis();

#endif
