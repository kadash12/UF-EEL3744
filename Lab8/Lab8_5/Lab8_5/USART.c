/*******************************************************************************
;Lab 7 Part 5
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: SWITCHING BETWEEN MULTIPLE INPUTS
*/

#include <avr/io.h>

//USART Initialization
void usartd0_init(void){
	//Configure TxD and RxD pins
	PORTD.OUTSET = PIN3_bm;
	PORTD.DIRSET = PIN3_bm;
	PORTD.DIRCLR = PIN2_bm;
	
	//Baud rate: At 32 MHz, 234 BSEL, -4 BSCALE corresponds to 128000 bps */
	USARTD0.BAUDCTRLA = (uint8_t)234;
	USARTD0.BAUDCTRLB = (uint8_t)( (-4 << 4) | (234 >> 8));

	//8 data bits, no parity, and one stop bit.
	USARTD0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc & ~USART_SBMODE_bm;

	//Enable Receiver and/or Transmitter
	USARTD0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	
	//Enable interrupt 
	USARTD0.CTRLA = USART_RXCINTLVL_LO_gc;
}

//Output character
void usartd0_out_char(char output){
	//Wait till transmission is done
	while(!(USARTD0.STATUS & USART_DREIF_bm));
	USARTD0.DATA = output;	//output c
}

//Output string
void usartd0_out_string(char *str){
	//Loop char pointer to get string
	while(*str){
		usartd0_out_char(*(str++));	//Output string
	}
}