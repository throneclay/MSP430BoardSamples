#include "../common/sys/sys.h"
#include "../common/led/led.h"
#include "../common/button/button.h"

void main( void )
{
  uchar keymap=0;
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  Clock_Init();
  led_init();
  button_init();
  
  while(1)
  {
    keymap=button_scan();
    if(keymap!=0)
    {
      led1_on(keymap-1);
      Delay_ms(500);
      led1_off(keymap-1);
    }
  }
}