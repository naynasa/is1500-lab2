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
char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
//setting TRISn, where n is the port letter e.g. A,B,C etc. to:
//1 marks input
//0 mars output
void labinit( void )
{
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
  int button_status = getbtns(); //0000...0ABC
  if(button_status != 0){
    bool btn_4_status =  button_status >> 2; //000.0A
    bool btn_3_status =  (button_status >> 1) - (btn_4_status << 1); ///000.00B
    bool btn_2_status =  button_status -(btn_4_status << 2)- (btn_3_status << 1); ///000.00C

    int switch_status = getsw();
    if(btn_4_status != 0){
      int maskning = mytime & 0b000000000000000000000000111111111111;
      mytime = maskning + (switch_status << 12);

      //switch_status 0000...0XYZW e.g. 0000.00100

      //mytime = 000...0EEEEFFFFGGGGHHHH
      //                       +
      //         000000000000XYZW = switch_status

     
    }

    if(btn_3_status != 0){
      int maskning = mytime & 0b000000000000000000001111000011111111;
      mytime = maskning + (switch_status << 8);
  

    }
    if(btn_2_status != 0){
      int maskning = mytime & 0b000000000000000000001111111100001111;
      mytime = maskning + (switch_status << 4);

    }

    
    
  }
  delay( 1000 );
  time2string( textstring, mytime ); //pass by reference
  display_string( 3, textstring );
  display_update();
  //tick( &mytime );
  num_ticks++; //increment ticks
  volatile char* p = 0xbf886110; //PORTE
  *p = num_ticks; //5 <=> 00000101 <=> 0x05
  display_image(96, icon);
}
