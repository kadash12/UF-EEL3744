/******************************************************************************
;Lab 6 Part Qiz
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
	
	uint8_t f =0;
	uint8_t b = 0;
	uint8_t l = 0;
	uint8_t r = 0;
	
	char* one = "left";
	char* two = "right";
	char* three = "forward";
	char* four = "backward";
	//Loop always
	while(1){
		//Check on global flag
		if(accel_flag){
			if (accel_read(OUT_X_L_A)>0 && l ==0){
			//Read -> Output XL
			lsm_data.byte.accel_x_low = accel_read(OUT_X_L_A);
			usartd0_out_string(one);
				f=0;
				b = 0;
				l = 1;
				r = 0;
			}
			if(accel_read(OUT_X_H_A)>0 && r == 0){
			//Read -> Output XH
			lsm_data.byte.accel_x_high = accel_read(OUT_X_H_A);
			usartd0_out_string(two);
				f=0;
				b = 0;
				l = 0;
				r = 1;
			}
			if(accel_read(OUT_Y_L_A)>0 && b==0){
			//Read -> Output YL
			lsm_data.byte.accel_y_low = accel_read(OUT_Y_L_A);
			usartd0_out_string(four);
				f=0;
				b = 1;
				l = 0;
				r = 0;
			}
					if(accel_read(OUT_Y_H_A) && f==0){
			//Read -> Output YL
			lsm_data.byte.accel_y_high = accel_read(OUT_Y_H_A);
			usartd0_out_string(three);
							f=1;
				b = 0;
				l = 0;
				r = 0;
			}
		/*
			//Read -> Output ZL
			lsm_data.byte.accel_z_low = accel_read(OUT_Z_L_A);
			usartd0_out_char((char) lsm_data.byte.accel_z_low);

			//Read -> Output ZH
			lsm_data.byte.accel_z_high = accel_read(OUT_Z_H_A);
			usartd0_out_char((char) lsm_data.byte.accel_z_high);*/
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