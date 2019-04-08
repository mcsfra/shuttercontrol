#include <xc.h>        

#include "hardware.h"
#include "buttons.h"
#include "rs485.h"
#include "fifo.h"
#include "buttons.h"
#include "shuttergroups.h"

#define MAXBUTTONS 24

/*
 *  Taster-Aus Hysterese.
 *  In Kunjunktion mit dem Timer-Preload (Timer0) und der Geschwindigkeit der Seriellen Schnittstelle
 *  Bei 31Kbaud habe ich eine merkliche latenz.
 *
 *  Abzuwägen ist zwischen latenz und schalterflattern.
 */

#define SCHWELLWERT 5

BUTTON buttons[MAXBUTTONS];

unsigned char lastPressedKey = 0x00;

/*
 * Damit wir feststellen koennen, ob ein Button n i c h t  m e h r gedrueckt ist,
 * benoetigen wir einen timer, hier stellen wir dann fest, ob ein watchdogevent eingetreten ist
 */

void initButtonTimer(void)  {

    unsigned char i; 
    
    for ( i = 0; i < MAXBUTTONS; i++)  {
        buttons[i].isActive = 0; 
        buttons[i].watchdog = 0; 
    }
    
    T0CONbits.TMR0ON = 0;

    TMR0H = 0;
    TMR0L = 0;
    INTCONbits.TMR0IF = 0;

    T0CONbits.T08BIT  = 0;    // 16 Bit
    T0CONbits.T0CS    = 0;    // INput Source: FOSC/4
    T0CONbits.PSA     = 0;    // Use Prescaler (nagative logik. grrr)

    T0CONbits.T0PS    = 0;    // Prescaler 1:16

    RCONbits.IPEN      = 1;   // Periphere Interrupts ein
    INTCONbits.PEIE    = 1;   // das sollte garnicht nötig sein, oder?
    INTCON2bits.TMR0IP = 0;   // Low-Prio
    INTCONbits.TMR0IE  = 1;   // Interrupt für Timer ein
  
    /*
    ** und nun einschalten
    */
    T0CONbits.TMR0ON = 1;
}

/*
 *
 */

unsigned char getLastPressedKey(void)  {

    return lastPressedKey;
}


void decodeButton(unsigned char code)  {

    signed char buttonIndex = findShuttergroupButton(code) ;
    
    lastPressedKey = code; 
    
    if (buttonIndex != -1)  {
        
        if ( buttons[buttonIndex].isActive == 0 )
           handleShuttergroupButton(buttonIndex);
        
        buttons[buttonIndex].isActive = 1;
        buttons[buttonIndex].watchdog = 0;
        
    }
}


/*
 *    15.04.2016: Hier soll nicht nur das reset einees konopfes gemacht werden 
 * sondern auch das "einrasten" also das verhalten taster ./. schalter
 *
 *   Buttons signalisieren nur ihren "Ein" zustand. Der 
 *   "Aus" zustand muss also erzeugt werden. Das geschieht hier
 *
 *   Der Watchdog wert wird duch die knopfempfangsfunktion regelmässig auf 0 gesetzt
 */


void buttonsReset(void)  {

   unsigned char i;

   for ( i = 0; i < MAXBUTTONS; i++)  {

       if ( buttons[i].watchdog > SCHWELLWERT)  {
           
           /*
            * Es wurde nun "länger" kein knopfsignal empfangen,
            * release des knopfes
            */
           
           if ( buttons[i].isActive )  {
               buttons[i].watchdog = 0;
               releaseShuttergroupButton(i); 
               
           }
            buttons[i].isActive = 0; 
       }
       else  {
           if ( buttons[i].isActive )  {
               buttons[i].watchdog++;
           }
       }     
   }
}


// EOF
