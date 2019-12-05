/*******************************************************************************
;Lab 6 Part 5
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: Plotting Real-Time Accelerometer Data
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "lsm330.h"
#include "lsm330_registers.h"
#include "USART.h"

//ISR global flag
volatile int accel_flag = 0;

int main(void){
	//Initialize SPI
	spif_init();
	//Initialize USART
	usartd0_init();
	//Initialize External Interrupt and the Accelerometer
	accel_init();
	
	//Turn on low level interrupts
	PMIC.CTRL=0x07;
	//Turn on global interrupts
	sei();
	
	// This type is a union that will allow you to access the data read from the LSM330 in a much easier way. 
	lsm330_data_t lsm_data;
	
	//Loop always
	while(1){
		//Check on global flag
		if(accel_flag){
			//Read -> Output XL
			lsm_data.byte.accel_x_low = accel_read(OUT_X_L_A);
			usartd0_out_char((char) lsm_data.byte.accel_x_low);
			//Read -> Output XH
			lsm_data.byte.accel_x_high = accel_read(OUT_X_H_A);
			usartd0_out_char((char) lsm_data.byte.accel_x_high);
			//Read -> Output YL
			lsm_data.byte.accel_y_low = accel_read(OUT_Y_L_A);
			usartd0_out_char((char)lsm_data.byte. accel_y_low);
			//Read -> Output YL
			lsm_data.byte.accel_y_high = accel_read(OUT_Y_H_A);
			usartd0_out_char((char)lsm_data.byte. accel_y_high);
			//Read -> Output ZL
			lsm_data.byte.accel_z_low = accel_read(OUT_Z_L_A);
			usartd0_out_char((char) lsm_data.byte.accel_z_low);
			//Read -> Output ZH
			lsm_data.byte.accel_z_high = accel_read(OUT_Z_H_A);
			usartd0_out_char((char) lsm_data.byte.accel_z_high);
			
			//Reset Flag
			accel_flag = 0;
		}
	}
	return 0;
}

ISR(PORTC_INT0_vect){
	//Push Status Registers
	uint8_t status = CPU_SREG;

	//Set interrupt flags
	PORTC.INTFLAGS = 0x01;

	//Set global variable
	accel_flag = 1;
	
	//Pop Status Reg
	CPU_SREG = status;

	return;
}