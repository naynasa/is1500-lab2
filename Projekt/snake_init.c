/* mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015, 2017 by F Lundevall

   Latest update 2017-04-21 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "snake_defines.h"  /* Declatations for these labs */
#include <stdbool.h>

/*Given to us in the lab*/
void init_controller(){
	  /*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/
	SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0;  /* Lock OSCCON */
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
        SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;
}
/*Given to us in the lab*/
void display_init(void) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);
	
	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);
}


/*
Initializes timer 2 with timeout of 100ms 
when it times out it sets an interrupt bit to 1 and calls render_frame
*/
void init_timer(){
  //timer 2 interrupt enable and interrupt priority bits
  IEC(0) = IEC(0) | 0b0000000100000000;//set T2IE to 1 (Interrupt Enable Control bit in IEC0 interrupt register)
  IPC(2) = IPC(2) | 0b00000000000000000000000000011100;//set T2IP to ones (Interrupt Priority Control bits)
  
  //init Timer 2 - 16 bit timer
  T2CONCLR = T2CON_ENABLE_BIT; //stops the timer?
  uint32_t target_frequency = 60; //60 Hz / our timer timesout 10 times each second
  uint32_t prescaler = 256; //256 is needed since we need a low frequency 
  uint32_t pb_clock_frequency = 80E6;
  uint16_t period = pb_clock_frequency / (prescaler * target_frequency); //set our max value of the timer = 31250
  T2CON = T2CON_PRESCALER_BITS; //sets prescaler to 256 (see global definition)
  TMR2 = 0; //set the current number of ticks in our timer to 0
  PR2 = period; //set the max number of ticks equal to our period
  
  enable_interrupt();
}
//sets the LEDs on port E to output - 0 marks output
void init_LEDs(){
  volatile char* port_e_pointer = 0xbf886100; //TRISE
  *port_e_pointer = 0b00000000; //<=> 11111111 i bas 2 - sets 8 LSB of port E to output

}
//sets the Buttons and Switches on port D to output - 1 marks output
void init_buttons_switches(){
  volatile uint16_t* port_d_pointer = TRISD; //TRISD
  uint16_t mask = 0b1111100000001111; //<=> 0xf80f
  *port_d_pointer = *port_d_pointer | 0b1111100000001111; // 0bXXXXX0000000XXXX <=> ta bort de bitarna vi vill ändra
  *port_d_pointer += 0b0000011111110000; //add 1 to where we want 1:s - set input
  
}
