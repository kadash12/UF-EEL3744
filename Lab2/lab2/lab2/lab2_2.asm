;*********************************INCLUDES*************************************
.include "ATxmega128a1udef.inc"
;******************************END OF INCLUDES*********************************
;******************************DEFINED SYMBOLS*********************************
.equ outhigh=0xff	;set as input or high value
.equ stackaddress=0x3FFF	;stack starting address
;**************************END OF DEFINED SYMBOLS******************************
;******************************MEMORY CONSTANTS********************************
;***************************END OF MEMORY CONSTANTS****************************
;Lab 2 Part 2
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: Software Delays 
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

Loop:
	rcall DELAY_X_10MS ;number of delays

	rcall DELAY_10MS	;start subrountine

	ldi r16, 0x00	;load true value to r16
    sts PORTC_OUT, r16	;store toggle LED portout
    
	rcall DELAY_10MS	;start subrountine
	
	ldi r16, outhigh	;load true value to r16
    sts PORTC_OUT, r16	;store toggle LED portout

    rjmp Loop

.org 0x300	;put this here only to know the address of subroutine
;*********************SUBROUTINES**************************************
; Subroutine Name: DELAY_10MS
; Delay program form 10ms
; Inputs: N/A
; Ouputs: N/A
; Affected: R16,R17,R18,R19
	ldi	r19, 0x01	;bigger counter
DELAY_10MS:
	ldi r18, 0x00	;end value
	ldi r16, 0xFF	;max value
	mov r17, r20	;number of loop cycle
	//ldi r17, 0x04

work:
	inc r18		;counter
	cp r16,r18	;compare if counter reach max value, 
	breq ending	;branch to end

ending:
	cpi r17, 0x00	;check if zero
	breq done	;done
	dec r17	;contiune subrountine 
	rjmp work

done:
	dec r19	;bigger counter
	cpi r19, 0x00	;check if zero
	brne DELAY_10MS	;done

	ret	;return stack pointer

.org 0x400	;put this here only to know the address of subroutine
;*********************SUBROUTINES**************************************
; Subroutine Name: DELAY_X_10MS
; Number of delays to be run
; Inputs: N/A
; Ouputs: R20=#*0x09
; Affected: R20
DELAY_X_10MS:
	ldi r20, 12*0x08	;Number of delays to be run
	ret	;return stack pointer
;*****************************END**********************************************