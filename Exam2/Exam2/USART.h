/*******************************************************************************
;Lab 6 Part 5
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: Plotting Real-Time Accelerometer Data
*/
#ifndef USART_H_
#define USART_H_

//USART Initialization
void usartd0_init(void);

//Output character
void usartd0_out_char(char output);

//Output string
void usartd0_out_string(char *str);

#endif /* USART_H_ */