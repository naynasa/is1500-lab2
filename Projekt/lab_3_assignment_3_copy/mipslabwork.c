/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <stdbool.h>
int mytime = 0x5957;
uint32_t counter = 0; /*number of times timer 2 has been completed*/

char textstring[] = "text, more text, and even more text!";
#define T2CON_ENABLE_BIT 0x8000 //0b1000000000000000
#define T2CON_PRESCALER_BITS 0x0070//0b0000000001110000 TCKPS<2:0> - we only look at 3 bits (here 111) and 111 <=> 



/* T2IF (bit)- holds our interupt flag (bit 8 of IFS0 - its is 1 if timer is done else 0) */
/* T2IE (bit)- set to 1 to enable interrupts on timer 2 (bit 8 of IEC0)*/
/* T2IP (bits)- set to 1s to set timer 2 to highest priority (bit 20,19,18 of IPC2)*/
// TRISECLR sätter man en bit i den till 1 sätts motsvarande bit till 0 i TRISE (sätter man en bit till 0 händer inget)
// TRISESET 1 sätter värdet i TRISET 0 gör inget


/* Interrupt Service Routine - called when */
void user_isr( void ) {
    /*
    counter++;
    if(counter % 10 == 0){
      time2string( textstring, mytime );
      display_string( 3, textstring );
      display_update();
      tick( &mytime );
    }
    */
    //https://ww1.microchip.com/downloads/en/devicedoc/61143h.pdf page 53 table 4-4 IFS0 tells us its bit 8
    IFS(0) = IFS(0) ^ 0b0000000100000000; //set bit 8 to 0


}
/*
Initializes timer 2 with timeout of 100ms 
when it times out it sets an interrupt bit to 1 and calls user_isr
*/
void init_timer(){
  //timer 2 interrupt enable and interrupt priority bits
  IEC(0) = IEC(0) | 0b0000000100000000;//set T2IE to 1 (Interrupt Enable Control bit in IEC0 interrupt register)
  IPC(2) = IPC(2) | 0b00000000000000000000000000011100;//set T2IP to ones (Interrupt Priority Control bits)
  
  //init Timer 2 - 16 bit timer
  T2CONCLR = T2CON_ENABLE_BIT; //stops the timer?
  uint32_t target_frequency = 10; //10 Hz / our timer timesout 10 times each second
  uint32_t prescaler = 256; //256 is needed since we need a low frequency 
  uint32_t pb_clock_frequency = 80E6;
  uint16_t period = pb_clock_frequency / (prescaler * target_frequency); //set our max value of the timer = 31250
  T2CON = T2CON_PRESCALER_BITS; //sets prescaler to 256 (see global definition)
  TMR2 = 0; //set the current number of ticks in our timer to 0
  PR2 = period; //set the max number of ticks equal to our period
}
//starts timer 2 by setting the enable bits to high
void start_timer(){
  T2CONSET = T2CON_ENABLE_BIT;
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


void game_init( void )
{
  enable_interrupt(); //allows our program to use interrupts
  
  init_timer();
  start_timer();
  
  init_LEDs();
  init_buttons_switches();
  return;
}

/* This function is called repetitively from the main program */
void game_main( void )
{
  
  //uint8_t entire_screen[4096] = {0}; //set to all 0s
  //int i;
  //for (i = 0; i < sizeof(entire_screen)/sizeof(entire_screen[0]); i++)
  //{
  //  entire_screen[i] = 255; //make all pixels white
  //}
  uint8_t white_square[128] = {0}; //set to all 0s
  int i;
  for (i = 0; i < sizeof(white_square)/sizeof(white_square[0]); i++)
  {
    white_square[i] = 255; //make all pixels white
  }

  
  //display_image(96,white_square);
  display_image(96, icon);
  display_update();
}

