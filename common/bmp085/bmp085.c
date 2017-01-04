#include "../sys/sys.h"
#include "bmp085.h"

unsigned char ac1,ac2,ac3,ac4,ac5,ac6,b1,b2,mb,mc,md;

unsigned char BUF[8];
void Init(void)
{
  SDA_0;
SCL_0;
  SCL_1;
  Delay_us(5);
  SDA_1;
  Delay_us(5);
}
void Start(void)
{
  SDA_1;
  Delay_us(5);
  SCL_1;
  Delay_us(5);
  SDA_0;
  Delay_us(5);
  SCL_0;
  Delay_us(5);
}
void Stop(void)
{
  SDA_0;
  Delay_us(5);
  SCL_1;
  Delay_us(5);
  SDA_1;
  Delay_us(5);
}
void WriteByte(unsigned char WriteData)
{
  unsigned char i;
  for (i=0; i<8; i++)
  {
    SCL_0;
    Delay_us(5);
    if (((WriteData >> 7) & 0x01) == 0x01)
    {
      SDA_1;
    }
    else
    {
      SDA_0;
    }
    Delay_us(5);
    SCL_1;
    WriteData = WriteData << 1;
    Delay_us(5);
  }
  SCL_0;
  SDA_1;
  Delay_us(5);
  Delay_us(5);
}
unsigned char ReadByte(void)
{
  unsigned char i;
  unsigned char TempBit  = 0;
  unsigned char TempData = 0;
  SCL_0;
  Delay_us(5);
  SDA_1;
  for (i=0; i<8; i++)
  {
    Delay_us(5);
    SCL_1;
    Delay_us(5);
    DIR_IN;
    if (SDA_IN == 0x01 /*sda==1*/)
    {
      TempBit = 1;
    }
    else
    {
      TempBit = 0;
    }
    DIR_OUT;
    TempData = (TempData << 1) | TempBit;
    SCL_0;
  }
  Delay_us(5);
  return(TempData);
}
void ReceiveAck(void)
{
  unsigned char i = 0;
  SCL_1;
  Delay_us(5);
  DIR_IN;
  while ((SDA_IN == 0x01 /*sda==1*/) && (i < 255)/*调试方便,可以不要*/)
  {
    i++;
  }
  DIR_OUT;
  SCL_0;
  Delay_us(5);
}
void Acknowledge(void)
{
  SCL_0;
Delay_us(5);
  DIR_OUT;
  SDA_0;
  SCL_1;
  Delay_us(5);
  SCL_0;
}
void Nack(void)
{
  SCL_0;
  Delay_us(5);
  DIR_OUT;
  SDA_1;
  SCL_1;
  Delay_us(5);
  SCL_0;
}
//*********************************************************
//读出BMP085内部数据,连续两个
//*********************************************************
short Multiple_read(uchar ST_Address)
{   

short _data;
    Start();                          //起始信号
    WriteByte(BMP085_SlaveAddress); //发送设备地址+写信号
   ReceiveAck();
    WriteByte(ST_Address);             //发送存储单元地址
   ReceiveAck();
    Start();                           //起始信号
    WriteByte(BMP085_SlaveAddress+1);         //发送设备地址+读信号
   ReceiveAck();
   
        BUF[0] = ReadByte();         //BUF[0]存储0x32地址中的数据
      
           Acknowledge();          //回应ACK     
      BUF[1] = ReadByte();
          Nack();                 //最后一个数据需要回NOACK
    

    Stop();                          //停止信号
    Delay_ms(5);
    _data = BUF[0]<< 8;
_data |= BUF[1]; 
return _data;
}
//初始化BMP085，根据需要请参考pdf进行修改**************
void Init_BMP085()
{
ac1 = Multiple_read(0xAA);
ac2 = Multiple_read(0xAC);
ac3 = Multiple_read(0xAE);
ac4 = Multiple_read(0xB0);
ac5 = Multiple_read(0xB2);
ac6 = Multiple_read(0xB4);
b1 =  Multiple_read(0xB6);
b2 =  Multiple_read(0xB8);
mb =  Multiple_read(0xBA);
mc =  Multiple_read(0xBC);
md =  Multiple_read(0xBE);
}
//*************************************************************
long bmp085ReadPressure(void)
{
	long pressure = 0;

    Start();                   //起始信号
   WriteByte(BMP085_SlaveAddress);   //发送设备地址+写信号
    WriteByte(0xF4);	          // write register address
   WriteByte(0x34);       	  // write register data for pressure
    Stop();                    //发送停止信号
	delay_ms(4);    	                  // max time is 4.5ms
	
	pressure = Multiple_read(0xF6);
	pressure &= 0x0000FFFF;
	
	return pressure;	

	//return (long) bmp085ReadShort(0xF6);
}


