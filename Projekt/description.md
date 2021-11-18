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
A: "simple imperative" we have a bunch of 

> do we need a timer in the project?
probably, in order to coordinate when we should write to the screen

## tasks
1) make a square that moves smoothly across the screen

`display_image(x,img)` displays an image (uint8_t array) at the x value specified - can img be any length?