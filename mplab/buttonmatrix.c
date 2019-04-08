#include <xc.h>

#include "buttons.h"
#include "buttonmatrix.h"
#include "shutter.h"
#include "eeprom.h"


/*
** Holt die Tasterkonfiguration aus dem Eeprom
 */

// kann entfallen

void initButtonMatrix()  {

    unsigned char c = 0;
    unsigned char i;

    for ( i = 0; i < 5; i++)  {
      //  initButton( c, eeprom_read(c++), i, buttonUpPressed, buttonUpReleased, 0);
//        initButton( c, eeprom_read(c++), i, commandShutterUp, buttonUpReleased, 0);
    }
    for ( i = 0; i < 5; i++)  {
     //   initButton( c, eeprom_read(c++), i, buttonDownPressed, buttonDownReleased, 0);
  //      initButton( c, eeprom_read(c++), i, commandShutterDown, buttonDownReleased, 0);
    }
}

unsigned char getKeyForShutter(unsigned char shutter)   { // 1 = down, 0 = up

    return eepromRead( shutter);
}

void setKeyForShutter(unsigned char shutter,  unsigned char key)  {

    eepromWrite(shutter, key);
    initButtonMatrix();
}
