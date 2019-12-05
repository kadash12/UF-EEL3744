/*********************************INCLUDES**************************************
;Lab 6 Part 3
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: Receiving with SPI & Communicating with the LSM330
;*********************************INCLUDES**************************************/
#include <avr/io.h>
#include "lsm330.h"
#include "lsm330_registers.h"
#include "spi.h"

#define READ_BIT PIN7_bm

/* your lsm330 function definitions here */
//Returns a single byte of data that is read from a specific accelerometer register (reg_addr) within the LSM330.s
uint8_t accel_read(uint8_t reg_addr){
//Select accel
PORTF.OUTCLR = SSA;	//SSA enable

//Address OR with READ CYCLE enable
spif_write( (reg_addr | READ_BIT) );
uint8_t value = spif_read();

PORTF.OUTSET = SSA;	//SSA disable

return(value);
}

//Writes a single byte of data (data) to a specific accelerometer register (reg_addr) within the LSM330.
void accel_write(uint8_t reg_addr, uint8_t data){
//Select accel
PORTF.OUTCLR = SSA;	//SSA enable

//Write reg address then data
spif_write(reg_addr);
spif_write(data);

PORTF.OUTSET = SSA;	//SSA disable

return;
}
