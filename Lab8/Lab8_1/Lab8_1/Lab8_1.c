/*******************************************************************************
;Lab 8 Part 1
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: INTRODUCTION TO DAC 
;*********************************INCLUDES**************************************/
#include <avr/io.h>

//Change clock.s
extern void clock_init(void);

int main(void){
	//Default 1V
    //CH0 enable and DAC enable
    DACA_CTRLA = DAC_CH0EN_bm | DAC_ENABLE_bm;  
	//Single mode channel 0
	DACA_CTRLB = DAC_CHSEL_SINGLE_gc;
	//AREFB
	DACA_CTRLC = DAC_REFSEL_AREFB_gc;
	
    PORTA_DIRSET = PIN5_bm;	//Output
	
	DACA_CH0DATA = (1.1/2.5)*4095;
	
    while (1) {}
	return 0;
}

