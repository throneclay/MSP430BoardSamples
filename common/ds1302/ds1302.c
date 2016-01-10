#include "../sys/sys.h"
#include "ds1302.h"

unsigned char time_buf[8] ;                         

uchar DS1302_Time[17];

/*------------------------------------------------
           向DS1302写入一字节数据
------------------------------------------------*/
void Ds1302_Write_Byte(unsigned char addr, unsigned char d)
{

	unsigned char i;
	RST_SET;
	
	//写入目标地址：addr
	addr = addr & 0xFE;     //最低位置零
	for (i = 0; i < 8; i ++) 
	{ 
		if (addr & 0x01) 
		{
			IO_SET;
		}
		else 
		{
			IO_CLR;
		}
		SCK_SET;
		SCK_CLR;
		addr = addr >> 1;
	}
	
	//写入数据：d
	for (i = 0; i < 8; i ++) 
	{
		if (d & 0x01) 
		{
			IO_SET;
		}
		else 
		{
			IO_CLR;
		}
		SCK_SET;
		SCK_CLR;
		d = d >> 1;
	}
	RST_CLR;					//停止DS1302总线
}
/*------------------------------------------------
           从DS1302读出一字节数据
------------------------------------------------*/

unsigned char Ds1302_Read_Byte(unsigned char addr) 
{

	unsigned char i;
	unsigned char temp;
	
	RST_SET;	

	//写入目标地址：addr
	addr = addr | 0x01;//最低位置高
	for (i = 0; i < 8; i ++) 
	{ 
		if (addr & 0x01) 
	   	{
			IO_SET;
		}
		else 
		{
			IO_CLR;
		}
		SCK_SET;
		SCK_CLR;
		addr = addr >> 1;
	}
	
	//输出数据：temp
	for (i = 0; i < 8; i ++) 
	{
		temp = temp >> 1;
		IO_IN;
		if (IO_R) 
	   	{
			temp |= 0x80;
		}
		else 
	   	{
			temp &= 0x7F;
		}
		IO_OUT;
		SCK_SET;
		SCK_CLR;
	}

	RST_CLR;	//停止DS1302总线
	//Delay_us(5);
	return temp;
}

/*------------------------------------------------
           向DS1302写入时钟数据
------------------------------------------------*/
void Ds1302_Write_Time(uchar *buff) 
{
	Ds1302_Write_Byte(ds1302_control_add,0x00);			//关闭写保护 
//	Ds1302_Write_Byte(ds1302_sec_add,0x80);				//暂停 
//	Ds1302_Write_Byte(ds1302_charger_add,0xa9);			//涓流充电 


	Ds1302_Write_Byte(ds1302_year_add,buff[1]);		//年 
	Ds1302_Write_Byte(ds1302_month_add,buff[2]);	//月 
	Ds1302_Write_Byte(ds1302_date_add,buff[3]);		//日 
	Ds1302_Write_Byte(ds1302_day_add,buff[7]);		//周 
	Ds1302_Write_Byte(ds1302_hr_add,buff[4]);		//时 
	Ds1302_Write_Byte(ds1302_min_add,buff[5]);		//分
	Ds1302_Write_Byte(ds1302_sec_add,buff[6]);		//秒
	Ds1302_Write_Byte(ds1302_day_add,buff[7]);		//周 

	Ds1302_Write_Byte(ds1302_control_add,0x80);			//打开写保护 
}

/*------------------------------------------------
           从DS1302读出时钟数据
------------------------------------------------*/
void Ds1302_Read_Time(void)  
{ 
	time_buf[1]=Ds1302_Read_Byte(ds1302_year_add);		//年 
	time_buf[2]=Ds1302_Read_Byte(ds1302_month_add);		//月 
	time_buf[3]=Ds1302_Read_Byte(ds1302_date_add);		//日 
	time_buf[4]=Ds1302_Read_Byte(ds1302_hr_add);		//时 
	time_buf[5]=Ds1302_Read_Byte(ds1302_min_add);		//分 
	time_buf[6]=(Ds1302_Read_Byte(ds1302_sec_add))&0x7F;    //秒 
	time_buf[7]=Ds1302_Read_Byte(ds1302_day_add);		//周 
}

/*------------------------------------------------
                DS1302初始化
------------------------------------------------*/
void Ds1302_Init(void)
{
	DS_RST_OUT;
	DS_SCL_OUT;
	DS_SDA_OUT; 
  
	RST_CLR;			//RST脚置低
	SCK_CLR;			//SCK脚置低
	Ds1302_Write_Byte(ds1302_control_add,0x00);			//关闭写保护 
        Ds1302_Write_Byte(ds1302_sec_add,0x00);
	Ds1302_Write_Byte(0x90,0xa6);				 
	Ds1302_Write_Byte(ds1302_control_add,0x80);				 
}

unsigned int Get_ds1302_year()
{

  unsigned int time = 2000;
  Ds1302_Read_Time();
  time+=(time_buf[1]>>4)*10;
  time+=time_buf[1]&0x0f;
  return time;
}

unsigned int Get_ds1302_md()
{

  unsigned int time = 0;
  Ds1302_Read_Time();
  time+=(time_buf[2]>>4)*1000;
  time+=(time_buf[2]&0x0f)*100;
  time+=(time_buf[3]>>4)*10;
  time+=time_buf[3]&0x0f;
  return time;
}
unsigned int Get_ds1302_hm()
{

  unsigned int time = 0;
  Ds1302_Read_Time();
  time+=(time_buf[4]>>4)*1000;
  time+=(time_buf[4]&0x0f)*100;
  time+=(time_buf[5]>>4)*10;
  time+=time_buf[5]&0x0f;
  return time;
}

unsigned int Get_ds1302_second()
{

  unsigned int time = 0;
  Ds1302_Read_Time();
  time+=(time_buf[5]>>4)*1000;
  time+=(time_buf[5]&0x0f)*100;
  time+=(time_buf[6]>>4)*10;
  time+=time_buf[6]&0x0f;
  return time;
}

