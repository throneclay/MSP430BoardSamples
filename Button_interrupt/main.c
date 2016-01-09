
#include "../common/sys/sys.h"
#include "../common/button/button.h"
#include "../common/led/led.h"

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
}
