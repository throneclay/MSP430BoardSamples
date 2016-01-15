#ifndef _DHT11_H_
#define _DHT11_H_
#include <msp430x14x.h>
//#define CPU          (8000000)
//#define delay_us(x)   (__delay_cycles((double)x*CPU/1000000.0))
//#define delay_ms(x)   (__delay_cycles((double)x*CPU/1000.0))
//#define CPU_F ((double)8000000)

//#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))

//#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define DHT11_OUTPUT 		P2DIR|=BIT0
#define DHT11_INPUT 		P2DIR&=~BIT0 
#define DHT11_H 		P2OUT|=BIT0
#define DHT11_L 		P2OUT&=~BIT0
#define DHT11_IN 		(P2IN&BIT0)
extern unsigned char humidity, temperature, check_sum ;    
unsigned  char start_DHT11(void);
void read_DHT11(void);

void delayus(unsigned int n);
#endif
