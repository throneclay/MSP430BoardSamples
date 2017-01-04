#include "../common/sys/sys.h"
#include "../common/SMG/SMG.h"
#include "../common/ds1302/ds1302.h"
#include "../common/button/button.h"

void main( void )
{
  uchar setting=0;
  uchar point=0,lpoint=0;
  uchar t=0,i=0,j=0;
  uint time=0;
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  SMG_init();
  Ds1302_Init();
  button_init();
  Clock_Init();
  setting=0;
  while(1)
  {
    t = button_scan();
    if(setting==1){
      lpoint = 1+point%7;
      if(t==1)
      {
        time_buf[lpoint]--;
      }
      if(t==2)
      {
        time_buf[lpoint]++;
      }
      if(t==3)
      {
        point++;
      }
      if(t==4)
      {
        point = 0;
        setting=0;
        Ds1302_Write_Time(time_buf);
      }
      time =(time_buf[lpoint]&0x0f)+(time_buf[lpoint]>>4)*10;
      time += lpoint*100;
    }
    else{
      if(t==3)
      {
        setting=1;
        Ds1302_Read_Time();
        time = time_buf[point%8];
      }
      if(t==4)
        i++;
      
      if(++j>10){
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
    }
    SMG_Display(time);
  }
}
