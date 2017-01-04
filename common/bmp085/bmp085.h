#ifndef __BMP085_H__ 

#define __BMP085_H__


#define SDA_1 P1OUT|=BIT0;
#define SDA_0 P1OUT&=~BIT0;
#define SCL_1 P1OUT|=BIT1;
#define SCL_0 P1OUT&=~BIT1;
#define DIR_OUT P1DIR|=BIT0;
#define DIR_IN P1DIR&=~BIT0;
#define SDA_IN  P1IN&BIT0
#define BMP085_SlaveAddress 0xee 
#define CPU_F ((double)8000000)     //定义CPU 的时钟频率为8M
#define Delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))// 定义延时1us 
#define Delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))   // 定义延时1ms
void Init(void);
void Start(void);
void Stop(void);
void WriteByte(unsigned char WriteData);
unsigned char ReadByte(void);
void ReceiveAck(void);
void Acknowledge(void);
void Nack(void);
short Multiple_read(unsigned char ST_Address);
void Init_BMP085();
long bmp085ReadPressure(void);


#endif 
