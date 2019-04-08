#include <xc.h>

#include "shutter.h"
#include "shutterhardware.h"

/*
 *  Hier sind die Steuerungsstatemachine und die funktionen zur Stateinitiierung implementiert. 
 */


/*
 *  Anzahl steuerbarer Rolläden
 */

#define MAXSHUTTERS 5

/*
 * latenzzeit zum dauerlaufen
 */

#define TIME_HOLD_SHORT 15

/*
 * Haltezeit
 */

#define TIME_MAX 300

/*
 * Zeit zwischen richtungswechseln
 */

#define TIME_COOLDOWN 5

SHUTTER shutter[MAXSHUTTERS];


/*
** ansteurung über kommandos
*/

/*
 Hier habe ich einen Architekturfehler gemacht.
 * 
 *  Die Funktion "ButtonPressed implementiert die Statemachine, die sich auf den ROLLADEN bezieht und nicht auf 
 *  den Knopf. 
 *  die Released funkktionen jedoch sind widerum bezogen auf den button.  
 * 
 *  so kann es gehen:
 * 
 *  es gibt 2 methoden, die das drücken und releasen abbilden
 *   TIME_HOLD_SHORT ist eine konstante für die knopfauswertung
 *   
 *  sp->short ist zu allem überfluss auch wieder eine entität des buttons und nicht des shutters...
 * 
 * 
 *   hmpf: neee, shutterstatus und knöppe hängen sehr wohl miteinander zusammen. kacke.
 * 
 *  Ein Beispiel:
 * 
 *  2 Rölläden laufen.
 * Es wird eine taste kurz gedrückt die einen weiteren rolladen bewegt
 * nun wieder losgelassen. Welche rolläden laufen wie weiter? 
 * 
 * das ist nicht vermittelbar.
 * 
 * 
 * Die MASTER-Eingänge werde ich auf eine gruppe zu mappen. Konfiguration wo?
 * 
 */



void commandShutterUp(unsigned char shutternumber)  {

   if ( shutternumber > MAXSHUTTERS-1) 
       return;
    
   SHUTTER *sp = &shutter[shutternumber];

   switch ( sp -> state ) {

       case SH_RUHE:
            sp -> state = SH_PREPARE_UP;
            break;
       case SH_DOWN: 
            sp -> state = SH_COOLDOWN_FROM_DOWN;
           break;
       case SH_UP:
            sp -> state = SH_STOP;
            break;
       case SH_STOP:   // hab ich so gemessen!
            break;

       default:
       //    fehler('U', sp -> state);
           break;
   }
}

void commandShutterDown(unsigned char shutternumber)  {

   if ( shutternumber > MAXSHUTTERS-1) 
       return; 
    
   SHUTTER *sp = &shutter[shutternumber];

   switch ( sp -> state ) {

       case SH_RUHE:
            sp -> state = SH_PREPARE_DOWN;
            break;
       case SH_UP:
            sp -> state = SH_COOLDOWN_FROM_UP;
            break;
       case SH_DOWN:
            sp -> state = SH_STOP;
       case SH_STOP:   // hab ich so gemessen!
            break;
       default:
         //   fehler('D', sp -> state);
            break;
   }
}

void commandShutterStop(unsigned char shutternumber)  {

     int i;

     if ( shutternumber == 'A')  {
        for (i = 0; i < MAXSHUTTERS; i++)
           shutter[i].state = SH_STOP;
    }
    else if ( shutternumber < MAXSHUTTERS )  {
        shutter[shutternumber].state = SH_STOP;
    }
    else   {
        fehler('s','s');
    }

}




/*
 **   [[ Note 5.4.2015: Ein Button-Event kann grundsätzlich während jedes Status' eintreffen.
 **   [[ Also muss auch jeder status berücksichtigt werden.
**    [[ 11.04.2015 --> haste so aber nicht implementiert!
**    [[ 15.03.2015 --> Die statechanges in den ButtonUp/Down prozeduren scheinen falsch zu sein...
**                      Ich aendere das mal.
 */

/*  entfällt
void buttonUpPressed (unsigned char shutternumber) {

   SHUTTER *sp = &shutter[shutternumber];

   switch ( sp -> state ) {

       case SH_RUHE:
            sp -> state = SH_PREPARE_UP;
            break;
       case SH_DOWN: 
            sp -> state = SH_COOLDOWN_FROM_DOWN;
           break;
       case SH_UP:
            sp -> state = SH_STOP;
            break;
       case SH_STOP:   // hab ich so gemessen!
            break;

       default:
       //    fehler('U', sp -> state);
           break;
   }
}
*/
 
/*
 Das gehört tatsächlihc zum buttonhandluing
 */
/*
void buttonUpReleased(unsigned char shutternumber)  {

   SHUTTER *sp = &shutter[shutternumber];

   if ( sp -> count < TIME_HOLD_SHORT )  {  // 
       sp -> state = SH_STOP;
   }
}
*/
/*
 *   ermittelt, wie lange der shutter schon läuft. 
 *  
 */

void testShortMove(unsigned char shutternumber)  {
    
   SHUTTER *sp = &shutter[shutternumber];

   if ( sp -> count < TIME_HOLD_SHORT )  {  // 
       sp -> state = SH_STOP;
   }
    
    
}

/*
void buttonDownPressed (unsigned char shutternumber)  {

   SHUTTER *sp = &shutter[shutternumber];

   switch ( sp -> state ) {

       case SH_RUHE:
            sp -> state = SH_PREPARE_DOWN;
            break;
       case SH_UP:
            sp -> state = SH_COOLDOWN_FROM_UP;
            break;
       case SH_DOWN:
            sp -> state = SH_STOP;
       case SH_STOP:   // hab ich so gemessen!
            break;
       default:
         //   fehler('D', sp -> state);
            break;
   }
}
*/
/*
void buttonDownReleased(unsigned char shutternumber)  {

  SHUTTER *sp = &shutter[shutternumber];

   if ( sp -> count < TIME_HOLD_SHORT )  {
       sp -> state = SH_STOP;
   }
}
*/

/*
void masterUpPressed (unsigned char dummy)  {

    unsigned char i;

    for ( i = 0; i < MAXSHUTTERS; i++ )  {
        buttonUpPressed(i);
    }
}

void masterUpReleased(unsigned char dummy)  {

    unsigned char i;

    for ( i = 0; i < MAXSHUTTERS; i++ )  {
        buttonUpReleased(i);
    }
}

void masterDownPressed (unsigned char dummy)  {

    unsigned char i;

    for ( i = 0; i < MAXSHUTTERS; i++ )  {
        buttonDownPressed(i);
    }
}

void masterDownReleased(unsigned char dummy)  {

    unsigned char i;

    for ( i = 0; i < MAXSHUTTERS; i++ )  {
        buttonDownReleased(i);
    }
}
*/

void shutterStateMachine(SHUTTERNUMBER which)   {

     SHUTTER *sp = &shutter[which];

     if ( sp -> state != SH_RUHE)
        sp -> count++;

     if ( sp -> count >  TIME_MAX)  {
         sp -> state = SH_STOP;
     }
     
     switch ( sp -> state )  {

         case SH_RUHE:
              break;

         case SH_STOP:       
              shutterOff(which);
              sp -> count = 0; 
              sp -> state = SH_STOP2;
              break;
              
         case SH_STOP2:       
              shutterDirectionDown(which);
              sp -> state = SH_RUHE;
              break;

         case SH_PREPARE_UP:
              shutterDirectionUp(which);
              sp -> state = SH_START_UP;
              break;
              
         case SH_PREPARE_DOWN:
              shutterDirectionDown(which);
              sp -> state = SH_START_DOWN;
              break;
              
         case SH_START_UP:
              shutterOn(which);
              sp -> state = SH_UP;
              sp -> count = 0;
              break;
              
         case SH_START_DOWN:
              shutterOn(which);
              sp -> state = SH_DOWN;
              sp -> count = 0;
              break;
              
         case SH_COOLDOWN_FROM_DOWN:
              shutterOff(which);
              sp -> count = 0;
              sp -> state = SH_COOLDOWN_FROM_DOWN_PHASE2;
              break;
              
        case SH_COOLDOWN_FROM_UP:
              shutterOff(which);
              sp -> count = 0;
              sp -> state = SH_COOLDOWN_FROM_UP_PHASE2;
              break;

         case SH_COOLDOWN_FROM_DOWN_PHASE2:
              if ( sp -> count > 5)
                 sp -> state = SH_PREPARE_UP;
              break;
              
        case SH_COOLDOWN_FROM_UP_PHASE2:
             if ( sp -> count > 5)
                 sp -> state = SH_PREPARE_DOWN;   // stimmt dfas ???
             break;

         case SH_UP:
             break;

         case SH_DOWN:
             break;

     }

}


void initShutterStateMachine(void)  {

    unsigned char i;

    T1CONbits.RD16    = 1;
    T1CONbits.T1CKPS  = 2;

    T1CONbits.T1OSCEN = 0;
    T1CONbits.TMR1CS  = 0;
    T1CONbits.TMR1ON  = 1;
    IPR1bits.TMR1IP   = 0;
    PIE1bits.TMR1IE   = 1;

    for (i = 0; i < getNumShutters(); i++)  {
        shutter[i].state = SH_RUHE;
        shutter[i].count = 0;
    }
}

signed char getShutterstatus(unsigned char which)  {
    
    SHUTTER *sp = &shutter[which];

     switch ( sp -> state )  {

         case SH_RUHE:
         case SH_STOP:        // hier noch 500 ms latenz bis zum nächsten
         case SH_STOP2:        // hier noch 500 ms latenz bis zum nächsten
              return 0; 
  
         case SH_PREPARE_UP:
         case SH_START_UP:
         case SH_UP:     
         case SH_COOLDOWN_FROM_DOWN:
         case SH_COOLDOWN_FROM_DOWN_PHASE2:
             return 1;          
             
         case SH_PREPARE_DOWN:
         case SH_START_DOWN:
         case SH_DOWN:
         case SH_COOLDOWN_FROM_UP:
         case SH_COOLDOWN_FROM_UP_PHASE2:
             return -1; 
     }

     return 0;     
}

/*
** Klassisches EEPROM-Thema
*/

unsigned char getNumShutters(void)  {

    return MAXSHUTTERS;
}



void fehler(char updown, unsigned char ls)  {

    char          modUpDown = updown;
    unsigned char lastState = ls;

    for (;;)
       ;
}