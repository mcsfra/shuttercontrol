#include <xc.h>
#include "protocol.h"
#include "buttons.h"
/*
** Implementiert das eingehende RS485 Protokoll
 * Tastenauswertung
*/


unsigned char protocolState;

#define PSTATE_INIT 0
#define PSTATE_PAYLOAD 1
#define PSTATE_CHECKSUM 2



void initProtocol(void)  {

    protocolState = PSTATE_INIT;

}

static unsigned char device = 0;
static unsigned char button = 0;

void byteReceived(unsigned char b)  {
   
    unsigned char checksum;
    unsigned char code;

    /*
    ** Sind wir am Anfang? Dann hätten wir auch gerne ein 0xF1 als startbyte
    ** Wenn das Gut ist, schalten wir weiter.
    */

    if (protocolState == PSTATE_INIT && b == 0xF1 )  {
        protocolState = PSTATE_PAYLOAD;
        return;
    }

    if ( protocolState == PSTATE_PAYLOAD )  {
       device = (b >> 4) & 0x0F;
       button = b & 0x0F;
       protocolState = PSTATE_CHECKSUM;
       return;
    }

    if ( protocolState == PSTATE_CHECKSUM )  {

        code     = ((device << 4) & 0xF0) | (button & 0x0F);
        checksum = 0xF1 ^ code ;

        if ( checksum == b)  {
            decodeButton(code);
        }
    }

    protocolState = PSTATE_INIT;

}

