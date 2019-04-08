/* 
 * File:   statemachine.h
 * Author: marcus
 *
 * Created on 11. August 2014, 15:08
 *
 *
 */

#ifndef BUTTONS_H
#define	BUTTONS_H

typedef unsigned char BUTTONINDEX;          // use in loops
typedef unsigned char BUTTONNUMBER;         // incoming from protocol
typedef unsigned char BUTTONFUNCTION;       // internal meaning

typedef struct button  {

   BUTTONNUMBER   buttonNumber;       // device und nummer des Knopfs
   BUTTONFUNCTION buttonFunction;     // device und nummer des Knopfs

   unsigned char isActive;      // ist der knopf akuell als aktiv erkannt ?
   unsigned char watchdog;      // zähler

   void (*risingEdge)(BUTTONFUNCTION bc);
   void (*fallingEdge)(BUTTONFUNCTION bc);
   void (*state)(BUTTONFUNCTION bc);

} BUTTON;

typedef struct button2  {

   unsigned char isActive;      // ist der knopf akuell als aktiv erkannt ?
   unsigned char watchdog;      // zähler
   
   unsigned char pressedCounter; 

} BUTTON2;




extern void initButtonTimer(void);

extern void decodeButton(unsigned char code);

extern void buttonsReset();

extern BUTTON *findButton(BUTTONNUMBER code);
extern void initButton(BUTTONINDEX i, BUTTONNUMBER bn, BUTTONFUNCTION bf, void (*risingEdge)(BUTTONFUNCTION ),  void (*fallingEdge)(BUTTONFUNCTION ), void (*state)(BUTTONFUNCTION ) ) ;
extern void buttonReceived(BUTTON *bp);

extern unsigned char getLastPressedKey(void);

#endif	/* BUTTONS_H */

