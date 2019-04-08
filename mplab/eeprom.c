#include <xc.h>
#include "eeprom.h"


void eepromWrite(unsigned char address, unsigned char data)  {

    while(EECON1bits.WR)
        ;
    
    EEADR = (address & 0x0ff);
  	EEDATA = data;
  	EECON1bits.EEPGD = 0;
	EECON1bits.CFGS = 0;
	EECON1bits.WREN = 1;
	INTCONbits.GIE = 0;
	EECON2 = 0x55;
	EECON2 = 0xAA;
	EECON1bits.WR = 1;
	INTCONbits.GIE = 1;
	EECON1bits.WREN = 0;
    
}



unsigned char eepromRead(unsigned char address)  {
    
    EEADR = (address & 0x0ff);
  	EECON1bits.CFGS  = 0;
	EECON1bits.EEPGD = 0;
	EECON1bits.RD    = 1;
	return EEDATA;      
    
}

