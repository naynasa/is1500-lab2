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

int mytime = 0x5957;
int num_ticks = 0;
char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  //p -> 0xbf886100
  //*p[0:7] = 1;
  volatile char* p = 0xbf886100; //TRISE
  *p = 0x00; //<=> 11111111 i bas 2 - sets 8 LSB of port E to output

  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  delay( 1000 );
  time2string( textstring, mytime );
  display_string( 3, textstring );
  display_update();
  tick( &mytime );
  num_ticks++; //increment ticks
  volatile char* p = 0xbf886110; //PORTE
  *p = num_ticks; //5 <=> 00000101 <=> 0x05
  display_image(96, icon);
}
