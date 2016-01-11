
#include "../common/sys/sys.h"
#include "../common/button/button.h"
#include "../common/nrf24l01/nrf24l01.h"
#include "../common/SMG/SMG.h"

void main( void )
{
  uchar temp[32]={5,'a','a','a','a','a'};
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  button_init();
  SMG_init();
  Clock_Init();
  NRF24L01_init(0,1);
  while(1)
  {
    if(NRF24L01_RxPacket(rece_buf))
    {
      NRF24L01_ConfigMode(MODETX);
      NRF24L01_TxPacket(temp);
      NRF24L01_ConfigMode(MODERX);
    }
    SMG_Display(rece_buf[0]);
  }
}
