#include "../sys/sys.h"
#include "button.h"

void button_init()
{
  P1DIR &= 0xF0;
}

void button_init_int()
{
  P1DIR &= 0xF0;
  P1IES = 0X0F;               //P1.0~P1.3ѡ���½����ж�
  P1IE  = 0X0F;               //���ж�ʹ��
  _EINT();                    //��ȫ���жϿ���λ
}

uchar button_scan()
{
  uchar KEY_Temp;
  if(KEY_IN != 0x0f)        //KEY_IN������0x0F˵���м�����
  {
    Delay_ms(5);                //��ʱ����
    if(KEY_IN != 0x0f)      //ȷ���м�����
    {
      KEY_Temp = KEY_IN;    //���°���ֵ      
      while(KEY_IN != 0x0f);//�ȴ������ɿ�
      switch(KEY_Temp)
      {
        case 0x0e:          //����S1 , P1.0                
            return KEY1;
        case 0x0d:          //����S2 , P1.1                
            return KEY2;
        case 0x0b:          //����S3 , P1.2                
            return KEY3;
        case 0x07:          //����S4 , P1.3                
            return KEY4;  
        default:
            break;
      } 
    }
  }
  return 0;
}