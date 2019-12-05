;******************************************************************************
;Lab 3 Quiz
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: Quiz
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
	rjmp Delay	;Debounce
.org TCC0_OVF_vect		
	rjmp Interrtupt	;LED

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

	rcall period10

	ldi r23,0
	;call our subroutine to initialize our interrupt
	rcall INIT_INTERRUPT	

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

	;select low level pin for external interrupt
	ldi	r16, 0x01
	sts PORTF_INTCTRL, r16
	;port falling config 
	ldi r16, 0x02
	sts PORTF_PIN2CTRL, r16	

	;Timer setup prescaler
	ldi r16, 1
	sts TCC0_INTCTRLA, r16

	;turn on low level interrupts
	ldi r16,1
	sts PMIC_CTRL, r16

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
	ldi r16, 1	;reset flag
	sts TCC0_INTFLAGS, r16

	ldi r18,0	;reset count
	sts TCC0_CNT, r18
	sts TCC0_CNT+1, r18
	;disable timer
	sts TCC0_CTRLA, r18	

	lds r16, PORTF_IN
	sbrc r16, 2	;check if 2nd bit is clear set thus S1 is not pressed
	rjmp Wait	;return to edit
	
LED:
ldi r16, 0xFF
	sts PORTC_OUT, r16
	rcall pause
ldi r16, 0x00
	sts PORTC_OUT, r16
	rjmp pause
	lds r16, PORTF_IN
	sbrc r16, 2	;check if 2nd bit is clear set thus S1 is not pressed
	rjmp LED	;return to edit
	rcall period10
	; Clear Interupts flags
	ldi r16,  0x01
	sts PORTF_INTFLAGS, r16	

	rjmp Wait		;return from the interrupt routine

;******************************************************************************
; Name: Delay
; Purpose:  Delay function
; Inputs:   None			 
; Outputs:  None
; Affected: 
;******************************************************************************
Delay:
	ldi r16,1  //prescaler
	sts TCC0_CTRLA,r16

	ldi r16, 4	;trigger flag
	sts PORTF_INTFLAGS, r16

	ldi r16, 0	;Disable port F
	sts PORTF_INTCTRL, r16
	reti		;return from the interrupt routine
;******************************************************************************
; Name: Wait
; Purpose:  Delay function
; Inputs:   None			 
; Outputs:  None
; Affected: 
;******************************************************************************
Wait:
	ldi r16, 1	;reset
	sts PORTF_INTCTRL, r16
	reti		;return from the interrupt routine

Period10:
	SBRC r23, 0
	rjmp Period2
	ldi ZL, low(0x4B)
	ldi ZH, high(0x4C)  //10Hz
	sts TCC0_PER, ZL		;load period tick
	sts TCC0_PER+1, ZH
	ret
Period2:	
	ldi ZL, low(0x43)
	ldi ZH, high(0x0F)  //10Hz
	sts TCC0_PER, ZL		;load period tick
	sts TCC0_PER+1, ZH
	ret 

PAUSE:
	ldi r18, 7
	sts TCC0_CTRLA, r18		;increment and set prescaler 256

	lds r17, TCC0_INTFLAGS	;load flag	
	sbrs r17, 0	;check if flag is triggered
	rjmp pause;continue delay

	ret	;return stack pointer
;*****************************END OF INTERRTUPT********************************
;*****************************END OF "lab2_4.asm"******************************