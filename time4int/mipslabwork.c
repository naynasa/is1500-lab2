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
int num_ticks = 0;
int counter = 0; /*number of times timer 2 has been completed*/
char textstring[] = "text, more text, and even more text!";

int prime = 1234567;
/* T2IF (bit)- holds our interupt flag (bit 8 of IFS0 - its is 1 if timer is done else 0) */
/* T2IE (bit)- set to 1 to enable interrupts on timer 2 (bit 8 of IEC0)*/
/* T2IP (bits)- set to 1s to set timer 2 to highest priority (bit 20,19,18 of IPC2)*/

/* Interrupt Service Routine */
void user_isr( void ) 
{
  counter++;
  if(counter % 10 == 0){
  time2string( textstring, mytime );
  display_string( 3, textstring );
  display_update();
  tick( &mytime );
  }
  //https://ww1.microchip.com/downloads/en/devicedoc/61143h.pdf page 53 table 4-4 IFS0 tells us its bit 8
  IFS(0) = IFS(0) ^ 0b0000000100000000 //set bit 8 to 0
}

/* Lab-specific initialization goes here */
//setting TRISn, where n is the port letter e.g. A,B,C etc. to:  
//1 marks input
//0 mars output
void labinit( void )
{
  //init Timer 2 - 16 bit timer
  T2CONCLR = T2CON_ENABLE_BIT;
  uint32_t target_frequency = 10; //10 Hz / our timer timesout 10 times each second
  uint32_t prescaler = 256; //256 is needed since we need a low frequency 
  uint32_t pb_clock_frequency = 80E6;
  uint16_t period = pb_clock_frequency / (prescaler * target_frequency); //set our max value of the timer = 31250
  T2CON = T2CON_PRESCALER_BITS; //sets prescaler to 256 (see global definition)
  TMR2 = 0; //set the current number of ticks in our timer to 0
  PR2 = period; //set the max number of ticks equal to our period

  IEC(0) = IEC(0) | 0b0000000100000000;//set T2IE to 1
  IPC(2) = IPC(2) | 0b00000000000111000000000000000000;//set T2IP to ones

  enable_interrupt();

  T2CONSET = T2CON_ENABLE_BIT; //start the timer
  
  //p -> 0xbf886100
  //*p[0:7] = 1;
  volatile char* port_e_pointer = 0xbf886100; //TRISE
  *port_e_pointer = 0b00000000; //<=> 11111111 i bas 2 - sets 8 LSB of port E to output

  volatile uint16_t* port_d_pointer = TRISD; //TRISD
  uint16_t mask = 0b1111100000001111; //<=> 0xf80f
  *port_d_pointer = *port_d_pointer | 0b1111100000001111; // 0bXXXXX0000000XXXX <=> ta bort de bitarna vi vill ändra
  *port_d_pointer += 0b0000011111110000; //add 1 to where we want 1:s - set input
  
  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  prime = nextprime( prime );
  display_string( 0, itoaconv( prime ) );
  display_update();
}
// TRISECLR sätter man en bit i den till 1 sätts motsvarande bit till 0 i TRISE (sätter man en bit till 0 händer inget)
// TRISESET 1 sätter värdet i TRISET 0 gör inget

//getbtns och getsw hamnar i register $v0? pga det är ett returvärde från en funktion och vi har bara ett


//1)gå till https://ww1.microchip.com/downloads/en/devicedoc/chipkit%20basic%20io%20shield_rm.pdf sid 8
// och hitta knappen/switchen vi vill ha
//2)kolla pin number under uno32
//3)https://electrathonoftampabay.org/www/Documents/uno32/chipKIT%20Uno32_rm.pdf sid 12 - kolla pic32 signal för pinnen vi hittade i 2)
//4)tolka resultatet t.ex. RD7 <=> bit 7 av port D
//(5) om vi inte vet var port D ligger kollar vi i https://github.com/sergev/pic32sim/blob/master/pic32mx.h så hittar vi minnesadressen för port D

