#ifndef __FIFO_H__
#define __FIFO_H__


#define FIFO_SIZE   16
#define FIFO_MASK 0x0F

extern void          fifoInit  ( void );

extern unsigned char fifo1Get   ( void );
extern void          fifo1Put   ( unsigned char in );
extern unsigned char fifo1Empty ( void );

extern unsigned char fifo2Get   ( void );
extern void          fifo2Put   ( unsigned char in );
extern unsigned char fifo2Empty ( void );

extern unsigned char fifo3Get   ( void );
extern void          fifo3Put   ( unsigned char in );
extern unsigned char fifo3Empty ( void );




#endif
