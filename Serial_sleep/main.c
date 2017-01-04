
#include "../common/sys/sys.h"
#include "../common/led/led.h"
#include "../common/serial/serial.h"

void main( void )
{
  uchar t;
  uchar a=0;
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  led_init();
  serial_init(1,1);
  Clock_Init();
  Send_string("Start working!");
  while(1)
  {
    _BIS_SR(LPM3_bits + GIE);                 // Enter LPM0 w/ interrupt
    Delay_ms(500);
    a++;
    if(a%2==0)
      led1_on(t);
    else
      led1_off(t);
    
    if((t=Read_string())!=0)
    {
      Send_string("I got data:");
      t+='0';
      Send_char(t);
      Send_char('\n');
      Send_string(rx_buff);
    }

  }
}
