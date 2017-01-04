
#include "../common/sys/sys.h"
#include "../common/button/button.h"
#include "../common/led/led.h"

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  Clock_Init();
  led_init();
  button_init_int();
  while(1)
  {
    LPM1;
    led1_on(KEY_Value_int);
    Delay_ms(500);
    led1_off(KEY_Value_int);
  }
}
