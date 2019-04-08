#include <xc.h>

#include "shutter.h"
#include "shutterhardware.h"
#include "hardware.h"

/*
 * Implementiert die relaisansteuerung.
 * Achtung, das hier steuert direkt die Hardware;
 * Es gibt zwar eine Absicherung gegen umsteuerung, die
 * komplexe steuerung findet aber "eins drüber" statt
 */

void shutterOn(SHUTTERNUMBER shutternumber)  {

    switch (shutternumber)  {

        case 0: SHUTTER_ON1 = 1; break;
        case 1: SHUTTER_ON2 = 1; break;
        case 2: SHUTTER_ON3 = 1; break;
        case 3: SHUTTER_ON4 = 1; break;
        case 4: SHUTTER_ON5 = 1; break;
    }
}

void shutterOff(SHUTTERNUMBER shutternumber)  {

     switch (shutternumber)  {

        case 0: SHUTTER_ON1 = 0; break;
        case 1: SHUTTER_ON2 = 0; break;
        case 2: SHUTTER_ON3 = 0; break;
        case 3: SHUTTER_ON4 = 0; break;
        case 4: SHUTTER_ON5 = 0; break;
    }
}


void shutterDirectionUp(SHUTTERNUMBER shutternumber)  {

     switch (shutternumber)  {
          case 0:
                 if ( SHUTTER_ON1 == 1)
                     return;
                 SHUTTER_DIR1 = 1;
                 break;
         case 1:
                 if ( SHUTTER_ON2 == 1)
                     return;
                 SHUTTER_DIR2 = 1;
                 break;
         case 2:
                 if ( SHUTTER_ON3 == 1)
                     return;
                 SHUTTER_DIR3 = 1;
                 break;
          case 3:
                 if ( SHUTTER_ON4 == 1)
                     return;
                 SHUTTER_DIR4 = 1;
                 break;
          case 4:
                 if ( SHUTTER_ON5 == 1)
                     return;
                 SHUTTER_DIR5 = 1;
                 break;
     }
}



void shutterDirectionDown(SHUTTERNUMBER shutternumber)  {

      switch (shutternumber)  {
          case 0:
                 if ( SHUTTER_ON1 == 1)
                     return;
                 SHUTTER_DIR1 = 0;
                 break;
         case 1:
                 if ( SHUTTER_ON2 == 1)
                     return;
                 SHUTTER_DIR2 = 0;
                 break;
         case 2:
                 if ( SHUTTER_ON3 == 1)
                     return;
                 SHUTTER_DIR3 = 0;
                 break;
          case 3:
                 if ( SHUTTER_ON4 == 1)
                     return;
                 SHUTTER_DIR4 = 0;
                 break;
          case 4:
                 if ( SHUTTER_ON5 == 1)
                     return;
                 SHUTTER_DIR5 = 0;
                 break;
      }
}







