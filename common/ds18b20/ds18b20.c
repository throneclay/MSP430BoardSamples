#include "ds18b20.h"

#define Ds18b20_High  P6OUT |= BIT7    //DS18B20数据线输出高
#define Ds18b20_Low   P6OUT &= ~BIT7   //DS18B20数据线输出低
#define Ds18b20_In    P6DIR &= ~BIT7   //DS18B20数据线设置输入
#define Ds18b20_Out   P6DIR |= BIT7    //DS18B20数据线设置输出
#define Ds18b20_Value (P6IN & BIT7)    //DS18B20数据线获取值

float ftemp;
uint zhengshu;
uint xiaoshu;
uchar dN[9];   //串口要显示的字符串


/*******************************************
函数名称：Init_18B20
功    能：对DS18B20进行复位操作
参    数：无
返回值  ：初始化状态标志：1--失败，0--成功
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
        Error = 1;          //初始化失败
    }
    else
    {
        Error = 0;          //初始化成功
    }
    Ds18b20_Out;
    Ds18b20_High;
    
    Delay_us(400);
    
    return Error;
}
/*******************************************
函数名称：Write_18B20
功    能：向DS18B20写入一个字节的数据
参    数：wdata--写入的数据
返回值  ：无
********************************************/
void Write_18B20(uchar wdata)
{
    uchar i;
    
    for(i = 0; i < 8;i++)
    {
        Ds18b20_Low;
        Delay_us(6);            //延时6us
        if(wdata & 0X01)    Ds18b20_High;
        else                Ds18b20_Low;
        wdata >>= 1;
        Delay_us(50);           //延时50us
        Ds18b20_High;
        Delay_us(10);           //延时10us
    }
}
/*******************************************
函数名称：Read_18B20
功    能：从DS18B20读取一个字节的数据
参    数：无
返回值  ：读出的一个字节数据
********************************************/
uchar Read_18B20(void)
{
    uchar i;
    uchar temp = 0;
    
    for(i = 0;i < 8;i++)
    {
        temp >>= 1;
        Ds18b20_Low;
        Delay_us(6);            //延时6us
        Ds18b20_High;
        Delay_us(8);            //延时9us
        Ds18b20_In;
        _NOP();
        if(Ds18b20_Value)   temp |= 0x80;
        Delay_us(45);           //延时45us
        Ds18b20_Out;
        Ds18b20_High;
        Delay_us(10);           //延时10us
    }
    
    return  temp;
}

/*******************************************
函数名称：Skip
功    能：发送跳过读取产品ID号命令
参    数：无
返回值  ：无
********************************************/
void Skip(void)
{
    Write_18B20(0xcc);
}
/*******************************************
函数名称：Convert
功    能：发送温度转换命令
参    数：无
返回值  ：无
********************************************/
void Convert(void)
{
    Write_18B20(0x44);
}
/*******************************************
函数名称：Read_SP
功    能：发送读ScratchPad命令
参    数：无
返回值  ：无
********************************************/
void Read_SP(void)
{
    Write_18B20(0xbe);
}
/*******************************************
函数名称：ReadTemp
功    能：从DS18B20的ScratchPad读取温度转换结果
参    数：无
返回值  ：读取的温度数值
********************************************/
uint Read_Temp(void)
{
    uchar temp_low;
    uint  temp;
    
    temp_low = Read_18B20();      //读低位
    temp = Read_18B20();     //读高位
    temp = (temp<<8) | temp_low;
    
    return  temp;
}
/*******************************************
函数名称：Ds18b20_Convert
功    能：控制DS18B20完成一次温度转换
参    数：无
返回值  ：测量的温度数值
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
        Delay_us(60000); //延时800ms以上
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
函数名称：Disp_Numb
功    能：将从DS18B20读取的11bit温度数据转换
          成液晶显示的温度数字
参    数：temper--11bit温度数据
返回值  ：无
********************************************/
void Disp_Numb()
{
  uint temper = Ds18b20_Convert(); 

    //数值转换
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
