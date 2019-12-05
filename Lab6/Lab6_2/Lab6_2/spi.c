/*
 * spi.c
 *
 *  Last updated: 10/21/2019 3:17 PM
 *  Author: Dr. Schwartz
*/ 

#include <avr/io.h>
#include "spi.h"

/*******************************************************************************
;Lab 6 Part 2
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: COMMUNICATING WITH THE LSM330---SPI
;*********************************INCLUDES**************************************/
extern const uint8_t PORTF_DIRSET_CONFIG;
extern const uint8_t PORTF_DIRCLR_CONFIG;
extern const uint8_t PORTF_OUTSET_CONFIG;
//******************************END OF INCLUDES*********************************
/* initializes the SPI module of Port F to communicate with the LSM330 */
void spif_init(void){
  /* configure pin direction of SPI signals */
  PORTF.DIRSET	= PORTF_DIRSET_CONFIG;	//Output
  PORTF.DIRCLR  = PORTF_DIRCLR_CONFIG;	//Input
  PORTF.OUTSET  = PORTF_OUTSET_CONFIG;	//Set initial output as high.

	/* 8 MHz SPI frequency since 10MHz is the maximum allowed by the LSM330 */
	SPIF.CTRL	=	SPI_PRESCALER_DIV4_gc	|	SPI_MASTER_bm	|	SPI_MODE_2_gc	|	SPI_ENABLE_bm;
}

/* writes a single byte of data to the SPIF data register */
void spif_write(uint8_t data){
	//Enable Slave port f pin 4
	PORTF.OUTTGL = 0x10;
	
	SPIF.DATA = data;
	while((SPIF.STATUS & 0x80) != 0x80);	/* wait for transfer to be complete */
	
	//Turn off slave select
	PORTF.OUTTGL = 0x10;
}

/* attempts to read a byte of data from device connected to SPIF */
uint8_t spif_read(void){
  PORTF.OUTTGL = 0x10;
  
  SPIF.DATA = 0x37;                   /* write garbage to cause transaction */
  while((SPIF.STATUS & 0x80) != 0x80);	/* wait for transfer to be complete */
  
  PORTF.OUTTGL = 0x10;
  
  return SPIF.DATA;	
}
