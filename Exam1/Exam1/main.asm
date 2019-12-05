;******************************************************************************
;Lab 4 Part 2
;Section #: 1823
;Name: Johnny Li
;Class #: 12378
;PI Name: Jared Holley
;Description: Interfacing with External I/O Ports
;*********************************INCLUDES*************************************
.include "ATxmega128a1udef.inc"
;******************************END OF INCLUDES*********************************
;******************************DEFINED SYMBOLS*********************************
.equ SRAM = 0x4000
.equ SRAM1 = 0x3FFF
.equ SRAM2 = 0xBFFF
.equ SRAM3 = 0xC000
.equ IO = 0x037000
;**************************END OF DEFINED SYMBOLS******************************
;******************************MEMORY CONSTANTS********************************
	; data memory allocation
	/*.dseg
	.org DATA_ADDR		;For table
DATA:
	.byte DATA_END		; TABLE SIZE*/
;***************************END OF MEMORY CONSTANTS****************************
;********************************MAIN PROGRAM**********************************
	.cseg
.org 0x0000
	rjmp MAIN

.org 0x100
MAIN:
	;switch
	; S1
	ldi r16, 0x01<<2
	sts PORTF_DIRCLR, r16
	; S2
	ldi r16, 0x01<<3
	sts PORTF_DIRCLR, r16

	ldi r16, 0b00110011		; Since RE(L), WE(L), CS0(L) and CS1(L) are active low signals, we must set  
	sts PORTH_OUTSET, r16	; the default output to 1 = H = false. See 8331, sec 27.9.
							; (ALE defaults to 0 = L = false)

	ldi r16, 0b00110111		; Configure the PORTH bits 4, 2 and 1 as outputs. 
	sts PORTH_DIRSET, r16 	; These are the CS1(L), CS0(L), ALE1(H), WE(L) and RE(L) outputs. 
							; (CS0 is bit 4; ALE1 is bit 2; RE is bit 1)
							; see 8385, Table 33-7
	
	ldi r16, 0xFF			; Set all PORTK pins (A15-A0) to be outputs. As requried	
	sts PORTK_DIRSET, r16	; in the data sheet. See 8331, sec 27.9.

	ldi r16, 0xFF			; Set all PORTJ pins (D7-D0) to be outputs. As requried 
	sts PORTJ_DIRSET, r16	; in the data sheet. See 8331, sec 27.9.
		
	ldi r16, 0x01			; Store 0x01 in EBI_CTRL register to select 3 port EBI(H,J,K) 
	sts EBI_CTRL, r16		; mode and SRAM ALE1 mode.

;Reserve a chip-select zone for our input port. The base address register is made up of
;  12 bits for the address (A23:A12). The lower 12 bits of the address (A11-A0) are 
;  assumed to be zero. This limits our choice of the base addresses.
; point appropriate indices to input/output tables

	; Set to size chip select space and turn on SRAM mode.
	ldi r16, 0b00011101	;32k		
	sts EBI_CS0_CTRLA, r16		

;Load the middle byte (A15:8) of the three byte address into a register and store it as the 
;  LOW Byte of the Base Address, BASEADDRL.  This will store only bits A15:A12 and ignore 
;  anything in A11:8 as again, they are assumed to be zero. 
	ldi r16, byte2(SRAM)
	sts EBI_CS0_BASEADDR, r16

;Load the highest byte (A23:16) of the three byte address into a register and store it as the 
;  HIGH byte of the Base Address, BASEADDRH.
	ldi r16, byte3(SRAM)
	sts EBI_CS0_BASEADDR+1, r16

;Load the middle byte (A15:8) of the three byte address into a register and store it as the 
;  LOW Byte of the Base Address, BASEADDRL.  This will store only bits A15:A12 and ignore 
;  anything in A11:8 as again, they are assumed to be zero. 
	ldi r16, byte2(IO)
	sts EBI_CS1_BASEADDR, r16

;Load the highest byte (A23:16) of the three byte address into a register and store it as the 
;  HIGH byte of the Base Address, BASEADDRH.
	ldi r16, byte3(IO)
	sts EBI_CS1_BASEADDR+1, r16
	
; Set to 32K chip select space and turn on SRAM mode.
	ldi r16, 0x01	;256			
	sts EBI_CS1_CTRLA, r16		
	
	ldi r16, byte3(IO)		; initalize a pointer to point to the base address of the SRAM
	sts CPU_RAMPZ, r16			; use the CPU_RAMPX register to set the third byte of the pointer

	ldi ZL, low(IO)
	ldi ZH, high(IO)		; set the middle (XH) and low (XL) bytes of the pointer as usual			

TEST0:
	;X points to input table.
	ldi YL, low(SRAM) ; Shift due to being 16-bits in a 8-bit processor 
	ldi YH, high(SRAM) ; Z points to the table
	ldi r16, BYTE3(SRAM)
    sts CPU_RAMPY,r16
TEST:
	; load value from input table into an appropriate register
	elpm r16,Z
	;Store in SRAM
	st Y, r16

	//READ to LED
	ld r16,Y
	st Z,r16

	;initialization of I/O
	lds r16, PORTF_IN
	sbrs r16, 2	;check if 2rd bit is set thus S1 is pressed
	rcall COMP
	ld r16, Z							; read the input port into r16
	nop
	nop
	st Z, r16
	lds r16, PORTF_IN
	sbrs r16, 3	;check if 3rd bit is set thus S2 is pressed
	rjmp TEST11							; put a breakpoint on me and check r16!
	rjmp TEST

	;End of program (never reached)
DONE: 
	rjmp DONE
;*****************************END OF MAIN PROGRAM *****************************
;*****************************END OF SUBROUTINES*******************************
 COMP: 
	ld r16, Z
	com r16							; read the input port into r16
	nop
	nop
	st Z, r16
	lds r16, PORTF_IN
	sbrs r16, 2	;check if 2rd bit is set thus S1 is not pressed
	rjmp COMP							; put a breakpoint on me and check r16!
 ret



 TEST11:
	;X points to input table.
	ldi YL, low(SRAM1) ; Shift due to being 16-bits in a 8-bit processor 
	ldi YH, high(SRAM1) ; Z points to the table
	ldi r16, BYTE3(SRAM1)
    sts CPU_RAMPY,r16
TEST1:
	; load value from input table into an appropriate register
	elpm r16,Z
	;Store in SRAM
	st Y, r16

	//READ to LED
	ld r16,Y
	st Z,r16

	;initialization of I/O
	lds r16, PORTF_IN
//	sbrs r16, 2	;check if 2rd bit is set thus S1 is pressed
	//rcall COMP
	ldi r16, 0xFF							; read the input port into r16
	nop
	nop
	st Z, r16
	lds r16, PORTF_IN
	sbrs r16, 3	;check if 3rd bit is set thus S2 is pressed
	rjmp TEST22							; put a breakpoint on me and check r16!
	rjmp TEST1							; put a breakpoint on me and check r16!

TEST22:
	;X points to input table.
	ldi YL, low(SRAM2) ; Shift due to being 16-bits in a 8-bit processor 
	ldi YH, high(SRAM2) ; Z points to the table
	ldi r16, BYTE3(SRAM2)
    sts CPU_RAMPY,r16
TEST2:
	; load value from input table into an appropriate register
	elpm r16,Z
	;Store in SRAM
	st Y, r16

	//READ to LED
	ld r16,Y
	st Z,r16

	;initialization of I/O
	lds r16, PORTF_IN
	sbrs r16, 2	;check if 2rd bit is set thus S1 is pressed
	rcall COMP
	ld r16, Z							; read the input port into r16
	nop
	nop
	st Z, r16
	lds r16, PORTF_IN
	sbrs r16, 3	;check if 3rd bit is set thus S2 is pressed
	rjmp TEST33							; put a breakpoint on me and check r16!
	rjmp TEST2							; put a breakpoint on me and check r16!

TEST33:
	;X points to input table.
	ldi YL, low(SRAM3) ; Shift due to being 16-bits in a 8-bit processor 
	ldi YH, high(SRAM3) ; Z points to the table
	ldi r16, BYTE3(SRAM3)
    sts CPU_RAMPY,r16
TEST3:
	; load value from input table into an appropriate register
	elpm r16,Z
	;Store in SRAM
	st Y, r16

	//READ to LED
	ld r16,Y
	st Z,r16

	;initialization of I/O
	lds r16, PORTF_IN
	//sbrs r16, 2	;check if 2rd bit is set thus S1 is pressed
//	rcall COMP
	ldi r16, 0xFF							; read the input port into r16
	nop
	nop
	st Z, r16
	lds r16, PORTF_IN
	sbrs r16, 3	;check if 3rd bit is set thus S2 is pressed
	rjmp TEST0						; put a breakpoint on me and check r16!
	rjmp TEST3							; put a breakpoint on me and check r16!
;*****************************END OF "lab4_2.asm"******************************
