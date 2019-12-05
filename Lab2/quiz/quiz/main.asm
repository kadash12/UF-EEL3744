;Lab 2 Quiz
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: Quiz
.include "ATxmega128a1udef.inc"

.include "lab2_4.inc"
;******************************END OF INCLUDES*********************************
;******************************DEFINED SYMBOLS*********************************
.equ outhigh=0xff	;set as input or high value
.equ inlow=0x00	;set as output or low value
	.equ IN_TABLE_START_ADDR = 0xAAAA/2
	.equ OUT_TABLE_START_ADDR = 0x3000
	.equ NULL =0
;**************************END OF DEFINED SYMBOLS******************************
;******************************MEMORY CONSTANTS********************************
	; program memory constants (if necessary)
	.cseg

	.org IN_TABLE_START_ADDR

IN_TABLE1:  .db 0b00000100, 0b00000110, 0b00000111, 0b00000000, 0b00000000 


 .db 0b00100000, 0b01100000, 0b11100000, 0b00000000, 0b00000000
	; label used to calculate size of input table

.db 0b00100100, 0b01100110, 0b11100111
	; label used to calculate size of input table
IN_TABLE1_END:

	; data memory allocation
	.dseg

	.org OUT_TABLE_START_ADDR
OUT_TABLE1: .byte (IN_TABLE1_END - IN_TABLE1)
;***************************END OF MEMORY CONSTANTS****************************
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
	ldi ZL, low(IN_TABLE1 << 1) 
	ldi ZH, high(IN_TABLE1 << 1) 
	
	;initialize y
	ldi YL, low(OUT_TABLE1)  
	ldi Yh, high(OUT_TABLE1) 
	ldi r20,16

Loop1:
lpm r16,Z+
st Y+,r16
dec r20
	CPi r20,0 	;compare NULL and r16 to see if NULL is reached
	; if end of table (EOT) has been reached, i.e., the NULL character was 
	; encountered, the program should branch to the relevant label used to
	; terminate the program (i.e., LOOP_END)
	BREQ EDIT
	rjmp LOOP1


	; begin main program loop 
	; "EDIT" mode
EDIT: ;IDLE///////////////////////////
	; check if it is intended that "PLAY" mode be started
	; (determine if the relevant switch has been pressed)
	;read S2
	lds r16, PORTF_IN
	sbrs r16, 3	;check if 3rd bit is set thus S2 is pressed
	rjmp right

	lds r16, PORTF_IN
	sbrs r16, 2	;check if 2rd bit is set thus S2 is pressed
	rjmp Left

	lds r17, PORTA_IN	;store port A input to 
	sbrs r17, 0		;output table.
	rjmp hold

	rjmp edit

Right:
	ldi Yl, low(0x00)  
	ldi Yh, high(0x30)
RighRUn: 
		ld r19, y+;load table
	sts PORTC_OUT, r19	;store switch value to LED portout
		lds r16, PORTF_IN
	sbrs r16, 3	;check if 3rd bit is set thus S2 is pressed
	rjmp edit

	rjmp right

 Left: 
	ldi Yl, low(0x00)  
	ldi Yh, high(0x30) 
LeftRUN:
		ld r19, y+;load table
	sts PORTC_OUT, r19	;store switch value to LED portout

	lds r16, PORTF_IN
	sbrs r16, 2	;check if 3rd bit is set thus S2 is pressed
	rjmp edit
	
	rjmp leftrun

Hold: 
; : .db 0b00100100, 0b01100110, 0b11100111 
	ldi Yl, 0b0100  
	ldi Yh, 0b0010 
		//lds r19, yl;load table
			sts PORTC_OUT, yl	;store switch value to LED portout
		//lds r20,yh
	sts PORTC_OUT, yh	;store switch value to LED portout

		ldi Yl, 0b0110
	ldi Yh, 0b0110 
	//	lds r19, yl;load table
			sts PORTC_OUT, yl	;store switch value to LED portout
		//lds r20,yh
	sts PORTC_OUT, yh	;store switch value to LED portout

			ldi Yl, 0b0111 
	ldi Yh, 0b1110
		//lds r19, yl;load table
			sts PORTC_OUT, yl	;store switch value to LED portout
		//lds r20,yh
	sts PORTC_OUT, yh	;store switch value to LED portout

	lds r17, PORTA_IN	;store port A input to 
	sbrc r17, 0	;check if 3rd bit is set thus S2 is pressed
	rjmp edit
	
	rjmp hold

/*	lds r16, PORTF_IN
	sbrc r16, 2	;check if 2nd bit is set thus S1 is not pressed
	
	; if the "STORE_FRAME" switch was not pressed,
	; branch back to "EDIT"
	rjmp EDIT

	; otherwise, if it was determined that relevant switch was pressed,
	; perform debouncing process, e.g., start relevant timer/counter
	; and wait for it to overflow (write to CTRLA and loop until
	; the OVFIF flag within INTFLAGS is set)*/
/*Timer:
ldi YL, low(OUT_TABLE1)  
	ldi Yh, high(OUT_TABLE1) 

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

	ld r19, Y+	;load table
	sts PORTC_OUT, r19


	lds r16, PORTF_IN
	sbrc r16, 2	;check if 2nd bit is clear set thus S1 is not pressed
	rjmp EDIT	;return to edit

	; wait for the "STORE FRAME" switch to be released
	; before jumping to "EDIT"
STORE_FRAME_SWITCH_RELEASE_WAIT_LOOP:  ;Right
	ldi xL, low(ANIMATION_START_ADDR)  
	ldi xH, high(ANIMATION_START_ADDR) 
	rjmp PLAY_LOOP
	
WAIT_LOOP:
	lds r16, PORTF_IN
	sbrc r16, 2	;check if 2nd bit is clear set thus S1 is not pressed
	rjmp EDIT	;return to edit
	rjmp WAIT_LOOP	;wait till debounced is over
	
	; "PLAY" mode
PLAY:   ;Left
	; reload the relevant index to the first memory location
	; within the animation table to play animation from first frame
	;reload y to memory location
ldi xL, low(OUT_TABLE1)  
	ldi xh, high(OUT_TABLE1) 

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

	; end of program (never reached)*/
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