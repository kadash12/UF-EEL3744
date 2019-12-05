;******************************************************************************
;Lab 3 Part 2
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: Interrupts, Continued
;******************************************************************************
;*********************************INCLUDES*************************************
.include "ATxmega128a1udef.inc"
;******************************END OF INCLUDES*********************************
;******************************DEFINED SYMBOLS*********************************
.equ outhigh=0xff	;set as input or high value
.equ inlow=0x00	;set as output or low value
.equ stackaddress=0x3FFF	;stack starting address
;**************************END OF DEFINED SYMBOLS******************************
;******************************MEMORY CONSTANTS********************************
	; data memory allocation
	.dseg

;***************************END OF MEMORY CONSTANTS****************************
;********************************MAIN PROGRAM**********************************
	.cseg
.org 0x0000
	rjmp MAIN

;Interrupt Vector
.org PORTF_INT0_VECT		
	rjmp Interrtupt

.org 0x100
MAIN:
	ldi r21,0	;counter

	;Stack initialization
	ldi YL, low(stackaddress)
	out CPU_SPL,YL
	LDI YL, high(stackaddress)
	out	CPU_SPH,YL
	;initialization of I/O
	rcall IO_INIT
	;call our subroutine to initialize our interrupt
	rcall INIT_INTERRUPT	
	;intialization blue led
	rcall BLUE_PWM

Done:
	rjmp Done
;*****************************END OF MAIN PROGRAM *****************************
;********************************SUBROUTINES***********************************
; Name: IO_INIT
; Purpose: To initialize the relevant input/output modules, as pertains to the
;		   application.
; Input(s): N/A
; Output: N/A
; Affected: N/A
;******************************************************************************
IO_INIT:
	; protect relevant registers

	; initialize the relevant I/O
	;switch
	; S1
	ldi r16, 0x01<<2
	sts PORTF_DIRCLR, r16
	; LED
	;8-bit LED
	ldi r16, outhigh	;load 1 to register
	sts PORTC_OUT,r16	;set portC initally off
	sts PORTC_DIR,r16	;set portC as output
	;BLUE_PWM
	ldi r16, 0xFF
	sts PORTD_OUT, r16	;set portC initally off

	ldi r16, 0x40	;load 1 to register LED
	sts PORTD_DIRSET,r16	;set portC as output

	; recover relevant registers

	; return from subroutine
	ret
;******************************************************************************
; Name:     INIT_INTERRUPT
; Purpose:  Subroutine to initialize the interrupt 
; Inputs:   None			 
; Outputs:  None
; Affected: 
;******************************************************************************
INIT_INTERRUPT:	
	;select s1 as interrupt source
	ldi r16, 0x04
	sts PORTF_INT0MASK, r16
	;turn on low level interrupts
	sts PMIC_CTRL, r16

	;select low level pin for external interrupt
	ldi	r16, 0x03
	sts PORTF_INTCTRL, r16
	;port falling config 
	ldi r16, 0x02
	sts PORTF_PIN2CTRL, r16	

	sei		;turn on the global interrupt flag
	ret
;******************************************************************************
; Name:     BLUE_PWM
; Purpose:  Toggle BLUE LED
; Inputs:   None			 
; Outputs:  None
; Affected: 
;******************************************************************************
 BLUE_PWM: 
	;Turn BLUE off
	ldi r16, 0xFF
	sts PORTD_OUT, r16
	;Turn BLUE on
	ldi r16, 0x00
	sts PORTD_OUT, r16
	;Loop endless
   	rjmp BLUE_PWM

	ret
;*****************************END OF SUBROUTINES*******************************
;**********************************INTERRTUPT**********************************
; Name: Interrtupt 
; Purpose:  Toggle LED
; Inputs:   None			 
; Outputs:  None
; Affected: 
;******************************************************************************
Interrtupt:
	lds r20,CPU_SREG
	push r20

	inc r21	;increment counter
	mov r17,r21
	com r17	;active high

    sts PORTC_OUT, r17	;store counter value to LED portout

	; Clear Interupts flags
	ldi r16,  0x01
	sts PORTF_INTFLAGS, r16	

	sts CPU_SREG, r20
	pop r20
	reti		;return from the interrupt routine
;*****************************END OF INTERRTUPT********************************
;*****************************END OF "lab2_4.asm"******************************
