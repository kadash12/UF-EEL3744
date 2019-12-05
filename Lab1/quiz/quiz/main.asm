;Lab 1
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: Quiz
	.include "ATxmega128a1udef.inc"

	.equ IN_TABLE_START_ADDR = 0xDCBA
	.equ OUT_TABLE_START_ADDR = 0x3000
	.equ NULL = 0

	.cseg


	.org IN_TABLE_START_ADDR

IN_TABLE: .db 44,23,55,37,33,13,0
		
IN_TABLE_END:

	.dseg

	.org OUT_TABLE_START_ADDR
OUT_TABLE: .byte (IN_TABLE_END - IN_TABLE)
	.cseg
	
	.org 0x0
	rjmp MAIN

	.org 0x100
MAIN:

	ldi ZL, low(IN_TABLE << 1) 
	ldi ZH, high(IN_TABLE <<1)

	ldi r16, BYTE3(IN_TABLE<<1);
    sts CPU_RAMPZ,r16

	ldi YL, low(OUT_TABLE)  
	ldi YH, high(OUT_TABLE) 


LOOP:
	elpm r16,Z+ ;Num
	CPI r16, NULL	
	BREQ LOOP_END1

	/*elpm r17,Z+ ;Nunb
	CPI r17, NULL	
	BREQ LOOP_END1
	
	cp r16,r17
	SBRS r16,0*/


	SBRS r16,0
	BRNE CONDITION_1
	
	BRNE CONDITION_2

CONDITION_1:
	LSR r16
	rjmp STORE1

CONDITION_2:
	CPI r16, 37
	BRLO CONDITION_2P

	rjmp LOOP

CONDITION_2P:
	LSL r16
	LSL r16
	rjmp STORE1

LOOP_END1:
	st Y+,r16
	rjmp DONE

STORE1:
	st Y+,r16
	rjmp LOOP

DONE: 
	rjmp DONE
