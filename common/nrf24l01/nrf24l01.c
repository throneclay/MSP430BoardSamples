
#include "../sys/sys.h"
#include "nrf24l01.h"

uchar TX_ADDRESS[TX_ADR_WIDTH]={0x68,0x86,0x66,0x88,0x28}; //发送地址
uchar RX_ADDRESS[RX_ADR_WIDTH]={0x68,0x86,0x66,0x88,0x28}; //发送地址

uchar rece_buf[32];

void nrf24l01_init_pins()
{
  NRF_CSN_OUT;
  NRF_CE_OUT;
  NRF_SCK_OUT;
  NRF_MOSI_OUT;
  NRF_CE0;
  NRF_CSN1;
  NRF_SCK0;
}
//中断使能和中断停止
void nrf24l01_int_en()
{
  P2IFG = 0x00;
  P2IES = 0x40;
  P2IE = 0x40;
  _EINT();
}

void nrf24l01_int_dis()
{
  _DINT();
}

/////////////////////////读写操作/////////////////////////////////////
uchar SPI_RW(uchar byte)
{
  uchar bit_ctr;
  for(bit_ctr=0;bit_ctr<8;bit_ctr++) // 输出8位
  {
    if(byte & 0x80) NRF_MOSI1;                   
    else NRF_MOSI0;                   // output 'byte', MSB to MOSI
    
    byte=(byte<<1);	// shift next bit to MSB
    NRF_SCK1;
    
    if(NRF_MISO_READ) byte |= BIT0;       		        // capture current MISO bit
    else  byte &=~BIT0;
    
    NRF_SCK0;
  }
  NRF_MOSI0;  //PULL DOWN THE MOSI
  return byte;
}

/////////////////////写寄存器/////////////////////////////////////////////
uchar SPI_Write_Reg(uchar reg, uchar value)
{
  uchar status;
  
  NRF_CSN0;                            // CSN low, init SPI transaction
  status = SPI_RW(reg);                        // select register
  SPI_RW(value);                              // ..and write value to it..
  NRF_CSN1;                            // CSN high again
  return(status);                             // return nRF24L01 status byte
}

/////////////////////////读一个字节从24L01/////////////////////////////////////
uchar SPI_Read_Reg(uchar reg)
{
  uchar reg_val;
  
  NRF_CSN0;                          // CSN low, initialize SPI communication...
  SPI_RW(reg);                               // Select register to read from..
  reg_val = SPI_RW(0);                       // ..then read registervalue
  NRF_CSN1;                           // CSN high, terminate SPI communication
  
  return(reg_val);                            // return register value
}

/////////////////////////read RX payload, Rx/Tx address//////////////////////////
uchar SPI_Read_Buf(uchar reg, uchar *pBuf, uchar bytes)
{
  uchar status, byte_ctr;

  NRF_CSN0;                      		   // Set CSN low, init SPI tranaction
  status = SPI_RW(reg);       		           // Select register to write to and read status byte

  for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
    pBuf[byte_ctr] = SPI_RW(NOP);                         // Perform SPI_RW to read byte from nRF24L01 ...

  NRF_CSN1;                                    // Set CSN high again

  return(status);                                     // return nRF24L01 status byte
}

uchar SPI_Write_Buf(uchar reg, uchar *pBuf, uchar bytes)
{
  uchar status,byte_ctr;

  NRF_CSN0;                                   // Set CSN low, init SPI tranaction
  status = SPI_RW(reg);                               // Select register to write to and read status byte
  
  for(byte_ctr=0; byte_ctr<bytes; byte_ctr++)         // then write all byte in buffer(*pBuf)
    SPI_RW(*pBuf++);
  NRF_CSN1;                                     // Set CSN high again
  return(status);                                       // return nRF24L01 status byte
}

uchar NRF24L01_RxPacket(uchar *rxbuf)
{
  uchar stat = 0;
  stat = SPI_Read_Reg(STATUS);
  SPI_Write_Reg(WRITE_REG + STATUS, stat);
  if(stat&RX_OK)
  {
    NRF_CE0;
    SPI_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);
    SPI_Write_Reg(FLUSH_RX,0xff);
    NRF_CE1;
    Delay_us(130);  ///???
    return 1;
  }
  return 0;
}

uchar NRF24L01_TxPacket(uchar *txbuf)
{
  uchar stat = 0;
  NRF_CE0;
  SPI_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);
  NRF_CE1;
  while(NRF_IRQ_READ);
  stat = SPI_Read_Reg(STATUS);
  SPI_Write_Reg(WRITE_REG+STATUS,stat);
  if(stat&MAX_TX)//达到最大重发次数
  {
    SPI_Write_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器 
    return MAX_TX; 
  }
  if(stat&TX_OK)//发送完成
  {
    return TX_OK;
  }
  return 0xff;//发送失败
}

void NRF24L01_ConfigReg()
{// 24l01工作寄存器的配置
  NRF_CE0;
  SPI_Write_Reg(WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度
  SPI_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器    
  SPI_Write_Buf(WRITE_REG+TX_ADDR,(uchar*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
  SPI_Write_Buf(WRITE_REG+RX_ADDR_P0,(uchar*)RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  
  SPI_Write_Reg(WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
  SPI_Write_Reg(WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
  SPI_Write_Reg(WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  SPI_Write_Reg(WRITE_REG+RF_CH,40);       //设置RF通道为40
  SPI_Write_Reg(WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
}

void NRF24L01_ConfigMode(uchar mode)
{// 24l01模式配置
  switch(mode)
  {
  case MODERX:
    NRF_CE0;
    SPI_Write_Reg(WRITE_REG+CONFIG,0x0f);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
    NRF_CE1;
    break;
  case MODETX:
    NRF_CE0;
    SPI_Write_Reg(WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发送模式,开启所有中断
    NRF_CE1;
    break;
  case STANDBYII:
    NRF_CE0;
    break;
  case POWEROFF:
    NRF_CE0;
    SPI_Write_Reg(WRITE_REG+CONFIG,0x0C);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,关闭。。
    break;
  }
  Delay_us(15);
}

void NRF24L01_init(uchar interrupt,uchar check)
{
  nrf24l01_init_pins();
  if(check!=0)
    while(NRF24L01_Check()); // 必须检测到设备才能继续下去
  NRF24L01_ConfigReg();
  NRF24L01_ConfigMode(MODERX);
  if(interrupt!=0)
  {
    nrf24l01_int_en();
  }
  
}

uchar NRF24L01_Check(void)
{//check if there is a NRE24L01
  uchar check_in_buf[5]={0x11,0x22,0x33,0x44,0x55};
  uchar check_out_buf[5]={0x00};
  
  NRF_SCK0;
  NRF_CSN1;    
  NRF_CE0;
  
  SPI_Write_Buf(WRITE_REG+TX_ADDR, check_in_buf, 5);
  SPI_Read_Buf(READ_REG+TX_ADDR, check_out_buf, 5);
  
  if((check_out_buf[0] == 0x11)&&\
     (check_out_buf[1] == 0x22)&&\
     (check_out_buf[2] == 0x33)&&\
     (check_out_buf[3] == 0x44)&&\
     (check_out_buf[4] == 0x55))return 0;
  else return 1;
}



void NRF24L01_ClearBuf()
{
  uchar i=0;
  while(i<RX_PLOAD_WIDTH)
    rece_buf[i]=0;
}

void NRF24L01_PrepBuf(uchar *buf,uchar len)
{
  
  uchar i=0;
  if(len==0){
    while(i<31&&(buf[i]!='\0'))
    {
      rece_buf[i+1]=buf[i];
      i++;
    }
  }
  else
  {
    while(i<31&&i<len)
    {
      rece_buf[i+1]=buf[i];
      i++;
    }
  }
  rece_buf[0] = i;
}