
#ifndef __DS1302_H__
#define __DS1302_H__

#include "../sys/sys.h"

#define DS_SCL  BIT0        //DS_SCL = P2.5 
#define DS_SDA  BIT1        //DS_SDA = P2.6 
#define DS_RST  BIT3        //DS_RST = P2.7 
#define DS_RST_IN 	P2DIR &= ~DS_RST
#define DS_RST_OUT 	P2DIR |= DS_RST
#define DS_RST0 	P2OUT &= ~DS_RST
#define DS_RST1 	P2OUT |= DS_RST
#define DS_SCL_IN 	P2DIR &= ~DS_SCL
#define DS_SCL_OUT 	P2DIR |= DS_SCL
#define DS_SCL0 	P2OUT &= ~DS_SCL
#define DS_SCL1 	P2OUT |= DS_SCL
#define DS_SDA_IN 	P2DIR &= ~DS_SDA
#define DS_SDA_OUT 	P2DIR |= DS_SDA
#define DS_SDA0 	P2OUT &= ~DS_SDA
#define DS_SDA1 	P2OUT |= DS_SDA
#define DS_SDA_BIT 	P2IN & DS_SDA

//复位脚
#define RST_CLR	DS_RST0		//电平置低
#define RST_SET	DS_RST1		//电平置高

//双向数据
#define IO_CLR	DS_SDA0		//电平置低
#define IO_SET	DS_SDA1		//电平置高
#define IO_IN	DS_SDA_IN	//电平读取
#define IO_OUT	DS_SDA_OUT	//电平读取
#define IO_R	DS_SDA_BIT	//电平读取

//时钟信号
#define SCK_CLR	DS_SCL0		//时钟信号
#define SCK_SET	DS_SCL1		//电平置高


#define ds1302_sec_add			0x80		//秒数据地址
#define ds1302_min_add			0x82		//分数据地址
#define ds1302_hr_add			0x84		//时数据地址
#define ds1302_date_add			0x86		//日数据地址
#define ds1302_month_add		0x88		//月数据地址
#define ds1302_day_add			0x8a		//星期数据地址
#define ds1302_year_add			0x8c		//年数据地址
#define ds1302_control_add		0x8e		//控制数据地址
#define ds1302_charger_add		0x90 					 
#define ds1302_clkburst_add		0xbe

extern unsigned char time_buf[]; 

void Ds1302_Write_Byte(unsigned char addr, unsigned char d);
unsigned char Ds1302_Read_Byte(unsigned char addr) ;
//void Ds1302_Write_Time(void) ;
void Ds1302_Read_Time(void)  ;
void Ds1302_Init(void);
void Send_DS1302(void);
unsigned int Get_ds1302_year();

unsigned int Get_ds1302_md();
unsigned int Get_ds1302_hm();
unsigned int Get_ds1302_second();

#endif
