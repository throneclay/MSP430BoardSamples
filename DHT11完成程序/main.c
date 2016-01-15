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
unsigned char  outdata[5];  //���巢�͵��ֽ���	   
unsigned char  indata[5];
unsigned char  count, count_r=0;
unsigned int   U16temp1,U16temp2;

/*******************************************
�������ƣ�delay
��    �ܣ���ʱԼ15us��ʱ��
��    ������
����ֵ  ����
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
* ��    �ƣ�ClkInit
* ��    �ܣ�ʱ��ϵͳ��ʼ��  MCLKΪ8MHz��SMCLKΪ1MHz
* ��ڲ�������
* ���ڲ�������
****************************************************************************/

void ClkInit()
{
    char i;
   /*------ѡ��ϵͳ��ʱ��Ϊ8MHz-------*/
    BCSCTL1 &= ~XT2OFF;                 //��XT2��Ƶ��������
    do
    {
        IFG1 &= ~OFIFG;                 //�������ʧ�ܱ�־
        for (i = 0xFF; i > 0; i--);     //�ȴ�8MHz��������
    }
    while ((IFG1 & OFIFG));             //����ʧЧ��־��Ȼ���ڣ�
    BCSCTL2 |= SELM_2 + SELS;           //MCLK��SMCLKѡ���Ƶ����
    
    TACTL |= TASSEL_2 + ID_3;           //����ʱ��ѡ��SMLK=8MHz��1/8��Ƶ��Ϊ1MHz    
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

		if(DHT11_IN) 			        //26-28us�ߵ�ƽ��ʾ'0',30us������Ϊ�ߵ�ƽ���ʾΪ'1'
		   U8temp=1;
		   U8FLAG=2;
		while( (DHT11_IN) && U8FLAG++ );        //��ʱ��U8FLAG��255���Ϊ0,���Լ�һ�α�Ϊ1�˳�while
		                                        //��ʱ������forѭ��		  
		if(U8FLAG==1)
		break;//�ж�����λ��0����1	  
		// ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1 
	    U8comdata<<=1;
	    U8comdata|=U8temp;        //0
	}//rof
}

//--------------------------------
//-----ʪ�ȶ�ȡ�ӳ��� ------------
//--------------------------------
//----���±�����Ϊȫ�ֱ���--------
//----�¶ȸ�8λ== U8T_data_H------
//----�¶ȵ�8λ== U8T_data_L------
//----ʪ�ȸ�8λ== U8RH_data_H-----
//----ʪ�ȵ�8λ== U8RH_data_L-----
//----У�� 8λ == U8checkdata-----
//----��������ӳ�������----------
//---- Delay();, Delay_10us();,COM(); 
//--------------------------------

void RH(void)
{
    DHT11_OUTPUT;
	DHT11_L;
	delayus(8000);                   //��������18ms 
    DHT11_H;                        //������������������ ������ʱ20-40us
    delayus(15);                    //40us;	
	DHT11_H;                       //������Ϊ���� �жϴӻ���Ӧ�ź� 
    DHT11_INPUT;                   //�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������
	if(!DHT11_IN)		      //T !	  
	{
		U8FLAG=2;                //�жϴӻ����� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	
		while( (!DHT11_IN) && U8FLAG++ );
		U8FLAG=2;                //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
		while( (DHT11_IN) && U8FLAG++ );//���ݽ���״̬		 
		COM();                   //һ��COM()���ý���һ���ֽڵ�����
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
		DHT11_H;                  //����У�� 	
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
///////////////////////������///////////////////////////////////////////
void  main( void )
{
  
  WDTCTL=WDTPW+WDTHOLD;//�ؿ��Ź�
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
