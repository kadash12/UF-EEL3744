/*******************************************************************************
;Lab 6 Part 5
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: Plotting Real-Time Accelerometer Data
;*******************************************************************************/
#ifndef LAB6_5_H_
#define LAB6_5_H_

//Defines
#define TRUE 1

#define BIT7 0x80
#define BIT0 0x01
#define READ PIN7_bm
//****************** LSM.h ***********************************
//PORT C
const uint8_t PORTC_INTCTRL_CONFIG = 0x03;//0b00000011;
const uint8_t PORTC_INT0MASK_CONFIG = 0x80;//0b10000000;
const uint8_t PORTC_PIN7CTRL_CONFIG = 0x02;//0b00000010
const uint8_t PMIC_CTRL_CONFIG = 0x01;//0b00000001
const uint8_t PORTC_DIRCLR_CONFIG = 0x80; //0b10000000;

#endif /* LAB6_5_H_ */