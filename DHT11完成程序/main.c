#include <msp430x14x.h>
#include "dht11.h"
#include "uart0.h"
#include <stdio.h>

char s[40];
void delayus(unsigned int n);
typedef unsigned int    U16;
typedef unsigned char   U8;
unsigned char  U8FLAG,k;
unsigned char  U8count,U8temp;
unsigned char  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
unsigned char  U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
unsigned char  U8comdata;
unsigned char  outdata[5];  //定义发送的字节数	   
unsigned char  indata[5];
unsigned char  count, count_r=0;
unsigned int   U16temp1,U16temp2;

/*******************************************
函数名称：delay
功    能：延时约15us的时间
参    数：无
返回值  ：无
********************************************/

void delayus(unsigned int i)   //delay 2.5us at 8M
{  
   unsigned int ii;
   for(ii =i;ii > 0;ii--)
   { 
      _NOP();_NOP();_NOP();_NOP();   
      _NOP();_NOP();_NOP();_NOP();
      _NOP();_NOP();_NOP();_NOP();
      _NOP();_NOP();_NOP();_NOP();   
   }    
}

/****************************************************************************
* 名    称：ClkInit
* 功    能：时钟系统初始化  MCLK为8MHz，SMCLK为1MHz
* 入口参数：无
* 出口参数：无
****************************************************************************/

void ClkInit()
{
    char i;
   /*------选择系统主时钟为8MHz-------*/
    BCSCTL1 &= ~XT2OFF;                 //打开XT2高频晶体振荡器
    do
    {
        IFG1 &= ~OFIFG;                 //清除晶振失败标志
        for (i = 0xFF; i > 0; i--);     //等待8MHz晶体起振
    }
    while ((IFG1 & OFIFG));             //晶振失效标志仍然存在？
    BCSCTL2 |= SELM_2 + SELS;           //MCLK和SMCLK选择高频晶振
    
    TACTL |= TASSEL_2 + ID_3;           //计数时钟选择SMLK=8MHz，1/8分频后为1MHz    
}

//////////////////////DHT11///////////////////////////////////////////////////////

void COM(void)
{
    U8 i;
    DHT11_INPUT;
    
	for(i=0;i<8;i++)	   
	{
		U8FLAG=2;	
		while( (!DHT11_IN) && U8FLAG++ );
        delayus(15); //40us
		U8temp=0;

		if(DHT11_IN) 			        //26-28us高电平表示'0',30us后若还为高电平则表示为'1'
		   U8temp=1;
		   U8FLAG=2;
		while( (DHT11_IN) && U8FLAG++ );        //超时则U8FLAG到255后变为0,又自加一次变为1退出while
		                                        //超时则跳出for循环		  
		if(U8FLAG==1)
		break;//判断数据位是0还是1	  
		// 如果高电平高过预定0高电平值则数据位为 1 
	    U8comdata<<=1;
	    U8comdata|=U8temp;        //0
	}//rof
}

//--------------------------------
//-----湿度读取子程序 ------------
//--------------------------------
//----以下变量均为全局变量--------
//----温度高8位== U8T_data_H------
//----温度低8位== U8T_data_L------
//----湿度高8位== U8RH_data_H-----
//----湿度低8位== U8RH_data_L-----
//----校验 8位 == U8checkdata-----
//----调用相关子程序如下----------
//---- Delay();, Delay_10us();,COM(); 
//--------------------------------

void RH(void)
{
    DHT11_OUTPUT;
	DHT11_L;
	delayus(8000);                   //主机拉低18ms 
    DHT11_H;                        //总线由上拉电阻拉高 主机延时20-40us
    delayus(15);                    //40us;	
	DHT11_H;                       //主机设为输入 判断从机响应信号 
    DHT11_INPUT;                   //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行
	if(!DHT11_IN)		      //T !	  
	{
		U8FLAG=2;                //判断从机发出 80us 的低电平响应信号是否结束	
		while( (!DHT11_IN) && U8FLAG++ );
		U8FLAG=2;                //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
		while( (DHT11_IN) && U8FLAG++ );//数据接收状态		 
		COM();                   //一个COM()调用接收一个字节的数据
		U8RH_data_H_temp=U8comdata;
		COM();
		U8RH_data_L_temp=U8comdata;
		COM();
		U8T_data_H_temp=U8comdata;
		COM();
		U8T_data_L_temp=U8comdata;
		COM();
		U8checkdata_temp=U8comdata;
        DHT11_OUTPUT;
		DHT11_H;                  //数据校验 	
		U8temp=(U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
		if(U8temp==U8checkdata_temp)
		{
		  U8RH_data_H=U8RH_data_H_temp;
		  U8RH_data_L=U8RH_data_L_temp;
		  U8T_data_H=U8T_data_H_temp;
		  U8T_data_L=U8T_data_L_temp;
		  U8checkdata=U8checkdata_temp;
		}//fi
	}//fi
}
///////////////////////主函数///////////////////////////////////////////
void  main( void )
{
  
  WDTCTL=WDTPW+WDTHOLD;//关看门狗
  ClkInit();
  InitUART();
  PutString("DHT11 TEST");
  delay_ms(1000);
  delay_ms(1000);
  //int k= 10;
  while(1)
  {
    RH();
    PutString("DHT11 EXIST AND ACQUIRE SUCCESSFUL!");
    //sprintf(s, "%d.%d %d.%d\r\n",U8RH_data_H,U8RH_data_L,U8T_data_H,U8T_data_L);
    //PutString(s);
    
    sprintf(s, "%d.%d",U8RH_data_H,U8RH_data_L); 
    Send1Char('R'); 
    Send1Char('H'); 
    Send1Char('='); 
    PutString(s);
    sprintf(s, "%d.%d",U8T_data_H,U8T_data_L);
    Send1Char('T');  
    Send1Char('='); 
    PutString(s);
    
    delay_ms(1000);
    delay_ms(1000);
  }
  
}
