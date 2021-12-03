/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "snake_defines.h"  /* Declatations for these labs */
#include <stdbool.h>

int main(void) {
  /*initializations*/ 
	init_controller();
	display_init();
  init_timer();
  init_LEDs();
  init_buttons_switches();

	
	//display_image(96, icon);

	while( 1 )
	{
	  game_main(); /* Do lab-specific things again and again */
	}
	return 0;
}


//starts timer 2 by setting the enable bits to high
void start_timer(){
  T2CONSET = T2CON_ENABLE_BIT;
}



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


/* Our game loop - This function is called repetitively from the main program 
we imagine each pixel has an x,y value with 0,0 being in the bottom left corner
*/
void game_main( void ){
size_t i,j,k;
for(i = 0; i < 4; i++){
  for(j = 0 ; j<128 ;j++){
      for(k = 0; k<8; k++ ){
        if(i == 0){
          frame_buffer[i][j][k] = 1
        }else{
          frame_buffer[i][j][k] = 0
        }
      }
  }
}
display_buffer();
//wait_x_ms()
//calculate_frame();

  

}
/*x,y mark starting points of the square (lower left hand corner)*/
void add_square(int x, int y, int size){

}

void pixel_to_frame_buffer_position(int x, int y){

}

