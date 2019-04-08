/* 
 * File:   buttonmatrix.h
 * Author: marcus
 *
 * Created on 15. April 2015, 21:01
 *
 *  Verbindet Tasten mit Funktionen
 *
 *   */

#ifndef BUTTONMATRIX_H
#define	BUTTONMATRIX_H

void initButtonMatrix(void);
unsigned char getKeyForShutter(unsigned char shutter);
void setKeyForShutter(unsigned char shutter, unsigned char key);

#endif	/* BUTTONMATRIX_H */

