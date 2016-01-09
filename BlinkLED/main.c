
#include <msp430x14x.h> 
#include "../common/sys/sys.h"
#include "../common/led/led.h"

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  uchar i=0;
  WDTCTL = WDTPW + WDTHOLD;
  
  Clock_Init();
  led_init();
  
  while(1)
  {
    for(i=0;i<8;i++){
      led1_on(i);
      Delay_ms(500);
     }
    for(i=0;i<8;i++){
      led1_off(i);
      Delay_ms(500);
     }
  }
}