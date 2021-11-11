#include <stdint.h>   
#include <pic32mx.h>  
#include "mipslab.h" 

///deals with SW4,SW3,SW2 and SW1
///these switches correspond to Port D bit 11,10,9,8
///SW4 <=> bit 11, SW3 <=> bit 10, SW2 <=> bit 9, SW1 <=> bit 8
///000000...0ABCD, D <=> is_flicked(SW1), C <=> is_flicked(SW2) etc.
///4 LS bits are the values of SW4 through SW1
int getsw(){
    //port_d[8],port_d[9],port_d[10 ],port_d[11]
    uint16_t* d_pointer = PORTD; 
    uint16_t d_bits = *d_pointer;//0bXXXXXABCDXXXXXX   = d_bits
    uint16_t masked_d_value = d_bits & 0b0000111100000000;  //0b0000ABCD00000000  = masked_d_value
    int value = 0;
    return value + (masked_d_value >> 8); //0b000000000000ABCD = (masked_d_value >> 8)   
}


//3 LS bits of return value is:
//   BTN4,BTN3,BTN2
//bit 7,   6,   5 of port D (7<=> BTN4 etc.)
//i.e. 0000....00ABC, A<=> BTN4, B <=> BTN3, C<=>BTN2
int getbtns(void){
    uint16_t* d_pointer = PORTD;
    uint16_t d_bits = *d_pointer;//0bXXXXXXXABCXXXXX
    uint16_t masked_d_value = d_bits & 0b0000000011100000;  //0b00000000ABC00000  = masked_d_value
    int value = 0; 
    return value + (masked_d_value >> 5); //0b0000000000000ABC = (masked_d_value >> 5)
}