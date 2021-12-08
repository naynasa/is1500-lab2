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
#include <stdio.h>
/*the array that stores all our pixel data - the way we communicate with the screen*/
bool frame_buffer[4][128][8]; //4*128 = 512 bytes (8 bit each)
int square_x_value = 0;

#define BLOCK_SIZE 3 //varies size of all squares in the game (snake,apples,obstacles)
#define BASE_SPEED 3 //amount of pixels the snake moves from the start
#define SCREEN_HEIGHT 32
#define SCREEN_WIDTH 128

/**/
typedef struct {
  uint8_t x0; /*value range: 0-128, lower left hand corner x coordinate*/
  uint8_t y0; /*value range: 0-32, lower left hand corner y coordinate*/
  //uint8_t size; /*value range: 1-32 max for uint8_t is 255, the size of the square*/

}Block;

/**/
typedef struct {
  uint16_t num_blocks; /*number of blocks contained at the pointer location / that belong to the snake*/
  int num_apples_eaten; /*could be unsigned but noone is gonna collect over 2 million apples so we should be fine*/
  Direction facing_direction; /*the direction the snake is currently facing/moving in (if no user command is given it keeps going in that direction)*/
  Block* blocks_pointer; /*the larger squares that make up the snake - last block is the head*/
}Snake;

/*since only one apple is active at the time we update the block values each time instead of creating a new apple*/
typedef struct {
  Block block; /*the square that symbolizes the apple*/
}Apple;

/*declare global data variables*/
Snake snake;
Apple apple;

int main(void) {
  /*helper that starts timer 2 by setting the enable bits to high*/
  void start_timer(){
    T2CONSET = T2CON_ENABLE_BIT;
  }

  /*initializations*/ 
	init_controller();
	display_init();
  init_timer();
  init_LEDs();
  init_buttons_switches();

  /*todo make random - so you don't start at the same place each time*/

  Block blocks[] = {{10,15}, {10,15-BLOCK_SIZE}, {10,15-2*BLOCK_SIZE}};
  snake.blocks_pointer = blocks;
  snake.facing_direction = 'U'; //set the snake to always start going up
  snake.num_blocks = 3;//sizeof(blocks) / sizeof(blocks[0]);
  snake.num_apples_eaten = 0;
  apple.block = (Block) {100,10};

  
	start_timer();
	//display_image(96, icon);


	while( 1 )
	{
	  game_main(); /* Do lab-specific things again and again */
	}
	return 0;
}

/*
eat_apple
check_collide - kollar om han kolliderat med väggen eller sin svans
move_snake
game_over
display_highscore
*/



//each block is 32*128/4 = 4096/4 = 1024 bits
//we 0 index the blocks: block 0,1,2,3
bool* pixel_to_frame_buffer_position(int x, int y){
  int page = y / 8; // since we have 4 blocks of 8 height each
  int byte_index = x;
  int bit_index = y % 8; //each byte is 8 bits so modulo gives us how "far into" the block we are

  return &frame_buffer[page][byte_index][bit_index];
}

/*x,y mark starting points of the square (lower left hand corner)*/
void add_square(int x, int y, int size){
  //max size is 32 (our max y value)

  //set all pixels with x values between x and x+size-1 and y values between y and y-size+1 to 1
  int i,j;
  for(i = x; i <= x + size -1; i++){
    for(j = y-size + 1; j <= y; j++){
      bool *pixel_pointer = pixel_to_frame_buffer_position(i,j);
      *pixel_pointer = true;

    }
  }
}

/*sets all values in frame_buffer to 0 and updates the screen*/
void set_all_pixels_black(){
  int i,j,k;
  for(i = 0; i < 4; i++){
    for(j = 0 ; j<128 ;j++){
      for(k = 0; k<8; k++ ){
        frame_buffer[i][j][k] = 0;
      }
    }
  }
    display_buffer();
}


/* Our game loop - This function is called repetitively from the main program 
we imagine each pixel has an x,y value with 0,0 being in the bottom left corner
*/
void game_main( void ){

//wait_x_ms()
//calculate_frame();

}


//moves the snakes head forward if no command is given or in another direction if the user presses one of the buttons
//called each frame
//updates position of the snakes head block and snake.facing_direction
void move_head(){
  /*helper that calculates how much x should be updated depending on the direction*/
  int x_offset_from_dir(Direction dir){
    if(dir == 'L' || dir == 'R'){
      return (dir == 'L') ? -BASE_SPEED : BASE_SPEED; // L = - and R = +
    }else{
      return 0;
    }
  }
  /*helper that calculates how much y should be updated depending on the direction*/
  int y_offset_from_dir(Direction dir){
    if(dir == 'U' || dir == 'D'){
      return (dir == 'U') ? -BASE_SPEED : BASE_SPEED; // U = - and D = +
    }else{
      return 0;
    }
  }
  
  Direction user_move_dir = user_move_direction();
  int i;
  if(user_move_dir == 'N'){
    /*no command - just move the snake forward*/
    int y_add = x_offset_from_dir(snake.facing_direction);
    int x_add = y_offset_from_dir(snake.facing_direction);
    
    //update the head coordinates
    snake.blocks_pointer[0].x0 += x_add;
    snake.blocks_pointer[0].y0 += y_add;

    snake.blocks_pointer[0].x0 %= SCREEN_WIDTH;
    snake.blocks_pointer[0].y0 %= SCREEN_HEIGHT;

  }else{
    /*command is sent - move the snake in that direction*/
    int x_add = x_offset_from_dir(user_move_dir);
    int y_add = y_offset_from_dir(user_move_dir);
    
    //update the head coordinates
    snake.blocks_pointer[0].x0 += x_add;
    snake.blocks_pointer[0].y0 += y_add;

    snake.blocks_pointer[0].x0 %= SCREEN_WIDTH;
    snake.blocks_pointer[0].y0 %= SCREEN_HEIGHT;

    //update the direction the snake is facing
    snake.facing_direction = user_move_dir;
  }
  

}
//makes the snake "slither" by first moving the head then moving each block into the position the following block was in before
void move_snake(){
  //copy snake blocks at t-1
  Snake old_snake;
  memcpy(old_snake, snake, sizeof(snake));
  
  //move the head
  move_head();

  //put all other blocks at position i-1 from the snake at t-1
  int;
  for (i = 1; i < snake.num_blocks; i++){ //the head is at index 0
    snake.blocks_pointer[i].x0 = old_snake.blocks_pointer[i-1].x0; //block 1 goes to the prev. head pos, block 2 goes to block 1 prev. etc.
    snake.blocks_pointer[i].y0 = old_snake.blocks_pointer[i-1].y0;
  }
  //delete the copy
  free(old_snake);
  

}

/*Render a new frame - called when timer ticks over*/
void render_frame() {
  /*helper function to reset the isr via bit manipulation*/
  void reset_isr(){
    IFS(0) = IFS(0) ^ 0b0000000100000000; //set bit 8 to 0
  }
    
  set_all_pixels_black();  
  int i;
  

  move_snake();
  
  
  
  //send the snake to the buffer
  for(i = 0; i<snake.num_blocks; i++){
    //iterates over each block in the snake
    Block pointed_block = snake.blocks_pointer[i];
    add_square(pointed_block.x0, pointed_block.y0, BLOCK_SIZE);
  }
  

  //send the apple to the buffer
  add_square(apple.block.x0,apple.block.y0,BLOCK_SIZE); //write the apple
  
 /*
 //{{10,15}, {10,15-BLOCK_SIZE}, {10,15-2*BLOCK_SIZE}}
  add_square(10,15,BLOCK_SIZE);
  add_square(10,15-BLOCK_SIZE,BLOCK_SIZE);
  add_square(10,15-2*BLOCK_SIZE,BLOCK_SIZE);
  */
  display_buffer();

  reset_isr();

}
