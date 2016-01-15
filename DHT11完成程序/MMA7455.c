/*#include "dht11.h"
void delayus(unsigned int n);
//unsigned char humidity, temperature, check_sum ;   

/**********************************************/
/*           开始 DHT11 温湿度计              */
/* 输入：无           						  */
/* 输出：应答标志  0：应答失败   1：应答成功  */
/**********************************************/
/*unsigned  char start_DHT11(void)
{
 unsigned  char time=0;
 DHT11_OUTPUT;
 DHT11_H; 
 DHT11_L;	   //拉低18ms以上
 delayus(8000);
 DHT11_H; 
 delayus(15);
 DHT11_INPUT;
 
 if(!DHT11_IN)
 {
   while((!DHT11_IN)&&time++);//等待低电平
   while(DHT11_IN&&time++);//等待高电平
   return 1;
 }
 else
   return 0;

}
void read_DHT11(void)
{
   unsigned char tem[5];     //读到的五个数
   unsigned char i, j, temp;
   for(i = 0; i < 5; i ++)
  {
     tem[i] = 0;
     for(j = 0; j < 8; j ++)             
     {
        tem[i] *= 2;    //不可用tem[i] << 1 否则不移位
        temp = 2;       //
        delayus(7);   //先延20uS 再检查30uS        
        while((!DHT11_IN) && temp ++)//避免死等
           if(temp == 2)
              break;
        delayus(12);   //高电平长于26-28uS为1否为0
        temp = 2;   
        if(DHT11_IN)
        {
           while(DHT11_IN && temp ++) //当为1时等到低电平再进入下次循环。
              if(temp == 2)
                 break;
           tem[i] |= 0x01;
        }
        delayus(1);
     }
   //humidity = tem[0];
   //temperature = tem[2];
   //check_sum = tem[4];
  
  }
   
}*/
