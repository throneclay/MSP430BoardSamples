#include "../common/sys/sys.h"
#include "../common/bmp085/bmp085.h"

//ÆøÑ¹²¿·Ö
extern  unsigned char ac1,ac2,ac3,ac4,ac5,ac6,b1,b2,mb,mc,md;  
unsigned long  yaqiang=0;

void main()
{
    int_clk();
    Init_BMP085();
    while (1)
    { 
    Init_BMP085();
    yaqiang=bmp085ReadPressure();
    yaqiang>>=8;
    delay_ms(10);
    }
}

