/*#include "dht11.h"
void delayus(unsigned int n);
//unsigned char humidity, temperature, check_sum ;   

/**********************************************/
/*           ��ʼ DHT11 ��ʪ�ȼ�              */
/* ���룺��           						  */
/* �����Ӧ���־  0��Ӧ��ʧ��   1��Ӧ��ɹ�  */
/**********************************************/
/*unsigned  char start_DHT11(void)
{
 unsigned  char time=0;
 DHT11_OUTPUT;
 DHT11_H; 
 DHT11_L;	   //����18ms����
 delayus(8000);
 DHT11_H; 
 delayus(15);
 DHT11_INPUT;
 
 if(!DHT11_IN)
 {
   while((!DHT11_IN)&&time++);//�ȴ��͵�ƽ
   while(DHT11_IN&&time++);//�ȴ��ߵ�ƽ
   return 1;
 }
 else
   return 0;

}
void read_DHT11(void)
{
   unsigned char tem[5];     //�����������
   unsigned char i, j, temp;
   for(i = 0; i < 5; i ++)
  {
     tem[i] = 0;
     for(j = 0; j < 8; j ++)             
     {
        tem[i] *= 2;    //������tem[i] << 1 ������λ
        temp = 2;       //
        delayus(7);   //����20uS �ټ��30uS        
        while((!DHT11_IN) && temp ++)//��������
           if(temp == 2)
              break;
        delayus(12);   //�ߵ�ƽ����26-28uSΪ1��Ϊ0
        temp = 2;   
        if(DHT11_IN)
        {
           while(DHT11_IN && temp ++) //��Ϊ1ʱ�ȵ��͵�ƽ�ٽ����´�ѭ����
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
