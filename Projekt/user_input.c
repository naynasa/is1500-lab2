#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "snake_defines.h"  /* Declatations for these labs */
#include <stdbool.h>
#include <stdio.h>

/*returns what direction the user wants to move in based on which buttons are pressed
- if more than one button is pressed the snake moves nowhere and continues on its current path
- Left <=> 'L', Up <=> 'U', Right <=> 'R', Down <=> 'D', None <=> 'N'
- BTN4 <=> left, BTN1 <=> right, BTN3 <=> up, BTN2 <=> down
*/
