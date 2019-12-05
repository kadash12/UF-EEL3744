/*******************************************************************************
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

int main(void){
	//Initialize SPI
	spif_init();
	
	//Read WHO_AM_I_A register
	uint8_t check = accel_read(WHO_AM_I_A);
	return 0;
}

