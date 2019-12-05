/*******************************************************************************
;Lab 6 Part 2
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: COMMUNICATING WITH THE LSM330
;*******************************************************************************/
#ifndef LAB6_2_H_
#define LAB6_2_H_

//Defines
#define TRUE 1

//****************** SPI.h ***********************************
//PORT F
const uint8_t PORTF_DIRSET_CONFIG = 0xB0;	//0b10110000;
const uint8_t PORTF_DIRCLR_CONFIG = 0x40;	//0b01000000;
const uint8_t PORTF_OUTSET_CONFIG = 0x10;	//0b00010000;

#endif /* LAB6_2_H_ */