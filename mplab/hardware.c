#include <xc.h>
#include "hardware.h"

void initHardware(void)  {

  // 8MHz internal OscillatorBlock

  OSCCONbits.IDLEN = 0;

  OSCCONbits.IRCF2 = 1;
  OSCCONbits.IRCF1 = 1;
  OSCCONbits.IRCF0 = 0;

  OSCCONbits.SCS1  = 1;
  OSCCONbits.SCS0  = 0;

  /*
  ** I/O
  */

   CM1CON0  = 0;
   CM2CON0  = 0;

   ANSELA  = 0x00;
   LATA    = 0x00;
   PORTA   = 0x00;
   TRISA   = TRISAMASK;

   ANSELB  = 0x00;
   PORTB   = 0x00;
   LATB    = 0x00;
   TRISB   = TRISBMASK;

   ANSELC  = 0x00;
   LATC    = 0x00;
   PORTC   = 0x00;
   TRISC   = TRISCMASK;

   ANSELD  = 0x00;
   LATD    = 0x00;
   PORTD   = 0x00;
   TRISD   = TRISDMASK;

}
