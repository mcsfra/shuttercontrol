#include <xc.h>
#include "shuttergroups.h"
#include "shutter.h"
#include "eeprom.h"


#define SHGROUP_OFFSET 0x20
#define SHRGROUP_SIZE  0x03  



void storeShutterGroup(unsigned char number, unsigned char bitmask, unsigned char keyUp, unsigned char keyDown)  {
    
    eepromWrite(SHGROUP_OFFSET + (SHRGROUP_SIZE * number) + 0, bitmask);
    eepromWrite(SHGROUP_OFFSET + (SHRGROUP_SIZE * number) + 1, keyUp);
    eepromWrite(SHGROUP_OFFSET + (SHRGROUP_SIZE * number) + 2, keyDown);
}

void readShutterGroup(unsigned char number, unsigned char *bitmask, unsigned char *keyUp, unsigned char *keyDown)  {
    
    *bitmask = eepromRead(SHGROUP_OFFSET + (SHRGROUP_SIZE * number) + 0);
    *keyUp   = eepromRead(SHGROUP_OFFSET + (SHRGROUP_SIZE * number) + 1);
    *keyDown = eepromRead(SHGROUP_OFFSET + (SHRGROUP_SIZE * number) + 2);
}

unsigned char readShutterGroupByte(unsigned char number, unsigned char which)  {
    
    return eepromRead(SHGROUP_OFFSET + (SHRGROUP_SIZE * number) + which);
}


void commandShutterGroupDown(unsigned char group)  {
    
    unsigned char g = readShutterGroupByte(group, 0);
    
    if ( g & 0x01) commandShutterDown(0);
    if ( g & 0x02) commandShutterDown(1);
    if ( g & 0x04) commandShutterDown(2);
    if ( g & 0x08) commandShutterDown(3);
    if ( g & 0x10) commandShutterDown(4);
}

void commandShutterGroupUp(unsigned char group)  {
    
    unsigned char g = readShutterGroupByte(group, 0);
    
    if ( g & 0x01) commandShutterUp(0);
    if ( g & 0x02) commandShutterUp(1);
    if ( g & 0x04) commandShutterUp(2);
    if ( g & 0x08) commandShutterUp(3);
    if ( g & 0x10) commandShutterUp(4);
}

void handleShutterShortMove(unsigned char group)  {
    
    unsigned char g = readShutterGroupByte(group, 0);
    
    if ( g & 0x01) testShortMove(0);
    if ( g & 0x02) testShortMove(1);
    if ( g & 0x04) testShortMove(2);
    if ( g & 0x08) testShortMove(3);
    if ( g & 0x10) testShortMove(4);
}


/*
 * Gibt einen Knopfindex zurück
 */

signed char findShuttergroupButton(unsigned char button)  {
    
    unsigned char i; 
    
    for ( i = 0; i < MAXSHUTTERGROUPS; i++)  {
        
        if ( eepromRead(SHGROUP_OFFSET + (SHRGROUP_SIZE * i) + 1) == button )  {
            return i; 
        }
 
        if ( eepromRead(SHGROUP_OFFSET + (SHRGROUP_SIZE * i) + 2) == button )  {
            return MAXSHUTTERGROUPS + i;
        }
    }
    return -1;  // No button found
}




void handleShuttergroupButton(unsigned char buttonIndex)    {

   if ( buttonIndex >= MAXSHUTTERGROUPS)  {
       commandShutterGroupUp(buttonIndex - MAXSHUTTERGROUPS);
    }
    else  {
       commandShutterGroupDown(buttonIndex);
    }  

   }

void releaseShuttergroupButton(unsigned char buttonIndex)    {
    
    if ( buttonIndex >= MAXSHUTTERGROUPS)  {
       handleShutterShortMove(buttonIndex - MAXSHUTTERGROUPS);
    }
    else  {
       handleShutterShortMove(buttonIndex);
    }
}


 