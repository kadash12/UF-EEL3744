;******************************************************************************
;Lab 3 Part 1
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: Introduction to Interrupts
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
.org TCC0_OVF_VECT		
	rjmp Interrtupt

.org 0x100
MAIN:
	;Stack initialization
	ldi YL, low(stackaddress)
	sts CPU_SPL,YL
	ldi YH, high(stackaddress)
	sts	CPU_SPH,YH

	; initialize relevant I/O modules (LEDs)
	rcall IO_INIT

	ldi ZL, 0x0E
	ldi ZH, 0x03
	sts TCC0_PER, ZL		;load period tick
	sts TCC0_PER+1, ZH

	;call our subroutine to initialize our interrupt
	rcall INIT_INTERRUPT	

	ldi r16,6
	sts TCC0_CTRLA, r16

Loop:
	rjmp Loop

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
	; LED
	ldi r16, outhigh	;load 1 to register
	sts PORTC_OUT,r16	;set portC initally off
	sts PORTC_DIR,r16	;set portC as output

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
	;turn on low level interrupts
	ldi r16, 0x01
	sts PMIC_CTRL, r16		
	sts TCC0_INTCTRLA, r16

	sei		;turn on the global interrupt flag
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
	lds r16,PORTC_IN	;load value
	com r16
    sts PORTC_OUT, r16	;store toggle LED portout

	; Clear Interupts flags
	ldi r16,  0x01
	sts PORTC_INTFLAGS, r16	

	ldi r18,0	;reset count
	sts TCC0_CNT, r18
	sts TCC0_CNT+1, r18

	ldi r18,1	;reset flag
	sts TCC0_INTFLAGS, r18

	reti		;return from the interrupt routine
;*****************************END OF INTERRTUPT********************************
;*****************************END OF "lab2_4.asm"******************************
