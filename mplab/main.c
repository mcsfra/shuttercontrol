#include <xc.h>        /* XC8 General Include File */
#include <htc.h>

#include "hardware.h"
#include "buttons.h"
#include "rs485.h"
#include "fifo.h"
#include "protocol.h"
#include "shutter.h"
#include "masterswitch.h"
#include "shutterhardware.h"
#include "i2cslave.h"
#include "buttonmatrix.h"

/*
 * Rolladensteuerung Version 1.0 - Juli 2014
 *
 * Key Characteristics:
 *
 * 5 Roll�den k�nnen gesteuert werden
 *
 *
 * Das Protokoll
 *
 * Fx -- Commando
 * xx -- Data
 * xx -- checksum
 */

/*
 * Kommandos
 *
 * Taste gedr�ckt:    F1 DT CH   // DT ist adresse des devices - T ist tastennummer wird solange wiederholt solange taste gedr�ckt ist
 * Taste losgelassen: F2 DT CH   // DT ist adresse des devices - T ist tastennummer wird ein paar mal gesendet wenn taste losgelassen
 *
 */

/*

Es bedarf einiger Logik.

1. Vorrangschaltung: Der erste gedr�ckte Taster gilt. Wenn weitere gedr�ckt werden, wird alles gestoppt.
2. Ein taster muss eine zeit lang gedr�ckt bleiben, bis das kommando gehalten wird.
3. die entsprechenden ausg�nge verweilen eine gewisse zeit im ein zustand und werden dann ausgeschaltet.
4. Da es kein "aus" signal im protokoll gibt, ben�tige ich einen Monoflop, der retriggert wird, also eine
   art watchdog f�r die eingangssignale
5. es k�nnen mehrere roll�den gleichzeitug verfahren werden.

*/
unsigned char data[32];
unsigned char c;

extern unsigned char bButtonTimer;
extern unsigned char bShutterTimer;

unsigned char debugCount = 0; 


// wird von __delay_ms benoetigt
#define _XTAL_FREQ 8000000

void main(void)
{
    c = 0;
    int recv;
    
    /*
     *  Sowohl Master als aus Slave (also das hier) h�ngen an derselben Stromversorgung
     *  Es hat sich gezeigt, dass diese software nicht startet, wenn beide teile
     *  gleichzeitig bestromt werden. 
     *  Diese 3 delays helfen. 
     */
    
    for ( c = 0; c < 100; c++)  {
       __delay_ms(10);
    }
    for ( c = 0; c < 100; c++)  {
       __delay_ms(10);
    }
    for ( c = 0; c < 100; c++)  {
       __delay_ms(10);
    }

    
    c = 0; 
    
    initHardware();
    initProtocol();
    initButtonTimer();
    initShutterStateMachine();
    initRS485();

    initI2CSlave(0x88);
   
    initButtonMatrix();             // Buttons sind keine Physischen Kn�pfe sondern das was seriell reinkommt.

    RCONbits.IPEN   = 1;    //
    INTCONbits.PEIE = 1;

    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;

    for (;;)  {

        if ( debugCount++ == 0)  {
            if ( LATAbits.LATA0 == 0)
              LATAbits.LATA0 = 1;
            else 
              LATAbits.LATA0 = 0;
        }
        
        if(!fifo1Empty())  {
           recv = fifo1Get();
           byteReceived(recv);
        }

        if(bButtonTimer)  {
           buttonsReset();
           bButtonTimer = 0; 
        }

        if(bShutterTimer)  {
            bShutterTimer = 0;
  
            for ( c = 0; c < getNumShutters(); c++)  {
               shutterStateMachine(c);
           }
        }
       
         while ( !fifo2Empty())  {
           i2cParser(fifo2Get());
         }
       
        /*
        * �bersetzen In Normale Buttons 
        */
        
        testeMasterTaster(); 
    }
}
