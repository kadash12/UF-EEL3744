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
.equ SRAM = 0x170000
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
	sts CPU_RAMPX, r16			; use the CPU_RAMPX register to set the third byte of the pointer

	ldi XL, low(IO)
	ldi XH, high(IO)		; set the middle (XH) and low (XL) bytes of the pointer as usual

TEST:
	ld r16, X							; read the input port into r16
	nop
	nop
	st X, r16
	rjmp TEST							; put a breakpoint on me and check r16!

	;End of program (never reached)
DONE: 
	rjmp DONE
;*****************************END OF MAIN PROGRAM *****************************
;********************************SUBROUTINES***********************************
; Name: Subroutines
; Purpose: 
; Input(s): N/A
; Output: N/A
;******************************************************************************
Subroutines:
	ret
;*****************************END OF SUBROUTINES*******************************
;*****************************END OF "lab4_2.asm"******************************
