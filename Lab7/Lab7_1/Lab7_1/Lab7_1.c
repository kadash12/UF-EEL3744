/*******************************************************************************
;Lab 7 Part 1
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: USING THE ADC SYSTEM
;*********************************INCLUDES**************************************/

#include <avr/io.h>

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

	//Enable ADC
	ADCA_CTRLA = ADC_ENABLE_bm;
}

int main(void){
	//Initialize
	adc_init();

	//Test ADC value
	int16_t test = 0x00;
	
	//Loop conversion
	while (1){
		//Start ADC
		ADCA_CH0_CTRL |= ADC_CH_START_bm;

		//Stall till conversion is complete
		while( !(ADCA_CH0_INTFLAGS & ADC_CH_CHIF_bm) );

		//Store value to check
		test = ADCA_CH0_RES;

		//Reset
		ADCA_CH0_INTFLAGS = ADC_CH_CHIF_bm;
	}
	return 0;
}


