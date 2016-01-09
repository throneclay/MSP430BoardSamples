
#include <msp430x14x.h> 
#include "../common/sys/sys.h"
#include "../common/SMG/SMG.h"

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  uint t=0,time=0;
  WDTCTL = WDTPW + WDTHOLD;
  
  Clock_Init();
  SMG_init();
  
  while(1)
  {
    SMG_Display(time);
    if(t++>10)
    {
      t=0;
      if(time++>=9999)
      {
        time=0;
      }
    }
  }
}
