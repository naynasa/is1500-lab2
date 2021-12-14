/* mipslabdata.c
   This file compiled 2015 by F Lundevall
   from original code written by Axel Isaksson

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "snake_defines.h"  /* Declatations for these labs */




/* mipslabfunc.c
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   For copyright and licensing, see file COPYING */



/* quicksleep:
   A simple function to create a small delay.
   Very inefficient use of computing resources,
   but very handy in some special cases. */
void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}


uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}



void display_string(int line, char *s) {
	int i, j, k;
	if(line < 0 || line >= 4)
		return;
	if(!s)
		return;
	
	for(i = 0; i < 16; i++)
		if(*s) {
			textbuffer[line][i] = *s;
			s++;
		} else
			textbuffer[line][i] = ' ';


  //display the string
  
	int c;
	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		
		DISPLAY_CHANGE_TO_DATA_MODE;
		
		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;
			
			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

//the display is 128x32 pixels each pixel is either 0 or 1

//example: 96,const uint8_t [128]

//we use a 512 byte buffer (the screen is 512 bytes = 512*8 = 128*32 = 4096 pixels / bits)
//displayen har 4 sidor med 128 bytes var
/*
void display_buffer(int x) {
	int i, j;
	//TODO calculate byte difference
	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;

		spi_send_recv(0x22); //set page command
		spi_send_recv(i);//0,1,2,3 <=> page number

		spi_send_recv(x & 0xF); //4 MS bitarna av x
		spi_send_recv(0x10 | ((x >> 4) & 0xF)); //0b10000 OR 000000..x[7,6,5,4] = 000..01ABCD A<=>x[7], B<=>x[6]
		
		DISPLAY_CHANGE_TO_DATA_MODE;
		
		for(j = 0; j < 32; j++)
			spi_send_recv(~screen_buffer[i*32 + j]);
	}
}

*/
/*Helper function unique to this file - converts an array of bools to a uint8_t*/
uint8_t bit_array_to_uint8(bool *ptr){
    uint8_t ret = 0;
    int count = 8;//always 8 bits in a byte - sizeof(arr)/sizeof(arr[0]);
    int i;
    for (i = 0; i < count; i++) {
        ret = ret | ( *ptr << i);
        ptr += 1;
    }

    return ret;
}
//iterates through each page (4 of them)
// in each page iterate through each byte of the page (128 of them)
//in each byte iterate through each bit (8 of them)
//in total 4 pages with 128 bytes = 4*16 = 512 bytes 
void display_buffer() {
	int i,j,k,m,p;

    //uint8_t (*byte_pointer)[8] = frame_buffer; //*byte_pointer är en adress **byte_pointer ger bit 0 av byten i fråga
	for(i = 0; i < 4; i++) {//loops pages
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22); //set page command
		spi_send_recv(i); //0,1,2,3 <=> page number
		
		spi_send_recv(0x0); //set low nibble of column
		spi_send_recv(0x10); // set hight nibble of column
		
		DISPLAY_CHANGE_TO_DATA_MODE; //"PORTSetBits(prtDataCmd, bitDataCmd);"
		
		/*write each byte*/
        for(j = 0; j<128; j++){
            
            bool *first_pixel_pointer = frame_buffer[i][j];
           
            
            //convert our 8 bits in memory to an uint8 to be able to write
            uint8_t pixel_byte = bit_array_to_uint8(first_pixel_pointer);
            
            
            spi_send_recv(pixel_byte);//0xFF -> alla vita -alltså rå byte värde här funkar
            
            /*
            if(pixel_byte != old_pixel_byte){
                //if theyre different write it
                spi_send_recv(pixel_byte);
            }
            */
            

        }

	}
}



