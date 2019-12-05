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
#include "spi.h"
#include "Lab6_5.h"

//External constants
extern const uint8_t PORTC_INTCTRL_CONFIG;
extern const uint8_t PORTC_INT0MASK_CONFIG;
extern const uint8_t PORTC_PIN7CTRL_CONFIG;
extern const uint8_t PMIC_CTRL_CONFIG;
extern const uint8_t PORTC_DIRCLR_CONFIG;

/* your lsm330 function definitions here */
//Returns a single byte of data that is read from a specific accelerometer register (reg_addr) within the LSM330.s
uint8_t accel_read(uint8_t reg_addr){
	//Select accel
	PORTF.OUTCLR = SA;	//SA enable
	
	//Address OR with READ CYCLE enable
	spif_write( (reg_addr | READ) );
	uint8_t value = spif_read();
	
	PORTF.OUTSET = SA;	//SA disable
	
	return(value);
}

//Writes a single byte of data (data) to a specific accelerometer register (reg_addr) within the LSM330.
void accel_write(uint8_t reg_addr, uint8_t data){
	//Select accel
	PORTF.OUTCLR = SA;	//SA enable
	
	//Write reg address then data
	spif_write(reg_addr);
	spif_write(data);

	PORTF.OUTSET = SA;	//SA disable
	
	return;
}

void accel_init(void){	
	//Set up interrupt
	//Input
	PORTC.DIRCLR = PIN7_bm;
	//Low Level
	PORTC.INTCTRL = PORTC_INTCTRL_CONFIG;
	// Port c pin 7 is interrupt source
	PORTC.INT0MASK = PORTC_INT0MASK_CONFIG;
	//Rising edge
	PORTC.PIN7CTRL = PORTC_PIN7CTRL_CONFIG;

	//Reset CTRL_REG4_A, enable interrupt, active high interrupt
	//Int1 connects to pc7, need to config external interrupt.
	accel_write(CTRL_REG4_A, 0xC8);

	//Enable accelerometer to measure all 3 dimensions simultaneously and config measurements rate at 1600Hz
	accel_write(CTRL_REG5_A, 0x97);
	
	return;
}