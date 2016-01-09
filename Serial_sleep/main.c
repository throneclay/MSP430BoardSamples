
#include "../common/sys/sys.h"
#include "../common/led/led.h"
#include "../common/serial/serial.h"

void main( void )
{
  uchar t[3]={1,'\n',0};
  uchar a=0;
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  led_init();
  serial_init(1);
  Clock_Init();
  Send_string("Start working!");
  Send_string(t);
  while(1)
  {
    _BIS_SR(LPM3_bits + GIE);                 // Enter LPM0 w/ interrupt
    Delay_ms(500);
    a++;
    if(a%2==0)
      led1_on(t[0]);
    else
      led1_off(t[0]);
    if((t[0]=Read_string())!=0)
    {
      Send_string("I got data!");
      t[0]+='0';
      Send_string(t);
    }

  }
}
