#include "../sys/sys.h"
#include "ds1302.h"

unsigned char time_buf[8] ;                         

uchar DS1302_Time[17];

/*------------------------------------------------
           ��DS1302д��һ�ֽ�����
------------------------------------------------*/
void Ds1302_Write_Byte(unsigned char addr, unsigned char d)
{

	unsigned char i;
	RST_SET;
	
	//д��Ŀ���ַ��addr
	addr = addr & 0xFE;     //���λ����
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
	
	//д�����ݣ�d
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
	RST_CLR;					//ֹͣDS1302����
}
/*------------------------------------------------
           ��DS1302����һ�ֽ�����
------------------------------------------------*/

unsigned char Ds1302_Read_Byte(unsigned char addr) 
{

	unsigned char i;
	unsigned char temp;
	
	RST_SET;	

	//д��Ŀ���ַ��addr
	addr = addr | 0x01;//���λ�ø�
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
	
	//������ݣ�temp
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

	RST_CLR;	//ֹͣDS1302����
	//Delay_us(5);
	return temp;
}

/*------------------------------------------------
           ��DS1302д��ʱ������
------------------------------------------------*/
void Ds1302_Write_Time(uchar *buff) 
{
	Ds1302_Write_Byte(ds1302_control_add,0x00);			//�ر�д���� 
//	Ds1302_Write_Byte(ds1302_sec_add,0x80);				//��ͣ 
//	Ds1302_Write_Byte(ds1302_charger_add,0xa9);			//������ 


	Ds1302_Write_Byte(ds1302_year_add,buff[1]);		//�� 
	Ds1302_Write_Byte(ds1302_month_add,buff[2]);	//�� 
	Ds1302_Write_Byte(ds1302_date_add,buff[3]);		//�� 
	Ds1302_Write_Byte(ds1302_day_add,buff[7]);		//�� 
	Ds1302_Write_Byte(ds1302_hr_add,buff[4]);		//ʱ 
	Ds1302_Write_Byte(ds1302_min_add,buff[5]);		//��
	Ds1302_Write_Byte(ds1302_sec_add,buff[6]);		//��
	Ds1302_Write_Byte(ds1302_day_add,buff[7]);		//�� 

	Ds1302_Write_Byte(ds1302_control_add,0x80);			//��д���� 
}

/*------------------------------------------------
           ��DS1302����ʱ������
------------------------------------------------*/
void Ds1302_Read_Time(void)  
{ 
	time_buf[1]=Ds1302_Read_Byte(ds1302_year_add);		//�� 
	time_buf[2]=Ds1302_Read_Byte(ds1302_month_add);		//�� 
	time_buf[3]=Ds1302_Read_Byte(ds1302_date_add);		//�� 
	time_buf[4]=Ds1302_Read_Byte(ds1302_hr_add);		//ʱ 
	time_buf[5]=Ds1302_Read_Byte(ds1302_min_add);		//�� 
	time_buf[6]=(Ds1302_Read_Byte(ds1302_sec_add))&0x7F;    //�� 
	time_buf[7]=Ds1302_Read_Byte(ds1302_day_add);		//�� 
}

/*------------------------------------------------
                DS1302��ʼ��
------------------------------------------------*/
void Ds1302_Init(void)
{
	DS_RST_OUT;
	DS_SCL_OUT;
	DS_SDA_OUT; 
  
	RST_CLR;			//RST���õ�
	SCK_CLR;			//SCK���õ�
	Ds1302_Write_Byte(ds1302_control_add,0x00);			//�ر�д���� 
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

