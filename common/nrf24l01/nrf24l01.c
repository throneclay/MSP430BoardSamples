
#include "../sys/sys.h"
#include "nrf24l01.h"

uchar TX_ADDRESS[TX_ADR_WIDTH]={0x68,0x86,0x66,0x88,0x28}; //���͵�ַ
uchar RX_ADDRESS[RX_ADR_WIDTH]={0x68,0x86,0x66,0x88,0x28}; //���͵�ַ

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
//�ж�ʹ�ܺ��ж�ֹͣ
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

/////////////////////////��д����/////////////////////////////////////
uchar SPI_RW(uchar byte)
{
  uchar bit_ctr;
  for(bit_ctr=0;bit_ctr<8;bit_ctr++) // ���8λ
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

/////////////////////д�Ĵ���/////////////////////////////////////////////
uchar SPI_Write_Reg(uchar reg, uchar value)
{
  uchar status;
  
  NRF_CSN0;                            // CSN low, init SPI transaction
  status = SPI_RW(reg);                        // select register
  SPI_RW(value);                              // ..and write value to it..
  NRF_CSN1;                            // CSN high again
  return(status);                             // return nRF24L01 status byte
}

/////////////////////////��һ���ֽڴ�24L01/////////////////////////////////////
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
  if(stat&MAX_TX)//�ﵽ����ط�����
  {
    SPI_Write_Reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
    return MAX_TX; 
  }
  if(stat&TX_OK)//�������
  {
    return TX_OK;
  }
  return 0xff;//����ʧ��
}

void NRF24L01_ConfigReg()
{// 24l01�����Ĵ���������
  NRF_CE0;
  SPI_Write_Reg(WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��
  SPI_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ���    
  SPI_Write_Buf(WRITE_REG+TX_ADDR,(uchar*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
  SPI_Write_Buf(WRITE_REG+RX_ADDR_P0,(uchar*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  
  SPI_Write_Reg(WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  SPI_Write_Reg(WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  SPI_Write_Reg(WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  SPI_Write_Reg(WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
  SPI_Write_Reg(WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
}

void NRF24L01_ConfigMode(uchar mode)
{// 24l01ģʽ����
  switch(mode)
  {
  case MODERX:
    NRF_CE0;
    SPI_Write_Reg(WRITE_REG+CONFIG,0x0f);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
    NRF_CE1;
    break;
  case MODETX:
    NRF_CE0;
    SPI_Write_Reg(WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
    NRF_CE1;
    break;
  case STANDBYII:
    NRF_CE0;
    break;
  case POWEROFF:
    NRF_CE0;
    SPI_Write_Reg(WRITE_REG+CONFIG,0x0C);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,�رա���
    break;
  }
  Delay_us(15);
}

void NRF24L01_init(uchar interrupt,uchar check)
{
  nrf24l01_init_pins();
  if(check!=0)
    while(NRF24L01_Check()); // �����⵽�豸���ܼ�����ȥ
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