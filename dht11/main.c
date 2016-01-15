
#include "../common/sys/sys.h"
#include "../common/dht11/dht11.h"
#include "../common/nrf24l01/nrf24l01.h"
#include "../common/SMG/SMG.h"


void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  
}
