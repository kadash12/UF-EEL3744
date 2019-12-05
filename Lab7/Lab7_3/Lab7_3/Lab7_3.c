/*******************************************************************************
;Lab 7 Part 3
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: OUTPUTTING SAMPLED DATA WITH UART
;*********************************INCLUDES**************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "USART.h"

#define BLUE_PWM_LED PIN6_bm
#define m (5.0/4095)	//slope
#define b 0 //y-intercept

//Global Variables to store output
int16_t test = 0;	//Digit output
volatile int global_flag = 0;	//Global flag of interrupt

//Initialize TCC0 timer
void tcc0_init(void){
	//SCK = 2MHz, Prescaler = 1024, Time = 0.5sec
	int period = (2000000/(1024*2));	//2 Hz
	
	//Set period
	TCC0_PERL = (uint8_t) period;	//Low Period
	TCC0_PERH = (uint8_t) (period>>8);	//High Period

	//Normal mode timer
	TCC0_CTRLB = 0x00;
	
	//Trigger an event on Event Channel 0
	EVSYS_CH0MUX = EVSYS_CHMUX_TCC0_OVF_gc;

	//Set perscaler = 1024
	TCC0_CTRLA = 0x07;
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
	
	adc_init();
	tcc0_init();
	usartd0_init();
	
	//Storage of output characters
	char out[18];
	
	//Loop conversion
	while (1){
			//Output voltage when global flag gets set
			while (global_flag){
				//Reset
				global_flag = 0;

				float volt = (float) ((m*test)+b);	//Decimal voltage value

				//Check for +/-
				if (test<0) {	//Negative
					 out[0] = '-';
				}
				else {	//Positive
					out[0] = '+';
				}

				out[1] = ' ';	//Space		
				int index = 0;	//Indexing variable
				
				//Output decimal value
				for (int i = 0; i < 3; i++){	//3 digit
					if(volt<0){	//Convert negative to positive
						volt=volt*-1;
					}
					int temp = (int) volt;	//Int1 = (int) Pi = 3 
					out[2+index] = temp+'0';	//1 digit

					volt = 10*(volt-temp);	//Pi2 = 10*(Pi - Int1) = 1.4159
					
					if (i==0){
						out[3] = '.';	//Decimal point
						index++;	//Skip index 3
					}
					
					index++;	//increment
				}
	
				//Syntax
				out[6] = ' ';	
				out[7] = 'V';		
				out[8] = ' ';		
				out[9] = '(';	
				out[10] = '0';	
				out[11] = 'x';
				
				
					// Output the ADC to the serial terminal
				int hex3 = (uint8_t)(test>>8)%16;
				if(hex3 <= 9){
					out[12] = ((uint8_t) hex3)+'0';	//First hex
				}
				else if(hex3 > 9){
					switch(hex3){
						case 10: out[12] = 'A';	//First hex
						break;
						case 11: out[12] = 'B';	//First hex
						break;
						case 12: out[12] = 'C';	//First hex
						break;
						case 13: out[12] = 'D';	//First hex
						break;
						case 14: out[12] = 'E';	//First hex
						break;
						case 15: out[12] = 'F';	//First hex
						break;
					}
				}
				else{
					out[12] = '0';
				}

				// Output the ADC to the serial terminal
				int hex2 = (uint8_t)(test>>4)%16;
				if(hex2 <= 9){
					out[13] = ((uint8_t) hex2)+'0';	//First hex
				}
				else if(hex2 > 9){
					switch(hex2){
						case 10: out[13] = 'A';	//First hex
						break;
						case 11: out[13] = 'B';	//First hex
						break;
						case 12: out[13] = 'C';	//First hex
						break;
						case 13: out[13] = 'D';	//First hex
						break;
						case 14: out[13] = 'E';	//First hex
						break;
						case 15: out[13] = 'F';	//First hex
						break;
					}
				}
				else{
					out[13] = '0';
				}
				
				// Output the ADC to the serial terminal
				int hex = (uint8_t)(test)%16;
				if(hex <= 9){
					out[14] = ((uint8_t) hex)+'0';	//First hex
				}
				else if(hex > 9 ){
					switch(hex){
						case 10: out[14] = 'A';	//First hex
						break;
						case 11: out[14] = 'B';	//First hex
						break;
						case 12: out[14] = 'C';	//First hex
						break;
						case 13: out[14] = 'D';	//First hex
						break;
						case 14: out[14] = 'E';	//First hex
						break;
						case 15: out[14] = 'F';	//First hex
						break;
					}
				}
				else{
					out[14] = '0';
				}

				out[15] = ')';	
				out[16] = 13;	//Return
				out[17] = 10;	//New line
				
				//Output everything
				for (int j=0; j<18; j++){				
					usartd0_out_char(out[j]);
				}
			}
	}
	return 0;
}

//Interrupt Handler
ISR(ADCA_CH0_vect){
	//Test ADC value
	test = ADCA_CH0_RES;

	//Set global flag
	global_flag = 1;
	
	//Turn off the BLUE_PWM LED
	PORTD_OUTTGL = BLUE_PWM_LED;
}
