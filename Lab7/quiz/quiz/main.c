/*******************************************************************************
;Lab 7 Quiz
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: Quiz
;*********************************INCLUDES**************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "USART.h"

#define BLUE_PWM_LED PIN6_bm

//Global Variables to store output
int16_t test = 0;	//Digit output
volatile int tflag = 0;	//Conversion flag
volatile int rflag = 0;	//Receiver flag
volatile char c;

//Initialize TCC0 timer
void tcc0_init(void){
	//SCK = 2MHz, Prescaler = 1024,
	int period = 5000;	//100 Hz
	
	//Set period
	TCC0_PERL = (uint8_t) period;	//Low Period
	TCC0_PERH = (uint8_t) (period>>8);	//High Period

	//Normal mode timer
	TCC0_CTRLB = 0x00;
	
	//Trigger an event on Event Channel 0
	EVSYS_CH0MUX = EVSYS_CHMUX_TCC0_OVF_gc;

	//Set perscaler = 1024
	TCC0_CTRLA = 0x03;
}

//Initialize ADC
void adc_init(void){
	//12-bit signed, right-adjusted, Normal, 2.5Vref
	ADCA_CTRLB = ADC_RESOLUTION_12BIT_gc | ADC_CONMODE_bm;
	ADCA_REFCTRL = ADC_REFSEL_AREFB_gc;		//2.5Vref
	
	//ADC Clock prescaler=512
	ADCA_PRESCALER = ADC_PRESCALER_DIV512_gc;

	//Enable Port A
	PORTA_DIRCLR = PIN1_bm | PIN6_bm;	//PortA input pins
	PORTA_DIRCLR = PIN4_bm | PIN5_bm;	//PortA input pins

	//Differential input signal with gain
	ADCA_CH0_CTRL = ADC_CH_INPUTMODE_DIFFWGAIN_gc;
	
	//MUXCTRL pin1 + and pin6 -
	ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN1_gc | ADC_CH_MUXNEG_PIN6_gc;
	
	//Setup ADC Low Level interrupt
	ADCA_CH0_INTCTRL = ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_LO_gc;	//Triggered on flag- when a conversion is complete

	//Enable Low Level interrupts
	PMIC_CTRL = PMIC_LOLVLEN_bm;
	//Enable global enable interrupts
	sei();

	//ADC conversion start when Event Channel 0 is triggered
	ADCA_EVCTRL = ADC_SWEEP_0_gc | ADC_EVSEL_0123_gc | ADC_EVACT_CH0_gc;
	
	//Enable ADC
	ADCA_CTRLA = ADC_ENABLE_bm;
}

int main(void){
	//Initialize
	PORTD_OUTSET = BLUE_PWM_LED;	//LED initially off
	PORTD_DIRSET = BLUE_PWM_LED;	//Set output
	PORTA_OUTSET |= PIN5_bm;	//Control J3
	PORTA_DIRSET |= PIN5_bm;
	
	tcc0_init();
	usartd0_init();
	adc_init();
	
	//Loop conversion
	while (1){
		//Get input
		if (rflag || tflag){
			//Reset
			rflag = 0;
			//Reset
			tflag = 0;
			//Output
			if(c == '1') {
				//CDS
				//ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN1_gc | ADC_CH_MUXNEG_PIN6_gc;
				//Analog input jumper
				ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN4_gc | ADC_CH_MUXNEG_PIN5_gc;
				if(test<1000){
					//Output
					usartd0_out_char(((uint8_t) test));	//LSB
					usartd0_out_char(((uint8_t) (test >> 8)));	//MSB
				}
			}
			else if (c == '2') {
				//Analog input jumper
				ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN4_gc | ADC_CH_MUXNEG_PIN5_gc;
				if(test>=0){
					//Output
					usartd0_out_char(((uint8_t) test));	//LSB
					usartd0_out_char(((uint8_t) (test >> 8)));	//MSB
				}
			}
			else if (c == '3') {
				if(test>500 || test<-500){
					//Analog input jumper
					ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN4_gc | ADC_CH_MUXNEG_PIN5_gc;
					//Output
					usartd0_out_char(((uint8_t) test));	//LSB
					usartd0_out_char(((uint8_t) (test >> 8)));	//MSB
				}
			}
		}
	/*	if (tflag){
			//Reset
			tflag = 0;

			//Output
			usartd0_out_char(((uint8_t) test));	//LSB
			usartd0_out_char(((uint8_t) (test >> 8)));	//MSB
		}*/
	}
	return 0;
}

//Interrupt Handler
ISR (ADCA_CH0_vect){
	//Test ADC value
	test = ADCA_CH0_RES;

	//Set conversion flag
	tflag = 1;
	
	//Turn off the BLUE_PWM LED
	PORTD_OUTTGL = BLUE_PWM_LED;
	//Toggle J3 pin
	PORTA_OUTTGL |= PIN5_bm;
}

//Receiver Handler
ISR (USARTD0_RXC_vect){
	//Get input
	c = USARTD0.DATA;
	
	//Set receiver flag
	rflag = 1;
}
