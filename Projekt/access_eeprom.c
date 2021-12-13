/*
 * I2C Example project for the mcb32 toolchain
 * Demonstrates the temperature sensor and display of the Basic IO Shield
 * Make sure your Uno32-board has the correct jumper settings, as can be seen
 * in the rightmost part of this picture:
 * https://reference.digilentinc.com/_media/chipkit_uno32:jp6_jp8.png?w=300&tok=dcceb2
 */

#include <pic32mx.h>
#include <stdint.h>
#include <stdbool.h>
#include "snake_defines.h"  /* Declatations for these labs */

#define DISPLAY_VDD_PORT PORTF
#define DISPLAY_VDD_MASK 0x40
#define DISPLAY_VBATT_PORT PORTF
#define DISPLAY_VBATT_MASK 0x20
#define DISPLAY_COMMAND_DATA_PORT PORTF
#define DISPLAY_COMMAND_DATA_MASK 0x10
#define DISPLAY_RESET_PORT PORTG
#define DISPLAY_RESET_MASK 0x200

/* Address of the temperature sensor on the I2C bus */
#define TEMP_SENSOR_ADDR 0x48 //1010000
#define SPEEDEEPROM_CONTROL_BYTE 0b1010100 //1 0 1 0 A2 A1 A0 R/W - here A2 = 1 A1=0,A0=0
/* Temperature sensor internal registers */
typedef enum TempSensorReg TempSensorReg;
enum TempSensorReg {
	TEMP_SENSOR_REG_TEMP, //0
	TEMP_SENSOR_REG_CONF, //1
	TEMP_SENSOR_REG_HYST, //2
	TEMP_SENSOR_REG_LIMIT, //3
};



/* Wait for I2C bus to become idle */
void i2c_idle() {
	while(I2C1CON & 0x1F || I2C1STAT & (1 << 14)); //TRSTAT
}

/* Send one byte on I2C bus, return ack/nack status of transaction */
bool i2c_send(uint8_t data) {
	i2c_idle();
	I2C1TRN = data;
	i2c_idle();
	return !(I2C1STAT & (1 << 15)); //ACKSTAT
}

/* Receive one byte from I2C bus */
uint8_t i2c_recv() {
	i2c_idle();
	I2C1CONSET = 1 << 3; //RCEN = 1
	i2c_idle();
	I2C1STATCLR = 1 << 6; //I2COV = 0
	return I2C1RCV;
}

/* Send acknowledge conditon on the bus */
void i2c_ack() {
	i2c_idle();
	I2C1CONCLR = 1 << 5; //ACKDT = 0
	I2C1CONSET = 1 << 4; //ACKEN = 1
}

/* Send not-acknowledge conditon on the bus */
void i2c_nack() {
	i2c_idle();
	I2C1CONSET = 1 << 5; //ACKDT = 1
	I2C1CONSET = 1 << 4; //ACKEN = 1
}

/* Send start conditon on the bus */
void i2c_start() {
	i2c_idle();
	I2C1CONSET = 1 << 0; //SEN
	i2c_idle();
}

/* Send restart conditon on the bus */
void i2c_restart() {
	i2c_idle();
	I2C1CONSET = 1 << 1; //RSEN
	i2c_idle();
}

/* Send stop conditon on the bus */
void i2c_stop() {
	i2c_idle();
	I2C1CONSET = 1 << 2; //PEN
	i2c_idle();
}



/*our code*/

void send_write_control_byte(){
        /* Send start condition and address of the eeprom memory with
	write mode (lowest bit = 0) until the eeprom memory sends
	acknowledge condition */
	do {
		i2c_start();
	} while(!i2c_send(SPEEDEEPROM_CONTROL_BYTE << 1));
}
void send_read_control_byte(){
        /* Send start condition and address of the eeprom memory with
	read mode (lowest bit = 1) until the eeprom memory sends
	acknowledge condition */
	do {
		i2c_start();
	} while(!i2c_send(SPEEDEEPROM_CONTROL_BYTE << 1 | 1));
}
/*this is called "BYTE WRITE" in the manual*/
void write_byte_to_eeprom(uint16_t address, uint8_t data){
    uint8_t address_upper_bits = (uint8_t) address >> 8;
    uint8_t address_lower_bits = (uint8_t) address & 0xFF;//8 zeros followed by 8 ones
    send_write_control_byte();
    /* Send address high byte*/
	i2c_send(address_upper_bits);
    /* Send address low byte*/
    i2c_send(address_lower_bits);
	/* send our data */
	i2c_send(data);
	/* Send stop condition */
	i2c_stop();
}
/*this is called "RANDOM READ" in the manual*/
uint8_t read_byte_from_eeprom(uint16_t address){
    uint8_t address_upper_bits = (uint8_t) address >> 8;
    uint8_t address_lower_bits = (uint8_t) address & 0xFF;//8 zeros followed by 8 ones

    //uint16_t temp;
    //temp = I2C1RCV; //Clear receive buffer

    //random read requires us to first send a write command with the adress we want to read from 
    send_write_control_byte();
    i2c_send(address_upper_bits);
    i2c_send(address_lower_bits);

    //now we send our read command
    send_read_control_byte();


    /* Now the slave sends data to us */
	uint8_t read_value = i2c_recv();
	
		
    /* To stop receiving, send nack and stop */
	i2c_nack();
	i2c_stop();
    
    return read_value;
}