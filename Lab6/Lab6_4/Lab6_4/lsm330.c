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
#include "Lab6_2.h"

#define READ_BIT PIN7_bm

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
	PORTF.OUTCLR = SSA;	//SSA enable
	
	//Address OR with READ CYCLE enable
	spif_write( (reg_addr | READ_BIT) );
	uint8_t value = spif_read();
	
	PORTF.OUTSET = SSA;	//SSA disable
	
	return(value);
z}

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

//Initialize the Accelerometer
void accel_init(void){		
	//Reset CTRL_REG4_A, enable interrupt, active high interrupt
	//Int1 connects to pc7, need to config external interrupt on uController
	accel_write(CTRL_REG4_A, CTRL_REG4_A_STRT | CTRL_REG4_A_INT1_EN | CTRL_REG4_A_IEA);	
	//Enable accelerometer to measure all 3 dimensions simultaneously and config measurements rate at 1600Hz
	accel_write(CTRL_REG5_A, CTRL_REG5_A_XEN | CTRL_REG5_A_YEN | CTRL_REG5_A_ZEN | CTRL_REG5_A_ODR3 | CTRL_REG5_A_ODR0);
}

//External Interrupt
void init_interrupt(void) {
	// Port c pin 7 is interrupt source
	PORTC_INT0MASK = PORTC_INT0MASK_CONFIG;
	//Input
	PORTC_DIRCLR = PORTC_DIRCLR_CONFIG;
	//Low Level
	PORTC_INTCTRL = PORTC_INTCTRL_CONFIG;
	//Rising edge
	PORTC_PIN7CTRL = PORTC_PIN7CTRL_CONFIG;
	//Turn on low level interrupts
	PMIC_CTRL = PMIC_CTRL_CONFIG;
}

//ISR global flag
volatile int aFlag = 0;

//ISR Vector
ISR(PORTC_INT0_vect){
	//Preserve Status Reg
	uint8_t temp = CPU_SREG;
	//Set int flags
	PORTC.INTFLAGS = 0x01;
	//Set checker
	aFlag = 1;
	//Restore Status Reg
	CPU_SREG = temp;
}
