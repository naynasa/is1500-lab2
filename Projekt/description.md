## requirements basic project
Display game on built-in OLED graphical display.
- Snake moves across the screen in X and Y directions.
- Snake chases apples, which, when eaten, increases the length of the snake.
- Snake can collide with itself and walls.
- Detect when snake collides, and display number of apples eaten (score).

## requirements advanced project
- [Graphics] (see above). Note: In usual snake games, the snake moves more than one pixel in each frame update. To fulfill [Graphics], make sure that the snake moves (or is animated) pixel-by-pixel.
- There are both one and two-player modes, where the one-player plays against the computer (controlled by an AI). There are different difficulty levels for the AI.
- There are different difficulty levels, where there are various amounts and kinds of obstacles in the game world.
- [Highscore] (see above)

## questions / planning / ideas
>how do we structure the code? what paradigms to use?
A: "simple imperative" we have a bunch of functions and our data somewhat structured which calls these functions

> do we need a timer in the project?
probably, in order to coordinate when we should write to the screen

## tasks
1) make a square that moves smoothly across the screen

`display_image(x,img)` displays an image (uint8_t array) at the x value specified - can img be any length?

## useful info
Organic LED Graphic Display 
This is a 128x32 pixel monochrome OLED graphic display panel is accessed using the SPI interface.
https://digilent.com/reference/_media/chipkit_shield_basic_io_shield:chipkit_basic_io_shield_rm.pdf page 4

the display is monochrome - each pixel is either 0 or 1

our chipkit talks to the OLED display on the  basic i/o shield via SPI (SERIAL PERIPHERAL INTERFACE)


## data flow organization (ideas along the way)
snake_main is responsible for changing the frame_buffer in such a way that it represents the game being played - e.g. placing apples, growing the snake etc. (the game logic)
-the screen is divided into a grid of x,y values with 0,0 being at the bottom left

snake_init is responsible for initializing everything we need to play the game (timers, ports, buttons etc.) as well as what we need to talk to the display

display_interface is responsible for writing the frame_buffer to the screen

each frame all the pixels are redrawn

## data we need
snake - thickness, start and end / blocks ,
apple - position,
number_of_apples_eaten 
### advanced
obsticle
difficulty
snake_speed
ai_snake
## game functions we need
eat_apple
check_collide - kollar om han kolliderat med väggen eller sin svans
move_snake
game_over
display_highscore
### advanced
ai_plays
increase_snake_speed
place_obsticle

we use positive y direction downwards and positive x direction to the right (see drawing)


we need all 4 buttons and button 1 is on pin 4 (not close to the others) - we first look in here https://digilent.com/reference/_media/chipkit_shield_basic_io_shield:chipkit_basic_io_shield_rm.pdf 2.2 page 5 and see that button 1 is pin 4 - we then check https://digilent.com/reference/chipkit_uno32/refmanual pinout table and look in the "chipkit pin #" column - there we find that pin 4 is bit 1 of port F
