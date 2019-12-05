/*******************************************************************************
;Lab 6 Part 2
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: COMMUNICATING WITH THE LSM330---Testing Code
;*******************************************************************************/
#include <avr/io.h>
#include "Lab6_2.h"
#include "spi.h"


int main(void){
	//Initialize SPI
	spif_init();
	
	//Transmit 0x53 continuously
	while(TRUE){
		spif_write(0x53);
	}
}