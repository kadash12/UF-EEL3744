/*******************************************************************************
;Lab 7 Part 2
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: SAMPLING AT A SPECIFIC RATE USING EVENTS 
;*********************************INCLUDES**************************************/
#include <avr/io.h>
#include <avr/interrupt.h>

#define BLUE_PWM_LED PIN6_bm

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
	PORTD_OUTSET = 0x00;	//LED initially off
	PORTD_DIRSET = BLUE_PWM_LED;	//Set output
		
	adc_init();
	tcc0_init();
	
	//Loop conversion
	while (1){}
		
	return 0;
}

//Interrupt Handler
ISR(ADCA_CH0_vect){
	//Test ADC value
	int16_t test = ADCA_CH0_RES;

	//Turn off the BLUE_PWM LED
	PORTD_OUTTGL = BLUE_PWM_LED;
}
