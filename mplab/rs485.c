#include <xc.h>
#include "rs485.h"
#include "fifo.h"


/*
** Baudrate
** Datasheet Page 280
*/

#define MEGAHERTZ 1000000
#define FOSC (8 * MEGAHERTZ)


/*
** Sendet Bufferinhalt
*/

void initRS485(void)  {

    BAUDCONbits.BRG16 = 0;     // Baudratengenerator 8-Bit
    BAUDCONbits.CKTXP = 1;     // polaritaet verdrehen
    SPBRG1            = 3;     // Baudrate einstellen
    SPBRGH            = 0;     //

    RCSTA1bits.CREN   = 1;

    RCSTA1bits.SPEN   = 1;

    PIE1bits.RCIE     = 1;
    IPR1bits.RCIP     = 1;
}


void sendRS485(void)  {

   // wenn der fifo etwas enthält UND das senderegister leer ist
   // ein byte entnehmen und senden

   if ( !fifo1Empty() && PIR1bits.TX1IF == 1)  {
       TXREG1 = fifo1Get();
   }
}


void sendSwitchOn(unsigned char device, unsigned char switchNumber)  {

   unsigned char command = 0xF1;
   unsigned char payload =  (device << 4) || (switchNumber & 0x0F);
   unsigned char checksum = command ^ payload;

   fifo1Put( command  ); 
   fifo1Put( payload  );
   fifo1Put( checksum );
}



