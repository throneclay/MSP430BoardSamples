#include "ds18b20.h"

#define Ds18b20_High  P6OUT |= BIT7    //DS18B20�����������
#define Ds18b20_Low   P6OUT &= ~BIT7   //DS18B20�����������
#define Ds18b20_In    P6DIR &= ~BIT7   //DS18B20��������������
#define Ds18b20_Out   P6DIR |= BIT7    //DS18B20�������������
#define Ds18b20_Value (P6IN & BIT7)    //DS18B20�����߻�ȡֵ

float ftemp;
uint zhengshu;
uint xiaoshu;
uchar dN[9];   //����Ҫ��ʾ���ַ���


/*******************************************
�������ƣ�Init_18B20
��    �ܣ���DS18B20���и�λ����
��    ������
����ֵ  ����ʼ��״̬��־��1--ʧ�ܣ�0--�ɹ�
********************************************/
uchar Init_18B20(void)
{
    uchar Error;
    
    Ds18b20_Out;
    Ds18b20_Low;
    Delay_us(500);
    Ds18b20_High;
    Delay_us(55);
    Ds18b20_In;
    _NOP();
    if(Ds18b20_Value)      
    {
        Error = 1;          //��ʼ��ʧ��
    }
    else
    {
        Error = 0;          //��ʼ���ɹ�
    }
    Ds18b20_Out;
    Ds18b20_High;
    
    Delay_us(400);
    
    return Error;
}
/*******************************************
�������ƣ�Write_18B20
��    �ܣ���DS18B20д��һ���ֽڵ�����
��    ����wdata--д�������
����ֵ  ����
********************************************/
void Write_18B20(uchar wdata)
{
    uchar i;
    
    for(i = 0; i < 8;i++)
    {
        Ds18b20_Low;
        Delay_us(6);            //��ʱ6us
        if(wdata & 0X01)    Ds18b20_High;
        else                Ds18b20_Low;
        wdata >>= 1;
        Delay_us(50);           //��ʱ50us
        Ds18b20_High;
        Delay_us(10);           //��ʱ10us
    }
}
/*******************************************
�������ƣ�Read_18B20
��    �ܣ���DS18B20��ȡһ���ֽڵ�����
��    ������
����ֵ  ��������һ���ֽ�����
********************************************/
uchar Read_18B20(void)
{
    uchar i;
    uchar temp = 0;
    
    for(i = 0;i < 8;i++)
    {
        temp >>= 1;
        Ds18b20_Low;
        Delay_us(6);            //��ʱ6us
        Ds18b20_High;
        Delay_us(8);            //��ʱ9us
        Ds18b20_In;
        _NOP();
        if(Ds18b20_Value)   temp |= 0x80;
        Delay_us(45);           //��ʱ45us
        Ds18b20_Out;
        Ds18b20_High;
        Delay_us(10);           //��ʱ10us
    }
    
    return  temp;
}

/*******************************************
�������ƣ�Skip
��    �ܣ�����������ȡ��ƷID������
��    ������
����ֵ  ����
********************************************/
void Skip(void)
{
    Write_18B20(0xcc);
}
/*******************************************
�������ƣ�Convert
��    �ܣ������¶�ת������
��    ������
����ֵ  ����
********************************************/
void Convert(void)
{
    Write_18B20(0x44);
}
/*******************************************
�������ƣ�Read_SP
��    �ܣ����Ͷ�ScratchPad����
��    ������
����ֵ  ����
********************************************/
void Read_SP(void)
{
    Write_18B20(0xbe);
}
/*******************************************
�������ƣ�ReadTemp
��    �ܣ���DS18B20��ScratchPad��ȡ�¶�ת�����
��    ������
����ֵ  ����ȡ���¶���ֵ
********************************************/
uint Read_Temp(void)
{
    uchar temp_low;
    uint  temp;
    
    temp_low = Read_18B20();      //����λ
    temp = Read_18B20();     //����λ
    temp = (temp<<8) | temp_low;
    
    return  temp;
}
/*******************************************
�������ƣ�Ds18b20_Convert
��    �ܣ�����DS18B20���һ���¶�ת��
��    ������
����ֵ  ���������¶���ֵ
********************************************/
uint Ds18b20_Convert(void)
{
    uchar i; 
    
    do
    {
        i = Init_18B20();
    }
    while(i);
    Skip();
    Convert();
    for(i = 20;i > 0;i--)  
        Delay_us(60000); //��ʱ800ms����
    do
    {
        i = Init_18B20();
    }
    while(i);
    Skip();
    Read_SP();
    
    return Read_Temp();
}



/*******************************************
�������ƣ�Disp_Numb
��    �ܣ�����DS18B20��ȡ��11bit�¶�����ת��
          ��Һ����ʾ���¶�����
��    ����temper--11bit�¶�����
����ֵ  ����
********************************************/
void Disp_Numb()
{
  uint temper = Ds18b20_Convert(); 

    //��ֵת��
    if(temper & BIT0) 
    {
        dN[0] = 5;
        dN[1] = 2;
        dN[2] = 6;
    }
    if(temper&BIT1)     
    {
        dN[1] += 5;
        dN[2] += 2;
        dN[3] += 1;
    }
    if(temper & BIT2)     
    {
        dN[2] += 5;
        dN[3] += 2;
        if(dN[2] >= 10)
        {
            dN[2] -= 10;
            dN[3] += 1;
        }
    }
    if(temper&BIT3)     
    {
        dN[3] += 5;
    }
    if(temper & BIT4)
    {
        dN[4] += 1;
    }
    if(temper & BIT5)     
    {
        dN[4] += 2;
    }
    if(temper & BIT6)
    {
        dN[4] += 4;
    }
    if(temper & BIT7)     
    {
        dN[4] += 8;
        if(dN[4] >= 10)
        {
            dN[4] -= 10;
            dN[5] += 1;
        }
    }
    if(temper & BIT8)
    {
        dN[4] += 6;
        dN[5] += 1;
        if(dN[4] >= 10)
        {
            dN[4] -= 10;
            dN[5] += 1;
        }
    }
    if(temper & BIT9)
    {
        dN[4] += 2;
        dN[5] += 3;
        if(dN[4] >= 10)
        {
            dN[4] -= 10;
            dN[5] += 1;
        }
    }
    if(temper & BITA)
    {
        dN[4] += 4;
        dN[5] += 6;
        if(dN[4] >= 10)
        {
            dN[4] -= 10;
            dN[5] += 1;
        }
        if(dN[5] >= 10)
        {
            dN[5] -= 10;
        }
    }
    ftemp = dN[0]*10+dN[1]+dN[3]/10+dN[4]/100+dN[5]/1000+dN[6]/10000;
    zhengshu = dN[0]*10+dN[1];
    xiaoshu = dN[3]*1000+dN[4]*100+dN[5]*10+dN[6];
}