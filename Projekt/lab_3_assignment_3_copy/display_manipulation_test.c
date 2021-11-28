
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <stdbool.h>
/* ------------------------------------------------------------ */ 
/* Pin definitions for access to OLED control signals on chipKIT Uno32 
*/ 
 

#define prtVddCtrl IOPORT_F 
#define prtVbatCtrl IOPORT_F 
#define prtDataCmd IOPORT_F 
#define prtReset   IOPORT_G 
#define bitVddCtrl BIT_6 
#define bitVbatCtrl BIT_5 
#define bitDataCmd BIT_4 
#define bitReset   BIT_9 
 

/* ------------------------------------------------------------ */ 
/* Symbols describing the geometry of the display. 
 

#define cbOledDispMax  512   //max number of bytes in display buffer 
 

#define ccolOledMax  128  //number of display columns 
#define crowOledMax  32 //number of display rows 
#define cpagOledMax  4 //number of display memory pages 
 
/* ------------------------------------------------------------ */ 
 
/* This array is the off-screen frame buffer used for rendering. 
** It isn't possible to read back from the OLED display device, 
** so display data is rendered into this off-screen buffer and then 
** copied to the display. 

*/ 
BYTE rgbOledBmp[cbOledDispMax];


/* ------------------------------------------------------------ */ 

/*** OledHostInit 
** 
** Parameters: 
**  none 
** 

** Return Value: 
**  none 
** 

** Errors: 
**  none 
** 

** Description: 
**  Perform PIC32 device initialization to prepare for use 
**  of the OLED display. 
**  This example is hard coded for the chipKIT Uno32 and 
**  SPI2. 
*/
void 
OledHostInit() 
 { 
 unsigned int tcfg; 
 

 /* Initialize SPI port 2. 
 */ 
 SPI2CON = 0; 
 SPI2BRG = 15;     //8Mhz, with 80Mhz PB clock 
 SPI2STATbits.SPIROV = 0; 
 SPI2CONbits.CKP = 1; 
 SPI2CONbits.MSTEN = 1; 
 SPI2CONbits.ON = 1; 
 
 /* Make pins RF4, RF5, and RF6 be outputs. 
 */ 
 PORTSetBits(IOPORT_F, bitVddCtrl|bitVbatCtrl|bitDataCmd); 
 PORTSetPinsDigitalOut(prtDataCmd, bitDataCmd);  //Data/Command# select 
 PORTSetPinsDigitalOut(prtVddCtrl, bitVddCtrl);  //VDD power control 
(1=off) 

 PORTSetPinsDigitalOut(prtVbatCtrl, bitVbatCtrl);   //VBAT power control 
(1=off) 
 
 /* Make the RG9 pin be an output. On the Basic I/O Shield, this pin 
 ** is tied to reset. 
 */ 
 PORTSetBits(prtReset, bitReset); 
 PORTSetPinsDigitalOut(prtReset, bitReset); 
 

}

/* ------------------------------------------------------------ */ 
/*** OledDspInit 
** 
** Parameters: 
**  none 
** 

** Return Value: 
**  none 
** 

** Errors: 
**  none 
** 

** Description: 
**  Initialize the OLED display controller and turn the display on. 
*/ 

 

void 
OledDspInit() 
 { 
 
 /* We're going to be sending commands, so clear the Data/Cmd bit 
 */ 
 PORTClearBits(prtDataCmd, bitDataCmd); 
 
 /* Start by turning VDD on and wait a while for the power to come up.
 */ 
 PORTClearBits(prtVddCtrl, bitVddCtrl); 
 DelayMs(1); 
 
 /* Display off command 
 */ 
 Spi2PutByte(0xAE); 
 

 /* Bring Reset low and then high 
 */ 
 PORTClearBits(prtReset, bitReset); 
 DelayMs(1); 
 PORTSetBits(prtReset, bitReset); 
 

 /* Send the Set Charge Pump and Set Pre-Charge Period commands 
 */ 
 Spi2PutByte(0x8D); 
 Spi2PutByte(0x14); 
 
 Spi2PutByte(0xD9); 
 Spi2PutByte(0xF1); 
 
 /* Turn on VCC and wait 100ms 
 */ 
 PORTClearBits(prtVbatCtrl, bitVbatCtrl); 
 DelayMs(100); 
 

 /* Send the commands to invert the display. This puts the display origin 
 ** in the upper left corner. 
 */ 
 Spi2PutByte(0xA1);    //remap columns 
 Spi2PutByte(0xC8);    //remap the rows 
 
 /* Send the commands to select sequential COM configuration. This makes the 
 ** display memory non-interleaved. 
 */ 
 Spi2PutByte(0xDA);    //set COM configuration command 
 Spi2PutByte(0x20);    //sequential COM, left/right remap enabled 
 
 /* Send Display On command 
 */ 
 Spi2PutByte(0xAF); 
 
} 

/* ------------------------------------------------------------ */ 
/*** OledUpdate 
** 
** Parameters: 
**  none 
** 

** Return Value:
**  none 
** 
** Errors: 
**  none 
** 

** Description: 
**  Update the OLED display with the contents of the memory buffer 
*/ 
 

void 

OledUpdate() 
 { 
 int   ipag; 
 int   icol; 
 BYTE * pb; 
 
 pb = rgbOledBmp; 
 
 for (ipag = 0; ipag < cpagOledMax; ipag++) { 
 
  PORTClearBits(prtDataCmd, bitDataCmd); 
 

  /* Set the page address 
  */ 
  Spi2PutByte(0x22);   //Set page command 
  Spi2PutByte(ipag);   //page number 
 

  /* Start at the left column 
  */ 
  Spi2PutByte(0x00);   //set low nybble of column 
  Spi2PutByte(0x10);   //set high nybble of column 
 

  PORTSetBits(prtDataCmd, bitDataCmd); 
 

  /* Copy this memory page of display data. 
  */ 
  OledPutBuffer(ccolOledMax, pb); 
  pb += ccolOledMax; 
 

 } 
 

}

/* ------------------------------------------------------------ */ 

/*** OledPutBuffer 
** 
** Parameters: 
**  cb  - number of bytes to send/receive 
**  rgbTx - pointer to the buffer to send 
** 

** Return Value: 
**  none 
** 

** Errors: 
**  none 
** 

** Description: 
**  Send the bytes specified in rgbTx to the slave. 
 

*/
 
void 
OledPutBuffer(int cb, BYTE * rgbTx) 
 { 
 int   ib; 
 BYTE bTmp; 
 
 /* Write/Read the data 
 */ 
 for (ib = 0; ib < cb; ib++) { 
  /* Wait for transmitter to be ready 
  */ 
  while (SPI2STATbits.SPITBE == 0); 
 
  /* Write the next transmit byte. 
  */ 
  SPI2BUF = *rgbTx++; 
 
  /* Wait for receive byte. 
  */ 
  while (SPI2STATbits.SPIRBF == 0); 
  bTmp = SPI2BUF; 
 
 } 
 

} 
 
/* ------------------------------------------------------------ */ 

/*** Spi2PutByte 

** 
** Parameters: 
**  bVal  - byte value to write 
** 

** Return Value: 
**  Returns byte read 
** 

** Errors: 
**  none 
** 
** Description: 
**  Write/Read a byte on SPI port 2 
*/ 

 
BYTE 
Spi2PutByte(BYTE bVal) 

 { 
 BYTE bRx; 
 

 /* Wait for transmitter to be ready 
 */ 
 while (SPI2STATbits.SPITBE == 0); 
 
 /* Write the next transmit byte. 
 */ 
 SPI2BUF = bVal; 
 
 /* Wait for receive byte. 
 */ 
 while (SPI2STATbits.SPIRBF == 0); 
 

 /* Put the received byte in the buffer. 
 */ 
 bRx = SPI2BUF; 
  
 return bRx; 
 

}