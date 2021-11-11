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
#include <stdbool.h>  /* Declarations for bool datatype */
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
//0 marks output
void labinit( void )
{
  //p -> 0xbf886100
  //*p[0:7] = 1;
  volatile char* port_e_pointer = 0xbf886100; //TRISE
  *port_e_pointer = 0b00000000; // sets 8 LSB of port E to output

  //the following sets all bits of interest to 1 and maintains the value of all other bits
  volatile uint16_t* port_d_pointer = TRISD; //TRISD
  uint16_t mask = 0b1111000000011111; //<=> 0xf80f
  *port_d_pointer = *port_d_pointer & mask; // 0bXXXX0000000XXXXX <=> ta bort de bitarna vi vill ändra
  *port_d_pointer += 0b0000111111100000; //add 1 to where we want 1:s - set input
  
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
    if(btn_4_status == true){
      int maskning = mytime & 0b00000000000000000000111111111111; //0x0fff
      mytime = maskning + (switch_status << 12);
      //0b0000000000000000ABCD000000000000
      //0b00000000000000000000XXXXXXXXXXXX

      //switch_status 0000...0XYZW e.g. 0000.00100

      //mytime = 000...0EEEEFFFFGGGGHHHH
      //                       +
      //         000000000000XYZW = switch_status

     
    }

    if(btn_3_status == true){
      int maskning = mytime & 0b00000000000000001111000011111111;
      mytime = maskning + (switch_status << 8);
      //0b00000000000000000000ABCD00000000
      //0b0000000000000000XXXX0000XXXXXXXX

    }
    if(btn_2_status == true){
      int maskning = mytime & 0b00000000000000001111111100001111;
      mytime = maskning + (switch_status << 4);
      //0b000000000000000000000000ABCD0000
      //0b000000000000XXXXXXXXXXXX0000XXXX
    }

    
    
  }
  delay( 1000 );
  time2string( textstring, mytime ); //pass by reference (textstring is also a pointer to the first element in the char array)
  display_string( 3, textstring );
  display_update();
  tick( &mytime );
  num_ticks++; //increment ticks
  volatile char* p = 0xbf886110; //PORTE
  *p = num_ticks; //5 <=> 00000101 <=> 0x05
  display_image(96, icon);
}
// TRISECLR sätter man en bit i den till 1 sätts motsvarande bit till 0 i TRISE (sätter man en bit till 0 händer inget) (bör inte läsas)
// TRISESET 1 sätter värdet i TRISET 0 gör inget (bör inte läsas)

//getbtns och getsw hamnar i register $v0?(32 bitar och därav använder vi bara $v0) pga det är ett returvärde från en funktion och vi har bara ett


//reference manual: https://digilent.com/reference/_media/chipkit_uno32:chipkit_uno32_rm.pdf
//full text: https://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf
//1)vi kollar i reference manual table 3.7.1
//2)vi kollar på en viss chipkit pin vilken står skriven under den fysiska komponenten t.ex. BTN2 har chipkit pin 34 och
//korresponderande pic32 signal är "RD5"
//detta betyder att det är 5:e biten av PORT D (vilket överrensstämmer med labbyttrelsen)