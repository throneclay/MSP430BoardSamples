#include "../common/sys/sys.h"
#include "../common/SMG/SMG.h"
#include "../common/ds1302/ds1302.h"
#include "../common/button/button.h"

void main( void )
{
  uchar t=0,i=0,j=0;
  uint time=0;
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  SMG_init();
  Ds1302_Init();
  button_init();
  //Ds1302_Write_Time();
  Clock_Init();
  while(1)
  {
    t = button_scan();
    if(t==4)
      i++;
    if(++j>100){
      j=0;
      switch(i%4)
      {
      case 0:
        time = Get_ds1302_year();
        break;
      case 1:
        time = Get_ds1302_md();
        break;
      case 2:
        time = Get_ds1302_hm();
        break;
      case 3:
        time = Get_ds1302_second();
        break;
      }
  }
    SMG_Display(time);
  }
}
