
#include "../common/sys/sys.h"
#include "../common/button/button.h"
#include "../common/nrf24l01/nrf24l01.h"
#include "../common/led/led.h"

uchar temp[32]={5,'a','a','a','a','a'};
void main( void )
{

  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  button_init();
  led_init();
  Clock_Init();
  NRF24L01_init(1,1);
  NRF24L01_ConfigMode(MODETX);
  NRF24L01_TxPacket(temp);
  NRF24L01_ConfigMode(MODERX);
  while(1)
  {
    LPM3;  

    //SMG_Display(rece_buf[0]);
    led1_on(0);
    Delay_ms(300);
    led1_off(0);
    Delay_ms(300);
  }
}
#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
  if(P2IFG & 0x40) {
    LPM3_EXIT;
    NRF24L01_RxPacket(rece_buf);
  }
  P2IFG =0x00;
}