#include <xc.h>
#include "i2cslave.h"
#include "buttons.h"
#include "commands.h"
#include "fifo.h"
#include "shutter.h"
#include "buttonmatrix.h"
#include "shuttergroups.h"

unsigned char getShutterStatusByte1(void);
unsigned char getShutterStatusByte2(void);

/*
 * Hier merke ich mir, ob nach dem Aufrufen einer Szene ein Kanal in der Intensität geändert wurde.
 */
volatile unsigned char bSceneChanged = 0;

volatile unsigned char bStoreDone = 0;  // obsolet???

/*
 * Statemachine States
 */

#define SPISTATE_IDLE 0
#define SPISTATE_DATA 1
#define SPISTATE_CHKS 2   //Checksum
#define SPISTATE_ACK  3   //ACKNOWLEDGE


/*
 * Commands
 */

volatile unsigned char spiState;

volatile unsigned char spiCommand;

unsigned char spiCount;
unsigned char spiChecksum;
unsigned char spiData[64];

unsigned char dummy = 0;

unsigned char testLevel;

void initI2CParser()  {
    
    unsigned char i;
    
    for ( i = 0; i < 64; i++)   {
        spiData[i]  = 0x00;
    }
    spiCount    = 0x00; 
    spiChecksum = 0x00; 
}



void initI2CSlave(unsigned char adress)  {

    unsigned char i;
    
    spiState    = SPISTATE_IDLE;   
    spiCount    = 0;
    spiChecksum = 0;
    
    LATDbits.LATD0   = 0;     // PIN 19 - SCL2
    LATDbits.LATD1   = 0;     // PIN 20 - SDA2

    PORTDbits.RD0    = 0;
    PORTDbits.RD1    = 0;

    ANSELD           = 0;
    TRISDbits.TRISD0 = 1;
    TRISDbits.TRISD1 = 1;
   
     SSP2CON2 = 0x00;       // see Page 262 - 18f45k22.pdf
                            // 7  -- General Call  --> 0 : Disabled
                            // 6  -- Acknowledge received Readonly --> 0
                            // 5  -- Acknowledge Data Bit  das weiss ich nicht 
                            // 4  -- Only Master mode
                            // 3  -- receive enable. master only
                            // 2  -- mastr only  
                            // 1  -- mater only 
                            // 0  -- master only
     
     SSP2CON2bits.SEN2 = 1; 
     
     
     SSP2CON3 = 0x08;       // seet Page 264 - 18f45k22.pdf
     SSP2MSK  = 0xFE;       
     SSP2STAT = 0x00;       
     SSP2ADD  = adress;       // i2c Adress - page 265 --> 0x11
     SSP2CON1 = 0x026;      // See Page 260 - 18f45k22.pdf     

//    i2cRestart();

    PIR3bits.SSP2IF = 0;
    IPR3bits.SSP2IP = 1;
    PIE3bits.SSP2IE = 1;
    
    //PIE3bits.SSP2IE = 0;   // NIX Interrupt

    PEIE = 1; 
}


/*
 *  Diese Funktion dient zum sauberen neustart des SPI-Busses.
 *  Siehe hierzu Seite 217 Datenblait 18F45K22.
 */

void i2cRestart()  {

    SSP2CON1bits.SSPEN = 0;
    SSP2CON1bits.WCOL  = 0;
    SSP2CON1bits.SSPOV = 0;

    while( (LATDbits.LATD0 == 1 && SSP2CON1bits.CKP2 == 0) ||
         (LATDbits.LATD0 == 0 && SSP2CON1bits.CKP2 == 1))
       ;
    SSP2CON1bits.SSPEN = 1;
}

unsigned char myCounter = 0;
unsigned char help;


/*
**
 */

unsigned char testCommandOk(unsigned char numBytes)  {
     
    if ( spiCount == numBytes )  {   // immer eins mehr! wegen inkrement!  Trottel!
        if ( spiChecksum == 0)   {
            fifo3Put('!');
            spiCount = 0; 
            return 1; 
        }
        else  {
            fifo3Put('?');
        }   
        spiCount = 0; 
    }
    return 0; 
}


/*
 * 2. Version des Parsers
 */

void i2cParser(unsigned char data)  {
    
    /*
     * Die Kommunikation läuft Seitens des Clients Streng synchron. 
     * Ergo darf hier nichts mehr im Sendebuffer sein. 
     * Ich mach ihn also leer.
     */
    while( !fifo3Empty())  {
        fifo3Get(); 
    }
    
    if (spiCount == 0)  {
        spiChecksum = data; 
    }
    else  {
        /*
         * Wenn das Datum die checksumme ist ergibt sich nun in spiChecksum 0x00! 
         */
        spiChecksum ^= data; 
    }
    
    spiData[spiCount++] = data; 
    
    switch ( spiData[0] )  {
        case SPICMD_SHUTTERUP:
             if (testCommandOk(3))  {
                commandShutterUp(spiData[1]);
             }
             break;
        case SPICMD_SHUTTERDOWN      : 
             if (testCommandOk(3))  {
                commandShutterDown(spiData[1]);
             }
             break;
        case SPICMD_SHUTTERSTOP      : 
             if (testCommandOk(3))  {
                commandShutterStop(spiData[1]);
             }
             break;
        case SPICMD_SETKEYFORSHUTTER : 
             if (testCommandOk(4))  {
                 setKeyForShutter(spiData[1], spiData[2]);
             }
             break;
        case SPICMD_SETSHUTTERGROUP  :    
             if (testCommandOk(6))  {
                storeShutterGroup(spiData[1], spiData[2], spiData[3], spiData[4]);
             }
            break;
        case SPICMD_GETSHUTTERGROUP  :
             if (testCommandOk(3))  {
            
                spiChecksum = 0x00; 
                data = readShutterGroupByte(spiData[1], 0); 
                fifo3Put(data);
                spiChecksum ^= data; 
                data = readShutterGroupByte(spiData[1], 1);
                fifo3Put(data);
                spiChecksum ^= data;
                data = readShutterGroupByte(spiData[1], 2);
                fifo3Put(data);
                spiChecksum ^= data;
                fifo3Put(spiChecksum); 
             }
             break;      
        case SPICMD_SHUTTERGROUPUP   : 
             if (testCommandOk(3))  {
                 commandShutterGroupUp(spiData[1]);
             }   
            break;
        case SPICMD_SHUTTERGROUPDOWN : 
             if (testCommandOk(3))  {
                commandShutterGroupDown(spiData[1]);
             }
             break;
  
        case SPICMD_GETKEYFORSHUTTER:
             if (testCommandOk(3))  {
                 fifo3Put(getKeyForShutter(spiData[1]));
                 fifo3Put(getKeyForShutter(spiData[1] ^0xFF));
             }
             break;
        case SPICMD_GETLASTKEY      :
             if ( spiCount == 2 )  {   
                if ( spiChecksum == 0xFF)   {  // es kommt rein: kommando und ^kommando das ergibt 0xFF
                    fifo3Put('!');
                    data = getLastPressedKey();
                    fifo3Put(data);  
                    fifo3Put(0xFF^data);  
                }
                else  {
                    fifo3Put('?');
                }   
                spiCount = 0;
              }
              break; 
        case SPICMD_GETSTATUS:
             if ( spiCount == 2 )  {   
                if ( spiChecksum == 0xFF)   {  // es kommt rein: kommando und ^kommando das ergibt 0xFF
                    fifo3Put('!');
                    
                    spiChecksum = 0x00; 
                    data = getShutterStatusByte1();
                    fifo3Put(data);
                    spiChecksum ^= data; 
                    data = getShutterStatusByte2();
                    fifo3Put(data);
                    spiChecksum ^= data; 
                    fifo3Put(spiChecksum);  
                }
                else  {
                    fifo3Put('?');
                }   
                spiCount = 0;
              }
              break; 
        
         default:
            spiCount = 0; 
            break; 
    }
    
}


/*
 * liefert bitmaske für alle shutter, die nach oben fahren
 */

unsigned char getShutterStatusByte1()  {
    
   unsigned char i; 
   unsigned char r = 0;  
   
   for ( i = 0; i < 5; i++)  {
       if ( getShutterstatus(i) == 1)
             r |= (1<<i);
   }
   return r;  
}

/*
 * liefert bitmaske für alle shutter die nach unten fahren
 */

unsigned char getShutterStatusByte2()  {

   unsigned char i;
   unsigned char r = 0;

   for ( i = 0; i < 5; i++)  {
       if ( getShutterstatus(i) == -1)
             r |= (1<<i);
   }
   return r;

}


