
#include "../common/sys/sys.h"
#include "../common/SMG/SMG.h"
#include "../common/ds18b20/ds18b20.h"

void main( void )
{
  float temp=0;
  uint t=0;
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  Clock_Init();
  SMG_init();
    Clock_Init();
    temp = Sener_ftemp();
  while(1)
  {

    if(++t>400){
      t=0;
      temp = Sener_ftemp();
    }

    SMG_fDisplay(temp);
    
  }
}
