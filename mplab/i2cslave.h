/* 
 * File:   i2cslave.h
 * Author: marcus
 *
 * Created on 27. Oktober 2014, 19:37
 */

#ifndef I2CSLAVE_H
#define	I2CSLAVE_H

extern void i2cRestart(void);


extern void initI2CSlave(unsigned char adress);
extern void handleI2CSlave(unsigned char data);

extern void handleI2CStateIdle(unsigned char data);
extern void handleI2CStateData(unsigned char data);
extern void handleI2CStateChecksum(unsigned char data);
extern void handleI2CStateAcknowledge(unsigned char data);


extern void executeI2CAsynchronousCommand(void);

extern void i2cProtocolHandler(void);

extern void initI2CParser();
extern void i2cParser(unsigned char data);

unsigned char testCommandOk(unsigned char numBytes);

/*
 * testcases
 */

//extern unsigned char testI2CParserSetChannel();
//extern unsigned char testI2CParserGetChannel();



#endif	/* I2CSLAVE_H */

