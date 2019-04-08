/* 
 * File:   eeprom.h
 * Author: marcus
 *
 * Created on 18. Dezember 2016, 10:55
 */

#ifndef EEPROM_H
#define	EEPROM_H

extern void eepromWrite(unsigned char address, unsigned char data);
extern unsigned char eepromRead(unsigned char address);

#endif	

