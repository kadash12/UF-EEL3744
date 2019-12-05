;******************************************************************************
;  File name: lab2_4.asm
;  Author: Christopher Crary
;  Last Modified By: Christopher Crary
;  Last Modified On: 06 September 2019
;  Purpose: To allow LED animations to be created with the OOTB µPAD, 
;			OOTB SLB, and OOTB MB (or EBIBB, if a previous version of the kit
;			is used).
;
;			NOTE: The use of this file is NOT required! This file is just given
;			as an example for how to potentially write code more effectively.
;******************************************************************************
;*********************************INCLUDES*************************************
; the inclusion of the following file is REQUIRED for our course, since
; it is intended that you understand concepts regarding how to specify an 
; "include file" to an assembler. (this is also included within "lab2_4.inc"
; provided below, so it is not necessary here, but it is done for good 
; measure.)
.include "ATxmega128a1udef.inc"

; certain aspects of this overall program are placed into the following
; "include" file, for the purpose of potentially increasing both the 
; readability of the application code and the "modularity" of the application.
; the use of this (or any) "include" file is NOT required for our course!
.include "lab2_4.inc"
;******************************END OF INCLUDES*********************************
;******************************DEFINED SYMBOLS*********************************
.equ outhigh=0xff	;set as input or high value
.equ inlow=0x00	;set as output or low value
;**************************END OF DEFINED SYMBOLS******************************
;******************************MEMORY CONSTANTS********************************
	; data memory allocation
	.dseg

	.org ANIMATION_START_ADDR		; ANIMATION_START_ADDR = 0x2000
ANIMATION:
	.byte ANIMATION_SIZE			; ANIMATION_SIZE = 0x2000

;***************************END OF MEMORY CONSTANTS****************************
;Lab 2 Part 4
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: LED Animation Creator
;********************************MAIN PROGRAM**********************************
	.cseg

	; upon system reset, jump to main program (instead of executing
	; instructions meant for interrupt vectors)
	.org RESET_addr		; RESET_addr = 0x000
	rjmp MAIN

	; place the main program somewhere after interrupt vectors (ignore for now)
	.org MAIN_PROGRAM_START_ADDR		; MAIN_PROGRAM_START_ADDR = 0xFD
MAIN:
	; initialize the stack pointer
	INIT_STACK_POINTER	; macro defined in "atxmega128a1u_extra.inc"		

	; initialize relevant I/O modules (switches and LEDs)
	rcall IO_INIT

	; initialize (but do not start) the relevant timer/counter module(s)
	rcall TC_INIT

	; initialize the X and Y indices to point to the 
	; beginning of the animation table (although one pointer could be used 
	; to both store frames and playback the current animation, it is
	; simpler to utilize a separate index for each of these operations)
	; note: recognize that the animation table is in DATA memory
	;initialize X
	ldi XL, low(ANIMATION_START_ADDR)  
	ldi XH, high(ANIMATION_START_ADDR) 
	;initialize y
	ldi YL, low(ANIMATION_START_ADDR)  
	ldi YH, high(ANIMATION_START_ADDR) 

	; begin main program loop 
	; "EDIT" mode
EDIT:
	; check if it is intended that "PLAY" mode be started
	; (determine if the relevant switch has been pressed)
	;read S2
	lds r16, PORTF_IN
	sbrs r16, 3	;check if 3rd bit is set thus S2 is pressed

	; if it is determined that relevant switch was pressed, 
	; go to "PLAY" mode
	rjmp PLAY

	; otherwise, if the "PLAY" mode switch was not pressed,
	; update display LEDs with the voltage values from relevant DIP switches
	; and check if it is intended that a frame be stored in the animation
	; (determine if this relevant switch has been pressed)
	lds r16, PORTA_IN	;load port A value to r16
    sts PORTC_OUT, r16	;store switch value to LED portout

	lds r16, PORTF_IN
	sbrc r16, 2	;check if 2nd bit is set thus S1 is not pressed
	
	; if the "STORE_FRAME" switch was not pressed,
	; branch back to "EDIT"
	rjmp EDIT

	; otherwise, if it was determined that relevant switch was pressed,
	; perform debouncing process, e.g., start relevant timer/counter
	; and wait for it to overflow (write to CTRLA and loop until
	; the OVFIF flag within INTFLAGS is set)
Timer:
	ldi r18, 6
	sts TCC0_CTRLA, r18		;increment and set prescaler 256

	lds r17, TCC0_INTFLAGS	;load flag	
	sbrs r17, 0	;check if flag is triggered
	rjmp Timer	;continue delay

	; after relevant timer/counter has overflowed (i.e., after
	; the relevant debounce period), disable this timer/counter,
	; clear the relevant timer/counter OVFIF flag,
	; and then read switch value again to verify that it was
	; actually pressed -- if so, perform intended functionality, and
	; otherwise, do not; however, in both cases, wait for switch to
	; be released before jumping back to "EDIT"
	ldi r18, 0	;disable timer
	sts TCC0_CTRLA, r18	

	ldi r18,0	;reset count
	sts TCC0_CNT, r18
	sts TCC0_CNT+1, r18

	ldi r18,1	;reset flag
	sts TCC0_INTFLAGS, r18
	//////////////////////////////////////////////////////////////////
	lds r16, PORTF_IN
	sbrc r16, 2	;check if 2nd bit is clear set thus S1 is not pressed
	rjmp EDIT	;return to edit

	; wait for the "STORE FRAME" switch to be released
	; before jumping to "EDIT"
STORE_FRAME_SWITCH_RELEASE_WAIT_LOOP:
	lds r17, PORTA_IN	;store port A input to 
	st x+, r17	;output table.
	
WAIT_LOOP:
	lds r16, PORTF_IN
	sbrc r16, 2	;check if 2nd bit is clear set thus S1 is not pressed
	rjmp EDIT	;return to edit
	rjmp WAIT_LOOP	;wait till debounced is over
	
	; "PLAY" mode
PLAY:
	; reload the relevant index to the first memory location
	; within the animation table to play animation from first frame
	;reload y to memory location
	ldi YL, low(ANIMATION_START_ADDR)  
	ldi YH, high(ANIMATION_START_ADDR) 
	rjmp PLAY_LOOP

PLAY_LOOP:
	; check if it is intended that "EDIT" mode be started
	; i.e., check if the relevant switch has been pressed
	lds r16, PORTF_IN
	sbrs r16, 2	;check if 2nd bit is set thus S1 is pressed

	; if it is determined that relevant switch was pressed, 
	; go to "EDIT" mode
	rjmp EDIT	;return to edit

	; otherwise, if the "EDIT" mode switch was not pressed,
	; determine if index used to load frames has the same
	; address as the index used to store frames, i.e., if the end
	; of the animation has been reached during playback
	; (placing this check here will allow animations of all sizes,
	; including zero, to playback properly).
	; to efficiently determine if these index values are equal,
	; a combination of the "CP" and "CPC" instructions is recommended
	cp XL,YL	;compare lower half
	cpc XH, YH	;compare upper half

	; if index values are equal, branch back to "PLAY" to
	; restart the animation
	breq PLAY

	; otherwise, load animation frame from table, 
	; display this "frame" on the relevant LEDs,
	; start relevant timer/counter,
	; wait until this timer/counter overflows (to more or less
	; achieve the "frame rate"), and then after the overflow,
	; stop the timer/counter,
	; clear the relevant OVFIF flag,
	; and then jump back to "PLAY_LOOP"
	ld r19, Y+	;load table
	sts PORTC_OUT, r19	;store switch value to LED portout

TimerPLAY:
	ldi r18, 6
	sts TCC0_CTRLA, r18		;increment and set prescaler 256

	lds r17, TCC0_INTFLAGS	;load flag	
	sbrs r17, 0	;check if flag is triggered
	rjmp TimerPLAY	;continue delay

	ldi r18, 0	;disable timer
	sts TCC0_CTRLA, r18	

	ldi r18,0	;reset count
	sts TCC0_CNT, r18
	sts TCC0_CNT+1, r18

	ldi r18,1	;reset flag
	sts TCC0_INTFLAGS, r18

	rjmp PLAY_LOOP

	; end of program (never reached)
DONE: 
	rjmp DONE
;*****************************END OF MAIN PROGRAM *****************************
;********************************SUBROUTINES***********************************
; Name: IO_INIT 
; Purpose: To initialize the relevant input/output modules, as pertains to the
;		   application.
; Input(s): N/A
; Output: N/A
;******************************************************************************
IO_INIT:
	; protect relevant registers
	push r16
	; initialize the relevant I/O
	; Switch
	ldi r16, inlow	;load 0 to register
    sts PORTA_DIR,r16	;set portA as input
	; LED
	ldi r16, outhigh	;load 1 to register
	sts PORTC_OUT,r16	;set portC initally off
	sts PORTC_DIR,r16	;set portC as output
	; S1
	ldi r16, 0x01<<2
	sts PORTF_DIRCLR, r16
	; S2
	ldi r16, 0x01<<3
	sts PORTF_DIRCLR, r16
	; recover relevant registers
	pop r16
	; return from subroutine
	ret
;******************************************************************************
; Name: TC_INIT 
; Purpose: To initialize the relevant timer/counter modules, as pertains to
;		   application.
; Input(s): N/A
; Output: N/A
;******************************************************************************
TC_INIT:
	; protect relevant registers

	; initialize the relevant TC modules
	ldi ZL, 0x87
	ldi ZH, 0x01
	sts TCC0_PER, ZL		;load period tick
	sts TCC0_PER+1, ZH
	
	; recover relevant registers

	; return from subroutine
	ret
;*****************************END OF SUBROUTINES*******************************
;*****************************END OF "lab2_4.asm"******************************