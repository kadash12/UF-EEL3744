;******************************************************************************
;Lab 5 Part 5
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: USART, Character Input 
;*********************************INCLUDES*************************************
.include "ATxmega128a1udef.inc"
;******************************END OF INCLUDES*********************************
;******************************DEFINED SYMBOLS*********************************
.equ TR_xON = 0b00011000	;0x18
.equ pin_Tx = 0b00001000	;0x08
.equ pin_Rx = 0b00000100	;0x04
.equ usart = 0b00100011		;asynch, 8 data, even, 1 stat, 1 stop
.equ BSel = 9	
.equ BScale = -3	;57600 Hz
;**************************END OF DEFINED SYMBOLS******************************
;******************************MEMORY CONSTANTS********************************
;***************************END OF MEMORY CONSTANTS****************************
;********************************MAIN PROGRAM**********************************
	.cseg
.org 0x0000
	rjmp MAIN

String:
	.db "Johnny_Li", 0

.org 0x100
MAIN:
	ldi YL, 0xFF	;initialize low byte of stack pointer
	out CPU_SPL, YL
	ldi YL, 0x3F
	out CPU_SPH, YL	

	rcall INIT_USART

	ldi ZL, low(String<<1)
	ldi ZL, high(String<<1)
	//rcall OUT_STRING
	rcall IN_CHAR

	nop
LOOP:
	//ldi r16, 'U'
	rcall OUT_CHAR
	rjmp LOOP

;********************************SUBROUTINES***********************************
; NAME:			INIT_USART
; FUNCTION:     Initializes the USARTDO's TX and Rx, 
;               56000 (115200) BAUD, 8 data bits, 1 stop bit.
; INPUT:        None
; OUTPUT:       None
; DESTROYS:     R16
; REGS USED:	USARTD0_CTRLB, USARTD0_CTRLC, USARTD0_BAUDCTRLA,
;               USARTD0_BAUDCTRLB
; CALLS:        None.
;******************************************************************************
INIT_USART:
	push r16 

	ldi R16, pin_Tx	
	sts PortD_OUTSET, R16	;set the TX line to default to '1' as 
							;  described in the documentation
	sts PortD_DIRSET, R16	;Must set PortD_PIN3 as output for TX pin 
							;  of USARTD0					
	ldi R16, pin_Rx
	sts PortD_DIRCLR, R16	;Set RX pin for input

	ldi R16, TR_xON				;INIT_USART initializes UART 0 on PortD (PortD0)
	sts USARTD0_CTRLB, R16		;Turn on TXEN, RXEN lines

	ldi R16, usart
	sts USARTD0_CTRLC, R16		;Set Parity to none, 8 bit frame, 1 stop bit

	ldi R16, (BSel & 0xFF)		;select only the lower 8 bits of BSel
	sts USARTD0_BAUDCTRLA, R16	;set baudctrla to lower 8 bites of BSel 

	ldi R16, ((BScale << 4) & 0xF0) | ((BSel >> 8) & 0x0F)							
	sts USARTD0_BAUDCTRLB, R16	;set baudctrlb to BScale | BSel. Lower 
								;  4 bits are upper 4 bits of BSel 
								;  and upper 4 bits are the BScale. 
	pop r16
	ret
;******************************************************************************
; OUT_CHAR receives a character via R16 and will
;   poll the DREIF (Data register empty flag) until it true,
;   when the character will then be sent to the USART data register.
; SUBROUTINE:   OUT_CHAR
; FUNCTION:     Outputs the character in register R16 to the SCI Tx pin 
;               after checking if the DREIF (Data register empty flag)   
;     			is empty.  The PC terminal program will take this 
;               received data and  put it on the computer screen.
; INPUT:        Data to be transmitted is in register R16.
; OUTPUT:       Transmit the data.
; DESTROYS:     None.
; REGS USED:	USARTD0_STATUS, USARTD0_DATA
; CALLS:        None.
OUT_CHAR:
	push R17
POLL:
	lds R17, USARTD0_STATUS		;load status register
	sbrs R17, 5					;proceed to writing out the char if
								;  the DREIF flag is set
	rjmp POLL				;else go back to polling
	sts USARTD0_DATA, R16		;send the character out over the USART
	
	pop R17
	ret
;******************************************************************************
; NAME:			OUT_STRING
; FUNCTION:     Output a character string stored in program memory.
; INPUT:        None
; OUTPUT:       None
;******************************************************************************
OUT_STRING:
	push r16
Read:
	lpm r16, z+		;LOAD Z data and increment
	cpi r16, 0	;Check if null is reached
	breq End	;If null, reach end
	rcall OUT_CHAR
	rjmp Read

End:
	pop r16
	ret
; *********************************************
; IN_CHAR polls the receive complete flag and will
;   pass the received character pack to the calling routine in R16.
; SUBROUTINE:   IN_CHAR
; FUNCTION:     Receives typed character (sent by the PC terminal 
;               program through the PC to the PortD0 USART Rx pin) 
;               into register R16.
; INPUT:        None.
; OUTPUT:       Register R16 = input from SCI
; DESTROYS:     R16 (result is transferred in this register)
; REGS USED:	USARTD0_STATUS, USARTD0_DATA
; CALLS:        None
IN_CHAR:

Write:
	lds R16, USARTD0_STATUS		;load the status register
	sbrs R16, 7					;proceed to reading in a char if
								;  the receive flag is set
	rjmp Write				;else continue polling
	lds R16, USARTD0_DATA		;read the character into R16

	ret
;*****************************END OF SUBROUTINES*******************************