#include <avr/io.h>
#include <avr/interrupt.h>
#define period (int)((2000000/1) / (3.744*1000))
int set;
int main(void) {
 PORTA_DIR = 0xFE;	//input switch
 
 PORTC_DIR = 0xFF;	//Output LED
 PORTC_OUT = 0xFF;	//Initially off
 
 tcc0_init();
 
 while(1){
	 if(PORTA_IN ==0x00 && set==0){
		 tcc0_init();
		 PMIC_CTRL = PMIC_LOLVLEN_bm;	//Low level interrupts
		 sei();	//global interrupt
		 set=1;
	 }
	 else if (PORTA_IN ==0x01){
		 TCC0.CNT = 0; //Initial value 0
		 TCC0_CTRLA = 0x00;	//Prescaler = 1
		 TCC0.INTCTRLA = 0x00;
		 PORTC_OUTSET = 0xFF;	//Initially off
		 set =0;
	 }
 }
}

//Initialize TCC0 timer/counter
void tcc0_init(void){
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
	PORTC_OUTTGL=0xFF;
	
	//Restore Status Reg
	CPU_SREG = temp;
	//Return from ISR
	return;
}