#ifndef __DHT11_H__
#define __DHT11_H__

#define SDATA BIT0
#define DHT11_OUT    P4DIR |= SDATA
#define DHT11_IN     P4DIR &= ~SDATA
#define DHT11_1      P4OUT |= SDATA
#define DHT11_0      P4OUT &= ~SDATA
#define DHT11_READ   (P4IN &SDATA)

#endif