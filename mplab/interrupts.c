#include <xc.h>         /* XC8 General Include File */
#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
//#include "spislave.h"
#include "i2cslave.h"
#include "fifo.h"


/* 
** High-priority service
*/

volatile unsigned char bButtonTimer  = 0;
volatile unsigned char bShutterTimer = 0;


void interrupt high_isr(void)  {

    unsigned char data;

    if (PIR1bits.RCIF == 1)  {

        data = RCREG;
        fifo1Put(data);
        PIR1bits.RCIF = 0;
    }

    if ( PIR3bits.SSP2IF == 1 )  {

           /*
            movf SSPSTAT,W ; Get the value of SSPSTAT
            andlw b'00101101' ; Mask out unimportant bits in SSPSTAT.
            movwf Temp ; for comparision checking.
            
            relevante Bits: 
            
            7 - --- 
            6 - ---
            5 - D-/A    
            4 - ---
            3 - StartBit
            2 - R /W
            1 - ---
            0 - BufferFull
            */
           
           
           /*
              movlw b'00001001' ; address, buffer is full.
              xorwf Temp,W ;
              btfss STATUS,Z ; Are we in State1?
              goto State2 ; No, check for next state.....
              -------------memset RXBuffer,0,RX_BUF_LEN ; Clear the receive buffer.
              -------------clrf Index ; Clear the buffer index.
              movf SSPBUF,W ; Do a dummy read of the SSPBUF.
            */
           if ( SSP2STATbits.RW2 == 0 && SSP2STATbits.DA2 == 0 && SSP2STATbits.BF == 1)       {   // Master Write :: Address
               data = SSP2BUF;   // dummyread 
           }
           /*
            movlw b'00101001' ; buffer is full.
            xorwf Temp,W
            btfss STATUS,Z ; Are we in State2?
            goto State3 ; No, check for next state.....
            -----------------   load RXBuffer,Index ; Point to the buffer.
            movf SSPBUF,W ; Get the byte from the SSP.
            --------------------movwf INDF0 ; Put it in the buffer.
            --------------------incf Index,F ; Increment the buffer pointer.
            --------------------movf Index,W ; Get the current buffer index.
            --------------------sublw RX_BUF_LEN ; Subtract the buffer length.
            --------------------btfsc STATUS,Z ; Has the index exceeded the buffer length?
            -------------------- clrf Index
            */
           else if ( SSP2STATbits.RW2 == 0 && SSP2STATbits.DA2 == 1 && SSP2STATbits.BF == 1 )  {   // Master Write :: Data
               data = SSP2BUF;
               fifo2Put(data);
           }
           /*
            andlw b'00101100' ; Mask BF bit in SSPSTAT
            xorlw b'00001100'
            btfss STATUS,Z ; Are we in State3?
            goto State4 ; No, check for next state.....
            movf SSPBUF,W
            ---------------clrf Index ; Clear the buffer index.
            ------------------load RXBuffer,Index ; Point to the buffer
            ------------------movf INDF0,W ; Get the byte from buffer.
            call WriteI2C ; Write the byte to SSPBUF
            ------------------i-ncf Index,F ; Increment the buffer index.
            */
           else if ( SSP2STATbits.RW2 == 1 && SSP2STATbits.DA2 == 0   )  {   // Master Read :: Address
               
               SSP2BUF;   // Register anfassen.
               
               //if ( fifo3Empty())  {
               //     bI2CReadPending = 1; 
               //     return; 
               // }
                
                  // Auf jeden fall lesen.
              /* 
                if ( fifo3Empty())  {
                     PIR3bits.SSP2IF = 0; 
                     bI2CReadPending = 1; 
                     return; 
                }
                */    
                if ( fifo3Empty())  
                   data = '*';
                else 
                   data = fifo3Get();
               
               
                /**/
                while(SSP2STATbits.BF)
                    ;
                do {
                   SSP2CON1bits.WCOL = 0; 
                   SSP2BUF = data; 
                  // if ( rdp < 32 )
                  //     requestedData[rdp++] = data; 
                } 
                while (SSP2CON1bits.WCOL);
                 
           }
           /*
            btfsc SSPCON1,CKP ;
            goto State5
            movlw b'00101100' ; buffer is empty.
            xorwf Temp,W
            btfss STATUS,Z ; Are we in State4?
            goto State5 ; No, check for next state....
            ------------      movf Index,W ; Get the current buffer index.
            ------------      sublw RX_BUF_LEN ; Subtract the buffer length.
            --------------      btfsc STATUS,Z ; Has the index exceeded the buffer length?
            ---------------     clrf Index ; Yes, clear the buffer index.
            ----------------     load RXBuffer,Index ; Point to the buffer
             -------------      movf INDF0,W ; Get the byte
            call WriteI2C ; Write to SSPBUF
            --------------------incf Index,F ; Increment the buffer index.
           */
           else if ( SSP2STATbits.RW2 == 1 && SSP2STATbits.DA2 == 1 && SSP2STATbits.BF == 0 )  {   // Master Read :: Data
          
             //   if ( fifo3Empty())  {
             //       bI2CReadPending = 1; 
             //       return; 
             //   }
               
               
               data = '_'; //nix.
               /*
               if ( fifo3Empty())  
                   data = '*';
               else 
                   data = fifo3Get();
               
               */
                while(SSP2STATbits.BF)
                   ;
                do { 
                   SSP2CON1bits.WCOL = 0; 
                   SSP2BUF = data; 
               //    if ( rdp < 32 )
               //        requestedData[rdp++] = data; 
                } 
                while (SSP2CON1bits.WCOL);
               
                
               /*
               
               if ( fifo3Empty())  {
                     PIR3bits.SSP2IF = 0; 
                     bI2CReadPending = 1; 
                     return; 
                }
                    
                
                
                while(SSP2STATbits.BF)
                    ;
                do {
                   SSP2CON1bits.WCOL = 0; 
                   SSP2BUF = data; 
                } 
                while (SSP2CON1bits.WCOL);
                 */
               
               
           }
          /*
           movf Temp,W ;
           andlw b'00101000' ; Mask RW bit in SSPSTAT
           xorlw b'00101000'
           btfss STATUS,Z ; Are we in State5?
           goto I2CErr ; No, check for next state....
     
    */       
           else if ( SSP2STATbits.DA2 == 1 && SSP2STATbits.BF == 0 && SSP2CON1bits.CKP == 1)   {
               
               // hier irgendeinen reset machen.
               
           }
           
   /*
    I2CErr
nop ; Something went wrong! Set LED
bsf PORTB,7 ; and loop forever. WDT will reset
goto $ ; device, if enabled.
;---------------------------------------------------------------------
; WriteI2C
;---------------------------------------------------------------------
WriteI2C
btfsc SSPSTAT,BF ; Is the buffer full?
goto WriteI2C ; Yes, keep waiting.
DoI2CWrite
bcf SSPCON1,WCOL ; Clear the WCOL flag.
movwf SSPBUF ; Write the byte in WREG
btfsc SSPCON1,WCOL ; Was there a write collision?
goto DoI2CWrite
    
    */
           
           
           
        /* Das muss ich mir näher ansehen.
           else if ((!((sspstat & 0b00101000) ^ 0b00101000)) && SSP1CON1bits.CKP)   { //State 5: Master NACK    
            
           }
           else {
                //do {SSP1BUF;} while(SSP1STATbits.BF);
                //NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); SSP1CON1bits.CKP=1;
            }            
          */  
          SSP2CON1bits.CKP = 1;
          PIR3bits.SSP2IF = 0; 
       }
       /*
        * Datenstrom vom host
        */
    /*    
       if ( PIR3bits.SSP2IF == 1 )  {

          data = SSP2BUF;
          if ( SSP2STATbits.DA2 == 1)  {  // Ist Datenbyte // See Page 259
              
             if ( SSP2STATbits.RW2 == 0)  { 
              //   data = SSP2BUF;
                 fifo2Put(data);
             }
             else  {   // Adressbyte 
                 
               while(SSP2STATbits.BF)
                    ;
                
                do { 
                    SSP2CON1bits.WCOL = 0; 
                    SSP2BUF = 'A'; 
                } 
                while (SSP2CON1bits.WCOL);
                
                Nop(); 
                Nop(); 
                Nop(); 
                SSP2CON1bits.CKP = 1;
             }
          }
          else  {  // Ist Adressbyte
            
             if ( SSP2STATbits.RW2 == 0)  {   // empfangen
                 //SSP2BUF;
                 Nop(); // fifo2Put(data);
             }
             else  {                          // senden
               
                Nop(); 
                Nop(); 
                Nop(); 
                SSP2CON1bits.CKP = 1;
                
                while(SSP2STATbits.BF)
                    ;
                
                do { 
                    SSP2CON1bits.WCOL = 0; 
                    SSP2BUF = 'B'; 
                } 
                while (SSP2CON1bits.WCOL);
                
                Nop(); 
                Nop(); 
                Nop(); 
                SSP2CON1bits.CKP = 1;
                  
             }
          }
          PIR3bits.SSP2IF = 0;
          
       }
*/

    
   /* 
    if ( PIR3bits.SSP2IF == 1 )  {
          PIR3bits.SSP2IF = 0;
          data = SSP2BUF;
          handleSpiSlave(data);
       }
*/

   return;
}



/*
** Low prio Interrupt
*/

void low_priority interrupt low_isr(void)  {

   if ( INTCONbits.T0IF == 1)  {

       bButtonTimer    = 1;
       INTCONbits.T0IF = 0;
       TMR0H = 0xA0;
       TMR0L = 0;
   }

   if ( PIR1bits.TMR1IF == 1)  {

       bShutterTimer    = 1;
       PIR1bits.TMR1IF = 0;
       TMR1H = 0x3C;
       TMR1L = 0xB0;
    }
}