
/******************************************************************************************
** Fifo zur Pufferung eingehener Kommandos
** [[ ToDo: BufferOverflows werden nicht geblockt und auch nicht erkannt.
******************************************************************************************/

#include "fifo.h"

/*
** Ringpuffer
*/
unsigned char fifo1[FIFO_SIZE];
unsigned char fifo2[FIFO_SIZE];
unsigned char fifo3[FIFO_SIZE];

/*
** Index der schreibposition
*/
unsigned char fifo1In;
unsigned char fifo2In;
unsigned char fifo3In;

/*
** index der Leseposition
*/
unsigned char fifo1Out;
unsigned char fifo2Out;
unsigned char fifo3Out;

/******************************************************************************************
** Fifo initialisieren.
** Schreib und lesepointer auf 0
*/
void fifoInit()  {

    unsigned char i; 
    
    for ( i = 0; i < FIFO_SIZE; i++)  {
        fifo1[i] = 0x00; 
        fifo2[i] = 0x00; 
        fifo3[i] = 0x00; 
    }
    
    fifo1In  = 0;
    fifo1Out = 0;
    fifo2In  = 0;
    fifo2Out = 0;
    fifo3In  = 0;
    fifo3Out = 0;
}


/******************************************************************************************
** Ein zeichen aus dem Ringpuffer holen.
** Achtung ! Vorher prüfen, ( mit fifoEmpty() )
** ob es zeichen zur Abholung gibt.
*/
unsigned char fifo1Get()  {
    return fifo1[fifo1Out++ & FIFO_MASK];
}

unsigned char fifo2Get()  {
    return fifo2[fifo2Out++ & FIFO_MASK];
}

unsigned char fifo3Get()  {
    return fifo3[fifo3Out++ & FIFO_MASK];
}


/******************************************************************************************
** Ein zeichen in dem Ringpuffer legen.
** es wird nicht geprüft, ob der Puffer voll ist; ggf. wird
** das Älteste Zeichen überschrieben
*/
void fifo1Put(unsigned char in)  {
    fifo1[fifo1In++ & FIFO_MASK] = in;
}

void fifo2Put(unsigned char in)  {
    fifo2[fifo2In++ & FIFO_MASK] = in;
}

void fifo3Put(unsigned char in)  {
    fifo3[fifo3In++ & FIFO_MASK] = in;
}



/******************************************************************************************
** testet, ob der Ringbuffer leer ist.
*/
unsigned char fifo1Empty()  {
    
    return (fifo1In & FIFO_MASK) == (fifo1Out & FIFO_MASK);

}

unsigned char fifo2Empty()  {

  return (fifo2In & FIFO_MASK) == (fifo2Out & FIFO_MASK);

}


unsigned char fifo3Empty()  {

  return (fifo3In & FIFO_MASK) == (fifo3Out & FIFO_MASK);

}

