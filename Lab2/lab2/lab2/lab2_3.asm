;*********************************INCLUDES*************************************
.include "ATxmega128a1udef.inc"
;******************************END OF INCLUDES*********************************
;******************************DEFINED SYMBOLS*********************************
.equ outhigh=0xff	;set as input or high value
.equ stackaddress=0x3FFF	;stack starting address
;**************************END OF DEFINED SYMBOLS******************************
;******************************MEMORY CONSTANTS********************************
;***************************END OF MEMORY CONSTANTS****************************
;Lab 2 Part 3
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: Introduction To Timer/Counters 
;********************************MAIN PROGRAM**********************************
	.cseg
.org 0x0000
	rjmp MAIN

.org 0x100
MAIN:
	;Stack initialization
	ldi YL, low(stackaddress)
	out CPU_SPL,YL
	LDI YL, high(stackaddress)
	out	CPU_SPH,YL

	ldi r16, outhigh	;load 1 to register
	sts PORTC_OUT,r16	;set portC initally off
	sts PORTC_DIR,r16	;set portC as output

	ldi ZL, 0x87
	ldi ZH, 0x01
	sts TCC0_PER, ZL		;load period tick
	sts TCC0_PER+1, ZH

	;Ex10
	ldi r20,0	;number of minutes
	ldi r21,0	;number of runs 50ms -> 1s = 20 runs
	ldi r22,0	;number of seconds

Loop:
	rcall DELAY_50MS	;start subrountine

	;Ex10
	inc r21 ;increment run

	ldi r18,1	;reset flag
	sts TCC0_INTFLAGS, r18

	ldi r18,0	;reset count
	sts TCC0_CNT, r18
	sts TCC0_CNT+1, r18

	ldi r16, 0x00	;load true value to r16
    sts PORTC_OUT, r16	;store toggle LED portout
    
	rcall DELAY_50MS	;start subrountine

	;Ex10
	inc r21 ;increment run
	cpi r21,20	;1 seconds reached
	BREQ second

;Ex10
RETURN:
	cpi r22,60	;1 min reached
	BREQ minute

	ldi r18,1	;reset flag
	sts TCC0_INTFLAGS, r18

	ldi r18,0	;reset count
	sts TCC0_CNT, r18
	sts TCC0_CNT+1, r18
	
	ldi r16, outhigh	;load true value to r16
    sts PORTC_OUT, r16	;store toggle LED portout

    rjmp Loop

;Ex10
second:
	inc r22	;increment second counter
	ldi r21, 0	;reset run counter
	rjmp RETURN

minute:	
	inc r20	;increment minute counter
	ldi r22,0	;reset second counter
	rjmp return

.org 0x300	;put this here only to know the address of subroutine
;*********************SUBROUTINES**************************************
; Subroutine Name: DELAY_50MS
; Delay program form 50ms
; Inputs: N/A
; Ouputs: N/A
; Affected: R17,R18
DELAY_50MS:
	ldi r18, 6
	sts TCC0_CTRLA, r18		;increment and set prescaler 256

	lds r17, TCC0_INTFLAGS	;load flag	
	sbrs r17, 0	;check if flag is triggered
	rjmp DELAY_50MS	;continue delay

	ret	;return stack pointer
;*****************************END**********************************************