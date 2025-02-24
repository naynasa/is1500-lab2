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
#include <signal.h> 
#define T2CON_ENABLE_BIT 0x8000 //0b1000000000000000
#define T2CON_PRESCALER_BITS 0x0070//0b0000000001110000 TCKPS<2:0> (bits 4,5,6) - we only look at 3 bits (here 111) and 111 <=> prescaler of 256

//T2 CON is the control register for timer 2 (settings etc.)
//TCKPS 3 bits that control the prescaler

int mytime = 0x5957;
int num_ticks = 0;
char textstring[] = "text, more text, and even more text!";
uint32_t count = 0; //unsigned in order to make it go back to 0 after reaching max value - max value is reached after 429496729.5 sec or around 13.6 years - howerver overflows are not a problem since its unsigned
// if you for some reason really wanted to count every occurence you could make it uint64_t then it could store values for ca 29 billion years before overflow
/* Interrupt Service Routine */
void user_isr( void )
{
  
}

/* Lab-specific initialization goes here */
//setting TRISn, where n is the port letter e.g. A,B,C etc. to:
//1 marks input
//0 mars output

void labinit( void )
{
  //init Timer 2 - 16 bit timer
  T2CONCLR = T2CON_ENABLE_BIT; //T2CONCLR clears selected bits in T2CON
  uint32_t target_frequency = 10; //10 Hz / our timer timesout 10 times each second
  uint32_t prescaler = 256; //256 is needed since we need a low frequency 
  uint32_t pb_clock_frequency = 80E6; //clock speed of the chipkit
  uint16_t period = pb_clock_frequency / (prescaler * target_frequency); //set our max value of the timer = 31250
  T2CON = T2CON_PRESCALER_BITS; //sets prescaler to 256 (see global definition)
  TMR2 = 0; //set the current number of ticks in our timer to 0
  PR2 = period; //set the max number of ticks equal to our period


  T2CONSET = T2CON_ENABLE_BIT;

  //target_frequency = 80e6 / (256 * 65535) =
  //perioden är mellan 1-65535 för 16-bitar
  
  
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
      int maskning = mytime & 0b00000000000000000000111111111111; //0x0fff
      mytime = maskning + (switch_status << 12);

      //switch_status 0000...0XYZW e.g. 0000.00100

      //mytime = 000...0EEEEFFFFGGGGHHHH
      //                       +
      //         000000000000XYZW = switch_status

     
    }

    if(btn_3_status != 0){
      int maskning = mytime & 0b00000000000000001111000011111111;
      mytime = maskning + (switch_status << 8);
  

    }
    if(btn_2_status != 0){
      int maskning = mytime & 0b00000000000000001111111100001111;
      mytime = maskning + (switch_status << 4);

    }

    
    
  }


  if(TMR2 == PR2){ //check if our timer is "full" / has ended
    //TMR2  = 0; //no need to actually set it to 0 since its unsigned so implicitly turns to 0 after overflowing
    //once every 100ms
    count++;
    time2string( textstring, mytime ); //pass by reference
    display_string( 3, textstring );
    display_update();

    if(count % 10 == 0){
      //once every 1 second
      tick( &mytime );
      num_ticks++; //increment ticks
      volatile char* p = 0xbf886110; //PORTE
      *p = num_ticks; //5 <=> 00000101 <=> 0x05

    }
    display_image(96, icon);
  }
  //delay( 1000 );
  
}
//QUESTIONS
//1)N/A
//2)N/A
//3)T2CONCLR, T2CON, TMR2, PR2, T2CONSET in init
//4) yes it changes quickly since we make a call to display (update) the string every 100ms - this is achieved
// by moving the code out of the nested if statement