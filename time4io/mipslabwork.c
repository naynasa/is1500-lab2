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
#define T2CON_ENABLE_BIT 0x8000 //0b1000000000000000
#define T2CON_PRESCALER_BITS 0x0070//0b0000000001110000 TCKPS<2:0> - we only look at 3 bits (here 111) and 111 <=> 

int mytime = 0x5957;
int num_ticks = 0;
char textstring[] = "text, more text, and even more text!";
bool flag = false;
uint32_t count = 0;

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
  T2CONCLR = T2CON_ENABLE_BIT;
  uint32_t target_frequency = 10; //10 Hz / our timer timesout 10 times each second
  uint32_t prescaler = 256; //256 is needed since we need a low frequency 
  uint32_t pb_clock_frequency = 80E6;
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
    count++;
    time2string( textstring, mytime ); //pass by reference
      display_string( 3, textstring );
      display_update();

    if(count % 10 == 0){
      tick( &mytime );
      num_ticks++; //increment ticks
      volatile char* p = 0xbf886110; //PORTE
      *p = num_ticks; //5 <=> 00000101 <=> 0x05

    }
    display_image(96, icon);
  }
  //delay( 1000 );
  
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

