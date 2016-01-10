
#include "../common/sys/sys.h"
#include "../common/SMG/SMG.h"
#include "../common/ds18b20/ds18b20.h"

void main( void )
{
  uint temp=0;
  uint t=0;
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  Clock_Init();
  SMG_init();
    Clock_Init();
    temp = Ds18b20_Convert();
  while(1)
  {
    if(++t>600){
      t=0;
      temp = Ds18b20_Convert();
    }
    SMG_Display(temp);
    
  }
}
