;*********************************INCLUDES*************************************
.include "ATxmega128a1udef.inc"
;******************************END OF INCLUDES*********************************
;******************************DEFINED SYMBOLS*********************************
.equ outhigh=0xff	;set as input or high value
.equ inlow=0x00	;set as output or low value
;**************************END OF DEFINED SYMBOLS******************************
;******************************MEMORY CONSTANTS********************************
;***************************END OF MEMORY CONSTANTS****************************
;Lab 2 Part 1
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: I/O, Timing 
;********************************MAIN PROGRAM**********************************
	.cseg
.org 0x0000
	rjmp MAIN

.org 0x100
MAIN:
	ldi r16, inlow	;load 0 to register
    sts PORTA_DIR,r16	;set portA as input

	ldi r16, outhigh	;load 1 to register
	sts PORTC_OUT,r16	;set portC initally off
	sts PORTC_DIR,r16	;set portC as output

Loop:
	lds r16, PORTA_IN	;load port A value to r16
    sts PORTC_OUT, r16	;store switch value to LED portout
     
    rjmp Loop
;*****************************END**********************************************