/* 
 * File:   hardware.h
 * Author: marcus
 *
 * Created on 23. March 2014, 19:02
 */

#ifndef __HARDWARE_H__
#define	__HARDWARE_H__

#define MASTERSWITCHPORT_UP   PORTAbits.RA1    // -- Pin 3
#define MASTERSWITCHPORT_DOWN PORTAbits.RA2    // -- Pin 4


#define SHUTTER_ON1 LATBbits.LATB5             // -- Pin 38
#define SHUTTER_ON2 LATBbits.LATB4             // -- Pin 37
#define SHUTTER_ON3 LATBbits.LATB3             // -- Pin 36
#define SHUTTER_ON4 LATBbits.LATB2             // -- Pin 35
#define SHUTTER_ON5 LATBbits.LATB1             // -- Pin 34

#define SHUTTER_DIR1 LATBbits.LATB0            // -- Pin 33
#define SHUTTER_DIR2 LATDbits.LATD5            // -- Pin 28
#define SHUTTER_DIR3 LATCbits.LATC5            // -- Pin 24
#define SHUTTER_DIR4 LATCbits.LATC4            // -- Pin 23
#define SHUTTER_DIR5 LATDbits.LATD2            // -- Pin 21

/*
 * Für die Funktion nicht notwendig, aber gut Dokumentation
 */

/* 
  1 --  /MCLR/Vpp
  2 -- Debug OUt  RA0
  3 --  Master Up (in)
  4 --  Master Down (in)
  5 --   
  6 --  
  7 -- 
  8 -- 
  9 -- 
 10 -- 
 11 --  Vdd
 12 --  Vss
 13 -- 
 14 -- 
 15 -- 
 16 -- 
 17 -- 
 18 -- 
 19 --  SPI Clock
 20 --  SPI In ( Mosi)
 21 --  Shutter Direction 5
 22 --  SPI /SlaveSelect
 23 --  Shutter Direction 4
 24 --  Shutter Direction 3
 25 -- 
 26 -- 
 27 --  SPI Out (Miso)
 28 --  Shutter Direction 2
 20 -- 
 30 -- 
 31 --  Vss
 32 --  Vdd 
 33 --  Shutter Direction 1
 34 --  Shutter Ein 5
 35 --  Shutter Ein 4
 36 --  Shutter Ein 3
 37 --  Shutter Ein 2
 38 --  Shutter Ein 1
 39 --  RB6/PGC
 40 --  RB7/PGD
 
 */



#define TRISAMASK   0b00000110
#define TRISBMASK   0b00000000
#define TRISCMASK   0b11001111
#define TRISDMASK   0b11011011


extern void initHardware(void);


#endif	/* HARDWARE_H */

