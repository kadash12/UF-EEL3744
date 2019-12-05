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

//****************** LSM.h ***********************************
//PORT C
const uint8_t PORTC_INTCTRL_CONFIG = 0x01;//0b00000001;
const uint8_t PORTC_INT0MASK_CONFIG = 0x80;//0b10000000;
const uint8_t PORTC_PIN7CTRL_CONFIG = 0x01;//0b00000001
const uint8_t PMIC_CTRL_CONFIG = 0x01;//0b00000001
const uint8_t PORTC_DIRCLR_CONFIG = 0x80; //0b10000000;

#endif /* LAB6_2_H_ */