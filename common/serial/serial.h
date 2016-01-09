
//only using uart0
//ж╩сп uart0

//                For x = 1
//                MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |             P3.4|----------->
//           |                 | 9600 - 8N1
//           |             P3.5|<-----------

//               For x = 2
//               MSP430F149
//            -----------------
//        /|\|             X2IN|-
//         | |                 | 8Mhz
//         --|RST         X2OUT|-
//           |                 |
//           |             P3.4|------------>
//           |                 | 9600 - 8N1
//           |             P3.5|<------------
#ifndef __SERIAL_H__
#define __SERIAL_H__

#define RXBUFFLEN 65

extern uchar rx_buff[];

void serial_init(uchar x);
void Send_char(uchar t);
void Send_string(uchar *ptr);
uchar Read_string();

#endif