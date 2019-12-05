/*******************************************************************************
;Lab 6 Part 5
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: Plotting Real-Time Accelerometer Data
*/
#define BLUE_PWM_LED PIN4_bm
#include <avr/io.h>
#include <avr/interrupt.h>
#include "lsm330.h"
#include "lsm330_registers.h"
#include "USART.h"

//ISR global flag
volatile int accel_flag = 0;
volatile int set=0;
int16_t x;
		int16_t y;
		int16_t z;

int main(void){
	//Initialize SPI
	spif_init();
	//Initialize USART
	usartd0_init();
	//Initialize External Interrupt and the Accelerometer
	accel_init();
	  
	 PORTF_DIR = 0xFF;	//Output LED
	 PORTF_OUT = 0xFF;	//Initially off
	 
	 PORTD_OUTCLR = BLUE_PWM_LED;	//LED initially off
	PORTD_DIRSET = BLUE_PWM_LED;	//Set output
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
		x =abs((accel_read(OUT_X_H_A)<<8)+accel_read(OUT_X_L_A));
		 y =abs((accel_read(OUT_Y_H_A)<<8)+accel_read(OUT_Y_L_A));
		z =abs((accel_read(OUT_Z_H_A)<<8)+accel_read(OUT_Z_L_A));
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
		
		 if(x>y&&x>z && set!=1){
			 tcc0_init((int)((2000000/1) / (3*1000)));
			 set=1;
		 }
		 if(y>x&&y>z && set!=2){
			 tcc0_init((int)((2000000/1) / (4*1000)));
			 set=2;
		 }
		 if(z>y&&z>x && set!=3){
			 tcc0_init((int)((2000000/1) / (5*1000)));
			 set=3;
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
//Initialize TCC0 timer/counter
void tcc0_init(int period){
	TCC0.CNT = 0; //Initial value 0
	TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;	//Low level interrupt
	TCC0_PERL = (uint8_t) period;	//Period
	TCC0_PERH = (uint8_t) (period>>8);

	TCC0_CTRLA = 0x01;	//Prescaler = 1
	return;
}


ISR(TCC0_OVF_vect){
	//Preserve Status Reg
	uint8_t temp = CPU_SREG;
	
	//Clear interrupt flags
	TCC0.INTFLAGS = 0x01;
	
	//Toggle
	PORTF_OUTTGL=PIN0_bm;
	
	//Restore Status Reg
	CPU_SREG = temp;
	//Return from ISR
	return;
	}