
#include <msp430x14x.h> 
#include "sys/sys.h"
#include "SMG/SMG.h"

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  return 0;
}
