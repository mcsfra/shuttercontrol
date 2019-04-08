/* 
 * File:   shuttergroups.h
 * Author: marcus
 *
 * Created on 11. April 2016, 21:56
 */

#ifndef SHUTTERGROUPS_H
#define	SHUTTERGROUPS_H

#define MAXSHUTTERGROUPS 12


extern void storeShutterGroup(unsigned char number, unsigned char bitmask, unsigned char keyUp, unsigned char keyDown);
extern void readShutterGroup(unsigned char number, unsigned char *bitmask, unsigned char *keyUp, unsigned char *keyDown);
extern unsigned char readShutterGroupByte(unsigned char number, unsigned char which);
extern void commandShutterGroupDown(unsigned char group);  
extern void commandShutterGroupUp(unsigned char group);

extern signed char findShuttergroupButton(unsigned char button); 
extern void handleShuttergroupButton(unsigned char buttonIndex); 
extern void releaseShuttergroupButton(unsigned char buttonIndex);

extern void handleShutterShortMove(unsigned char group); 
#endif	



