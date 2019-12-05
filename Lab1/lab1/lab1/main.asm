;******************************************************************************
;  File name: lab1_skeleton.asm
;  Author:  Christopher Crary
;  Last Modified By: Dr. Schwartz
;  Last Modified On: 29 August 2019
;  Purpose: To filter data stored within a predefined input table based on a
;			set of given conditions and store a subset of filtered values
;			into an output table.
;******************************************************************************
;*********************************INCLUDES*************************************
	.include "ATxmega128a1udef.inc"
;******************************END OF INCLUDES*********************************
;*********************************EQUATES**************************************
	; potentially useful expressions
	; data to program address
	.equ IN_TABLE_START_ADDR = 0xAAAA/2
	.equ OUT_TABLE_START_ADDR = 0x3737
	.equ NULL = 0
;******************************END OF EQUATES**********************************
;***************************MEMORY CONFIGURATION*******************************
	; program memory constants (if necessary)
	.cseg

	.org IN_TABLE_START_ADDR

IN_TABLE: .db 234, 0xA0, '@', 060, 0b00110000, 93, 0x30, 042 
		  .db 'J', 52, 0b10011100, 210, 0xC6, 0xCA, '#', 0
	; label used to calculate size of input table
IN_TABLE_END:

	; data memory allocation (if necessary)
	.dseg

	.org OUT_TABLE_START_ADDR
OUT_TABLE: .byte (IN_TABLE_END - IN_TABLE)
;*************************END OF MEMORY CONFIGURATION**************************
;********************************MAIN PROGRAM**********************************
;Lab 1
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: Introduction to AVR and Assembly
	.cseg
	; configure the reset vector (ignore for now)
	.org 0x0
	rjmp MAIN

	; place main program after interrupt vectors (ignore for now)
	.org 0x100
MAIN:
	; point appropriate indices to input/output tables
	;Z points to input table.
	ldi ZL, low(IN_TABLE << 1) ; Shift due to being 16-bits in a 8-bit processor 
	ldi ZH, high(IN_TABLE <<1) ; Z points to the table

	//ldi r16, BYTE3(TABLE_INPUT_START_ADDRESS<<1);
    //sts CPU_RAMPZ,r16

	;Y points to output table
	ldi YL, low(OUT_TABLE)  
	ldi YH, high(OUT_TABLE) 

	; loop through input table, performing filtering and storing conditions
LOOP:
	; load value from input table into an appropriate register
	elpm r16,Z+

	; determine if the end of table has been reached (perform general check)
	CPI r16, NULL	;compare NULL and r16 to see if NULL is reached

	; if end of table (EOT) has been reached, i.e., the NULL character was 
	; encountered, the program should branch to the relevant label used to
	; terminate the program (i.e., LOOP_END)
	BREQ LOOP_END

	; if EOT was not encountered, perform the first specified 
	; overall conditional check on loaded value (CONDITION_1)
	BRNE CONDITION_1

CONDITION_1:
	; check if the first condition is met; if not, branch to the second
	; overall conditional check (CONDITION_2)
	; check if MSB is set
	SBRS r16,7 
	;if set skip else jump to condition_2
	RJMP CONDITION_2

	; if the first condition is met, perform the relevant operation(s)
	; divide by 2
	LSR r16
	; if the resulting value is greater than or equal to 78, store it to the first available location within the output table
	CPI r16,78
	BRSH STORE

	; if the first overall condition was met, the second condition should not
	; be performed and the next table value should be loaded, i.e.,
	; the program should jump back to the beginning of the relevant loop
	;rjmp LOOP	//<-------------

	; if the first overall condition was not met, the second condition
	; should be checked (CONDITION_2)
CONDITION_2:
	; check if the second condition is met (if necessary, add any additional
	; label[s], e.g., to handle the less than or equal condition)
	; check value of the data is less than or equal to 50
	CPI r16, 50
	BRLO CONDITION_2P
	BREQ CONDITION_2P

	; if the second condition is not met, the program should 
	; jump back to the beginning of the relevant loop and load the next 
	; value from the input table, i.e., the third specified condition
	rjmp LOOP

CONDITION_2P:
	; if the second condition is met, perform the relevant operation(s)
	; subtract 2 from the value 
	SUBI r16, 2 
	; store this result to the first available location within the output table. 
	rjmp STORE

	; if the second condition was met, the program should
	; unconditionally jump back to the beginning of the relevant loop and
	; load the next value from the input table
	;rjmp LOOP	//<--------
	
	; end of program loop, perform specified cleanup actions
LOOP_END:
	; store specified EOT value
	st Y+,r16
	rjmp DONE

;STORE value to output table
STORE:
	st Y+,r16
	rjmp LOOP

	; end of program (infinite loop)
DONE: 
	rjmp DONE
;*****************************END OF MAIN PROGRAM *****************************
