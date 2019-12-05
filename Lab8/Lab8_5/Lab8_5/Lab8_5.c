/*******************************************************************************
;Lab 8 Part 5
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description:  MAKING A MUSICAL INSTRUMENT 
;*********************************INCLUDES**************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "USART.h"

//Change clock.s
extern void clock_init(void);
#define period ((32000000/256/1) / 1760)
//1000Hz -> (32MHz/256)/1000Hz | ((32000000/256) / 1760)
//((int)((1000000)/(x))+10)

//Global Variable
const uint16_t sin[] = { //Sin wave
	0x800,0x832,0x864,0x896,0x8c8,0x8fa,0x92c,0x95e,
	0x98f,0x9c0,0x9f1,0xa22,0xa52,0xa82,0xab1,0xae0,
	0xb0f,0xb3d,0xb6b,0xb98,0xbc5,0xbf1,0xc1c,0xc47,
	0xc71,0xc9a,0xcc3,0xceb,0xd12,0xd39,0xd5f,0xd83,
	0xda7,0xdca,0xded,0xe0e,0xe2e,0xe4e,0xe6c,0xe8a,
	0xea6,0xec1,0xedc,0xef5,0xf0d,0xf24,0xf3a,0xf4f,
	0xf63,0xf76,0xf87,0xf98,0xfa7,0xfb5,0xfc2,0xfcd,
	0xfd8,0xfe1,0xfe9,0xff0,0xff5,0xff9,0xffd,0xffe,
	0xfff,0xffe,0xffd,0xff9,0xff5,0xff0,0xfe9,0xfe1,
	0xfd8,0xfcd,0xfc2,0xfb5,0xfa7,0xf98,0xf87,0xf76,
	0xf63,0xf4f,0xf3a,0xf24,0xf0d,0xef5,0xedc,0xec1,
	0xea6,0xe8a,0xe6c,0xe4e,0xe2e,0xe0e,0xded,0xdca,
	0xda7,0xd83,0xd5f,0xd39,0xd12,0xceb,0xcc3,0xc9a,
	0xc71,0xc47,0xc1c,0xbf1,0xbc5,0xb98,0xb6b,0xb3d,
	0xb0f,0xae0,0xab1,0xa82,0xa52,0xa22,0x9f1,0x9c0,
	0x98f,0x95e,0x92c,0x8fa,0x8c8,0x896,0x864,0x832,
	0x800,0x7cd,0x79b,0x769,0x737,0x705,0x6d3,0x6a1,
	0x670,0x63f,0x60e,0x5dd,0x5ad,0x57d,0x54e,0x51f,
	0x4f0,0x4c2,0x494,0x467,0x43a,0x40e,0x3e3,0x3b8,
	0x38e,0x365,0x33c,0x314,0x2ed,0x2c6,0x2a0,0x27c,
	0x258,0x235,0x212,0x1f1,0x1d1,0x1b1,0x193,0x175,
	0x159,0x13e,0x123,0x10a,0xf2,0xdb,0xc5,0xb0,
	0x9c,0x89,0x78,0x67,0x58,0x4a,0x3d,0x32,
	0x27,0x1e,0x16,0xf,0xa,0x6,0x2,0x1,
	0x0,0x1,0x2,0x6,0xa,0xf,0x16,0x1e,
	0x27,0x32,0x3d,0x4a,0x58,0x67,0x78,0x89,
	0x9c,0xb0,0xc5,0xdb,0xf2,0x10a,0x123,0x13e,
	0x159,0x175,0x193,0x1b1,0x1d1,0x1f1,0x212,0x235,
	0x258,0x27c,0x2a0,0x2c6,0x2ed,0x314,0x33c,0x365,
	0x38e,0x3b8,0x3e3,0x40e,0x43a,0x467,0x494,0x4c2,
	0x4f0,0x51f,0x54e,0x57d,0x5ad,0x5dd,0x60e,0x63f,
	0x670,0x6a1,0x6d3,0x705,0x737,0x769,0x79b,0x7cd
};
volatile int rflag = 0;	//Receiver flag
volatile char c;

//Initialize DAC
void DAC_init(void){
	PORTA_DIRSET = PIN3_bm;	//Output
	//Default 1V
	//CH1 enable and DAC enable
	DACA_CTRLA = DAC_CH0EN_bm | DAC_ENABLE_bm;
	//AREFB
	DACA_CTRLC = DAC_REFSEL_AREFB_gc;
}

//Initialize TCC0 timer/counter
void tcc0_init(int p){
	TCC0.CNT = 0; //Initial value 0
	
	TCC0_PERL = (uint8_t) p;	//Period
	TCC0_PERH = (uint8_t) (p>>8);
	TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;	//Low level interrupt
	
	TCC0_CTRLA = 0x01;	//Prescaler = 1
}

ISR(TCC0_OVF_vect){
	//Preserve Status Reg
	uint8_t temp = CPU_SREG;
	
	//Clear interrupt flags
	TCC0.INTFLAGS = 0x01;
	
	//Restore Status Reg
	CPU_SREG = temp;
	//Return from ISR
	return;
}

int main(void){
	//Initialization
	clock_init();
	DAC_init();
	tcc0_init(period);
	usartd0_init();
	DMA_init();
	
	//Enabled interrupts
	PMIC_CTRL = PMIC_LOLVLEN_bm;	//Low level interrupts
	sei();	//global interrupt
	
	PORTC_DIRSET |= PIN7_bm;	//enable_speaker
	PORTC_OUTSET = PIN7_bm;
	
	while (1){
		//Get input
		if (rflag){
			//Reset
			rflag = 0;
			//Output Note
			switch (c)
			{
			case 'W': tcc0_init((int) ((32000000/256/1) / 1046.50)); //C6 
				break;
			case '3': tcc0_init((int) ((32000000/256/1) / 1108.73)); //C#6/Db6
				break;
			case 'E': tcc0_init((int) ((32000000/256/1) / 1174.66)); //D6
				break;
			case '4': tcc0_init((int) ((32000000/256/1) / 1244.51)); //D#6/Eb6
				break;
			case 'R': tcc0_init((int) ((32000000/256/1) / 1318.51)); //E6
				break;
			case 'T': tcc0_init((int) ((32000000/256/1) / 1396.91)); //F6
				break;
			case '6': tcc0_init((int) ((32000000/256/1) / 1479.98)); //F#6/Gb6
				break;
			case 'Y': tcc0_init((int) ((32000000/256/1) / 1567.98)); //G6
				break;
			case '7': tcc0_init((int) ((32000000/256/1) / 1661.22)); //G#6/Ab6
				break;
			case 'U': tcc0_init((int) ((32000000/256/1) / 1760.00)); //A6
				break;
			case '8': tcc0_init((int) ((32000000/256/1) / 1864.66)); //A$6/Bb6
				break;
			case 'I': tcc0_init((int) ((32000000/256/1) / 1975.53)); //B6
				break;
			}
		}
	}
	return 0;
}

//Initialize DMA
void DMA_init(){
	uint32_t sine = (uint32_t)sin;
	
	//Reset DMA
	DMA.CTRL |= DMA_RESET_bm;
	
	//Increment the source after transfer
	//Increment the destination after received
	DMA.CH0.ADDRCTRL = DMA_CH_SRCRELOAD_BLOCK_gc|DMA_CH_SRCDIR_INC_gc|DMA_CH_DESTRELOAD_BURST_gc|DMA_CH_DESTDIR_INC_gc;
	//Transfer data when TCE0 overflows
	DMA.CH0.TRIGSRC = DMA_CH_TRIGSRC_TCC0_OVF_gc;
	//Byte transfers in a block transfer
	DMA.CH0.TRFCNT =512; //512=256(8bit)*2(16bit)
	//Unlimited repeats
	DMA.CH0.REPCNT = 0x00;

	//Starting address of the source
	DMA.CH0.SRCADDR0 = (uint8_t)(sine>>0);
	DMA.CH0.SRCADDR1 = (uint8_t)(sine>>8);
	DMA.CH0.SRCADDR2 = (uint8_t)(sine>>16);
	
	uint8_t* dac_ptr = &DACA.CH0DATA;
	uint32_t dac_address = (uint32_t)dac_ptr;
	//Store data to the DAC
	DMA.CH0.DESTADDR0 = (uint8_t) (dac_address>>0);
	DMA.CH0.DESTADDR1 = (uint8_t) (dac_address>>8);
	DMA.CH0.DESTADDR2 = (uint8_t) (dac_address>>16);
	
	//Enable CHO
	//Unlimited repeat, Data is sent in burst where each burst is 2 bytes long
	DMA.CH0.CTRLA = DMA_CH_REPEAT_bm|DMA_CH_SINGLE_bm|DMA_CH_BURSTLEN_2BYTE_gc;
	DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;
	
	//Enable DMA
	DMA.CTRL |= DMA_ENABLE_bm;
}

//Receiver Handler
ISR (USARTD0_RXC_vect){
	//Get input
	c = USARTD0.DATA;
	
	//Set receiver flag
	rflag = 1;
}