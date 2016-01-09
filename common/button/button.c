#include "../sys/sys.h"
#include "button.h"

void button_init()
{
  P1DIR &= 0xF0;
}

void button_init_int()
{
  P1DIR &= 0xF0;
  P1IES = 0X0F;               //P1.0~P1.3选择下降沿中断
  P1IE  = 0X0F;               //打开中断使能
  _EINT();                    //打开全局中断控制位
}

uchar button_scan()
{
  uchar KEY_Temp;
  if(KEY_IN != 0x0f)        //KEY_IN不等于0x0F说明有键按下
  {
    Delay_ms(5);                //延时消抖
    if(KEY_IN != 0x0f)      //确认有键按下
    {
      KEY_Temp = KEY_IN;    //存下按键值      
      while(KEY_IN != 0x0f);//等待按键松开
      switch(KEY_Temp)
      {
        case 0x0e:          //按下S1 , P1.0                
            return KEY1;
        case 0x0d:          //按下S2 , P1.1                
            return KEY2;
        case 0x0b:          //按下S3 , P1.2                
            return KEY3;
        case 0x07:          //按下S4 , P1.3                
            return KEY4;  
        default:
            break;
      } 
    }
  }
  return 0;
}