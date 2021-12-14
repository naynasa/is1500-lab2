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
#include <stdlib.h>
#include <math.h>
#include <string.h>


/*the array that stores all our pixel data - the way we communicate with the screen*/
bool frame_buffer[4][128][8]; //4*128 = 512 bytes (8 bit each)

#define BLOCK_SIZE 3 //varies size of all squares in the game (snake,apples,obstacles)
#define SPEED BLOCK_SIZE //amount of pixels the snake moves from the start - needs to be the same as BLOCK_SIZE for the snake to move smoothly
#define SCREEN_HEIGHT 32
#define SCREEN_WIDTH 128
#define MAX_NUMBER_OF_POSSIBLE_BLOCKS (SCREEN_HEIGHT*SCREEN_WIDTH)/(BLOCK_SIZE*BLOCK_SIZE)
#define SEED_ADDRESS 0b1
#define HIGH_SCORE_ADDRESS 0b10
#define FIRST_TIME_LAUNCH_ADDRESS 0b11

/*the basic building block of our game - a square of size BLOCK_SIZE with x0,y0 being the bottom left most pixel in the square*/
typedef struct {
  uint8_t x0; /*value range: 0-128, lower left hand corner x coordinate*/
  uint8_t y0; /*value range: 0-32, lower left hand corner y coordinate*/
//uint8_t size; /*value range: 1-32 max for uint8_t is 255, the size of the square*/

}Block;

/*the snake the user controlls*/
typedef struct {
  uint16_t num_blocks; /*number of blocks contained at the pointer location / that belong to the snake*/
  int num_apples_eaten; /*could be unsigned but noone is gonna collect over 2 million apples so we should be fine*/
  Direction facing_direction; /*the direction the snake is currently facing/moving in (if no user command is given it keeps going in that direction)*/
  Block blocks_array[MAX_NUMBER_OF_POSSIBLE_BLOCKS]; /*the larger squares that make up the snake - first block is the head (index 0) - we allocate the entire max size but only num_blocks actually exist in it rest is empty*/
  Block prev_tail; /*the tail block of the snake BEFORE MOVING - important since that's where we want to add the new block*/
}Snake;

/*since only one apple is active at the time we update the block values each time instead of creating a new apple*/
typedef struct {
  uint8_t x0; /*value range: 0-128, lower left hand corner x coordinate*/
  uint8_t y0; /*value range: 0-32, lower left hand corner y coordinate*/
}Apple;

/*declare global data variables - "instances" of our structs*/
Snake snake;
Apple apple;


int main(void) {
  
  /*helper that starts timer 2 by setting the enable bits to high*/
  void start_timer(){
    T2CONSET = T2CON_ENABLE_BIT;
  }
  /*our seed is stored in the EEPROM of the microcotroller - if this is the first time we run the program write to it - otherwise read the previous and increment it by 1*/
  int get_and_write_seed(){
    //read from our seed address
    uint8_t seed = read_byte_from_eeprom(SEED_ADDRESS);
    //increment it
    seed++;
    //write it back so its not the same next time we run the program
    write_byte_to_eeprom(SEED_ADDRESS,seed);

    return seed;
  } 
  
  


  void init_snake(){
    /*todo make random? - so you don't start at the same place each time*/
  
  snake.blocks_array[0] = (Block) {40,15};
  snake.blocks_array[1] = (Block) {40-1*BLOCK_SIZE,15};
  snake.blocks_array[2] = (Block) {40-2*BLOCK_SIZE,15};
  snake.blocks_array[3] = (Block) {40-3*BLOCK_SIZE,15};
  snake.blocks_array[4] = (Block) {40-4*BLOCK_SIZE,15};
  
  /*
  snake.blocks_array[5] = (Block) {40-5*BLOCK_SIZE,15};
  snake.blocks_array[6] = (Block) {40-6*BLOCK_SIZE,15};
  snake.blocks_array[7] = (Block) {40-7*BLOCK_SIZE,15};
  snake.blocks_array[8] = (Block) {40-8*BLOCK_SIZE,15};  
  snake.blocks_array[9] = (Block) {40-9*BLOCK_SIZE,15};
  snake.blocks_array[10] = (Block) {40-10*BLOCK_SIZE,15};
  snake.blocks_array[11] = (Block) {40-11*BLOCK_SIZE,15};
*/

  snake.facing_direction = 'R'; //set the snake to always start going right
  snake.num_blocks = 5;//sizeof(blocks) / sizeof(blocks[0]);
  snake.num_apples_eaten = 0;
  apple = (Apple) {128/2,15};
  snake.prev_tail = (Block) {40,15};
  }

  /*initializations*/ 
	init_controller();
	eeprom_init();
  display_init();
  
  init_timer();
  init_LEDs();
  init_buttons_switches();


  

  init_snake();
  
	start_timer();
  srand(get_and_write_seed()); //set seed random number generation used for apple placements

  
  
  

	while( 1 )
	{
	  /*our "game loop" just to keep the program alive - all calculations are done in render_frame()*/
	}
	return 0;
}


//each block is 32*128/4 = 4096/4 = 1024 bits
//we 0 index the blocks: block 0,1,2,3
/*returns a pointer of type bool to our x,y coordinate in the frame_buffer*/
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
      bool* pixel_pointer = pixel_to_frame_buffer_position(i,j);
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

/*makes the snake "slither" by first moving the head then moving each block into the position the following block was in before
also assigns snake_tail*/
void move_snake(){
  //moves the snakes head forward if no command is given or in another direction if the user presses one of the buttons
  //if the user tries to move 180 degrees from current direction or presses multiple buttons the command is ignored
  //called each frame
  //updates position of the snakes head block and snake.facing_direction
  void move_head(){
    /*helper that calculates how much x should be updated depending on the direction*/
    int x_offset_from_dir(Direction dir){
      if(dir == 'L' || dir == 'R'){
        return (dir == 'L') ? -SPEED : SPEED; // L = - and R = +
      }else{
        return 0;
      }
    }
    /*helper that calculates how much y should be updated depending on the direction*/
    int y_offset_from_dir(Direction dir){
      if(dir == 'U' || dir == 'D'){
        return (dir == 'U') ? -SPEED : SPEED; // U = - and D = +
      }else{
        return 0;
      }
    }
    /*helper that calculates if the user is trying to turn left when going right or up when going down etc. (180 degrees)*/
    bool user_turning_180_degrees(Direction dir){
      switch (snake.facing_direction){
      case 'U':
        return dir == 'D';
        
      case 'D':
        return dir == 'U';

      case 'L':
        return dir == 'R';
        
      case 'R':
        return dir == 'L';
    
      default:
        return false;
      }
    }
    Direction user_move_dir = user_move_direction();
    int i;
    if(user_move_dir == 'N' || user_turning_180_degrees(user_move_dir)){
      /*no command or invalid command - just move the snake forward*/
      int x_add = x_offset_from_dir(snake.facing_direction);
      int y_add = y_offset_from_dir(snake.facing_direction);
      
      //update the head coordinates
      snake.blocks_array[0].x0 += x_add;
      snake.blocks_array[0].y0 += y_add;

    }else{
      /*a valid command is sent - move the snake in the direction the user wants*/

      int x_add = x_offset_from_dir(user_move_dir);
      int y_add = y_offset_from_dir(user_move_dir);
      
      //update the head coordinates
      snake.blocks_array[0].x0 += x_add;
      snake.blocks_array[0].y0 += y_add;

      //update the direction the snake is facing
      snake.facing_direction = user_move_dir;
    }
    

  }
  //copy snake blocks at t-1 - we're not interested in the entire snake just the blocks so copy those
  Block old_blocks[snake.num_blocks];// = snake.blocks_array;

  int j;
  for (j = 0; j < snake.num_blocks; j++)
  {
   old_blocks[j] = snake.blocks_array[j];
  }

  //update prev_tail
  snake.prev_tail = old_blocks[snake.num_blocks-1];
 
  
  //old_blocks_array = snake.blocks_array;
  
  
  //move the head
  move_head();

  //put all other blocks at position i-1 from the snake at t-1
  int i;
  for (i = 1; i < snake.num_blocks; i++){ //the head is at index 0
    snake.blocks_array[i].x0 = old_blocks[i-1].x0; //block 1 goes to the prev. head pos, block 2 goes to block 1 prev. etc.
    snake.blocks_array[i].y0 = old_blocks[i-1].y0;
  }
  

}
/*called when the player loses the game - just infinitly prints game over, the score and the high score*/
void game_over(){
  set_all_pixels_black();
  display_buffer();
  
  
  //check if its the first time we run the game is so set high score to 0
  uint8_t first_time_run = read_byte_from_eeprom(FIRST_TIME_LAUNCH_ADDRESS);
  if(first_time_run != 0xFF){
    //this is the first time we run the program
    write_byte_to_eeprom(FIRST_TIME_LAUNCH_ADDRESS,0xFF);//set first_time_run
    write_byte_to_eeprom(HIGH_SCORE_ADDRESS,0);//set high score
  }
  //read highscore
  uint8_t high_score = read_byte_from_eeprom(HIGH_SCORE_ADDRESS);
  

  char score_string[18]; //holds the score string
  char high_score_string[18]; //holds the score string
  sprintf(score_string, "Score: %d!",snake.num_apples_eaten); //format the score string
  sprintf(high_score_string, "High Score: %d!",snake.num_apples_eaten > high_score ? snake.num_apples_eaten : high_score); //format the score string
  bool new_high_score = false;
  //if we beat the high score update it
  if(snake.num_apples_eaten > high_score){
    write_byte_to_eeprom(HIGH_SCORE_ADDRESS,snake.num_apples_eaten);
    new_high_score = true;
  }
  
  while (true)
  {
      
      display_string(0, "game over!");
      display_string(1, score_string);
      display_string(2, high_score_string);
      if(new_high_score){
        display_string(3,"yay, new high score!");
      }
      
    
      
      //display_string(3, "highscore: %d",snake.num_apples_eaten);

  }
  
}
/*function that "eats" the apple - moving the apple looks the same as creating a new one
also makes snake longer and increments snake.num_apples_eaten*/
void eat_apple(){

  //increment apples eaten
  snake.num_apples_eaten++;
  //move apple
  
  uint8_t apple_new_x = BLOCK_SIZE + rand() % (SCREEN_WIDTH-BLOCK_SIZE); //scaled_rand(SCREEN_WIDTH-BLOCK_SIZE);
  uint8_t apple_new_y = BLOCK_SIZE + rand() % (SCREEN_HEIGHT-BLOCK_SIZE);//scaled_rand(SCREEN_HEIGHT-BLOCK_SIZE);
  
  apple.x0 = apple_new_x;
  apple.y0 = apple_new_y;
  //apple->block = (Block) {apple_new_x,apple_new_y};
  //apple.*block = (Block) {apple_new_x,apple_new_y};
  //apple.*block.y0 = apple_new_y;
  
  //make sure new apple position isn't on the snakes tail

  snake.num_blocks++;
  snake.blocks_array[snake.num_blocks-1] = snake.prev_tail;
}
/*checks if the head of the snake has collided with anything, such as an apple, the wall or its own tail*/
void check_collision(){
  //helper that returns whether or not the pixel we are checking is outside the screen
  bool check_outside_screen(uint16_t x, uint16_t y){
    return (x>= SCREEN_WIDTH || x<= 0 || y>=SCREEN_HEIGHT || y<=0);
  }
  /*helper that returns whether or not we collided with our tail*/
  bool check_tail(uint16_t x, uint16_t y){
    int i;
    //iterate through each block and see if x0,y0 are the same as those of the head - in that case we collided
    for (i = 1; i < snake.num_blocks; i++){
      if(x == snake.blocks_array[i].x0 && y == snake.blocks_array[i].y0){
        return true;
      }
    }
    return false;
    
  }
  /*returns whether or not the head is on an apple*/
  bool is_apple(uint16_t x, uint16_t y){
    return  x >= apple.x0 &&  x< (apple.x0+BLOCK_SIZE) && y >= apple.y0 && y < (apple.y0 + BLOCK_SIZE);
  }
  uint16_t i,j,x,y;
  for(i = 0; i< BLOCK_SIZE; i++){
    for(j = 0; j<BLOCK_SIZE; j++){
      x = snake.blocks_array[0].x0 + i;
      y = snake.blocks_array[0].y0 - j;

      bool head_is_outside_screen = check_outside_screen(x,y);
      bool head_collided_with_tail = check_tail(x,y);
      if(is_apple(x,y)){
          eat_apple();
      }else if(head_is_outside_screen || head_collided_with_tail){
        game_over();
      }
    }
  }
      

  
}


/*Render a new frame - called when timer ticks over*/
void render_frame() {
  /*helper function to reset the isr via bit manipulation*/
  void reset_isr(){
    IFS(0) = IFS(0) ^ 0b0000000100000000; //set bit 8 to 0
  }
  /*"renders" the snake - sends it to the frame buffer*/
  void render_snake(){
    int i;
    //send the snake to the buffer - render snake
    for(i = 0; i<snake.num_blocks; i++){
      //iterates over each block in the snake
      Block pointed_block = snake.blocks_array[i];
      add_square(pointed_block.x0, pointed_block.y0, BLOCK_SIZE);
    }
  }
  /*"renders" the snake - sends it to the frame buffer*/
  void render_apple(){
    //send the apple to the buffer
    add_square(apple.x0,apple.y0,BLOCK_SIZE); //write the apple
  }

  
  set_all_pixels_black();  
  
  
  move_snake();
  check_collision();
  
 
  render_snake();
  render_apple();
  

  
  
  display_buffer();
  
  reset_isr();

}
