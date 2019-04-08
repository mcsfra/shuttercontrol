/* 
 * File:   shutter.h
 * Author: marcus
 *
 * Created on 19. August 2014, 19:10
 */

#ifndef SHUTTER_H
#define	SHUTTER_H

typedef struct shutter  {

    unsigned char state;
    
    //
    //unsigned char count;   // "Substate"
    // 1.02. - 18.07.2015 - die Zählzeit zum endabschalten ist mit 300 zu gross für char, ne?
    unsigned int count;   // "Substate"


} SHUTTER;


/*
 *     States
 */


#define SH_RUHE                       1  // nichts passiert, der rolladen befindet sich im ruhezustand

#define SH_START_UP                  24  //
#define SH_START_DOWN                25  //

#define SH_UP                        20  // Der Shutter läuft hoch
#define SH_DOWN                      21  // Der Shutter läuft runter

#define SH_PREPARE_UP                22  // nichts passiert, der rolladen befindet sich im ruhezustand
#define SH_PREPARE_DOWN              23  // nichts passiert, der rolladen befindet sich im ruhezustand

#define SH_COOLDOWN_FROM_UP           2  // der rolladen soll umgesteuert werden: von hach nach runter
#define SH_COOLDOWN_FROM_DOWN         3  // der rolladen soll umgesteuert werden: von runter nach hoch

#define SH_COOLDOWN_FROM_UP_PHASE2   26
#define SH_COOLDOWN_FROM_DOWN_PHASE2 27

#define SH_SHORT_UP                   4   // kurz hoch
#define SH_SHORT_DOWN                 5

#define SH_PERMANENT_UP               6   // dauerhaft hoch
#define SH_PERMANENT_DOWN             7

#define SH_STOP                       8  // anhalten
#define SH_STOP2                     30  // schaltet die richtungsrelais auf ruhezustand

#define SH_DISCRIMINATE               9  // 500 ms warten bis wir den nächsten kommando annehmen

#define SH_WAIT                      10

/*
 * Amsteuerung über Buttons
 */


//extern void buttonUpPressed (unsigned char shutternumber);
//extern void buttonUpReleased(unsigned char shutternumber);
//
//extern void buttonDownPressed (unsigned char shutternumber);
//extern void buttonDownReleased(unsigned char shutternumber);

void testShortMove(unsigned char shutternumber);

// Diese Funktionen können perspektivisch entfallen

/*
extern void masterUpPressed (unsigned char dummy);
extern void masterUpReleased(unsigned char dummy);

extern void masterDownPressed (unsigned char dummy);
extern void masterDownReleased(unsigned char dummy);
*/

/*
 * Ansteuerung über Kommandos
 */
extern void commandShutterUp(unsigned char shutternumber);
extern void commandShutterDown(unsigned char shutternumber);
extern void commandShutterStop(unsigned char shutternumber);

extern signed char getShutterstatus(unsigned char which);


typedef unsigned char SHUTTERNUMBER;

extern void          shutterToPosition(SHUTTERNUMBER, unsigned char position);
extern void          allStop(void);
extern void          initShutterStateMachine(void);
extern void          shutterStateMachine(SHUTTERNUMBER which);
extern unsigned char getNumShutters(void);
extern void          fehler(char updown, unsigned char ls);

#endif	/* SHUTTER_H */

