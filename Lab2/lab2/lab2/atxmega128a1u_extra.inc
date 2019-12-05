;******************************************************************************
;  File name: atxmega128a1u_extra.inc
;  Author: Christopher Crary
;  Last Modified By: Christopher Crary
;  Last Modified On: 06 September 2019
;  Purpose: To define useful, extra symbols for the ATxmega128A1U 
;			architecture, as pertains to EEL3744C.
;
;			NOTE: The use of this file is NOT required! This file is just 
;			given as an example for how to potentially write code more 
;			effectively.
;
;			NOTE: IT SHOULD NOT BE EXPECTED THAT THE USE OF THIS FILE WILL 
;			ALWAYS BE ALLOWED, FOR EXAMPLE, WITHIN EXAM-LIKE ENVIRONMENTS!
;
;		    NOTE: For the assembler within Atmel Studio to be able to 
;			reference this file, make sure that either a copy of it or a link
;			to it is added within the relevant Atmel Studio project. To do so,
;			right click on the project name within the "Solution Explorer",
;			and go to "Add -> Existing Item" (to add a linked version of the
;			file instead of a copied version, within the dialog box that pops
;			open upon clicking "Add -> Existing Item", within the drop-down
;			menu of the "Add" button, click "Add As Link"). Additionally,
;			if you choose to add a folder within the relevant project solution,
;			make sure that there is an additional "include path" set up; 
;			to do so, within "Project -> <Project Name> Properties" (top of
;			Atmel Studio), under "AVR Assembler -> General -> Include Paths" 
;			within "Toolchain" (left-hand side of Properties window), click
;			"Add Item" (the icon that contains a paper with a green plus) and
;			then add the path to the relevant folder. (If you want your project 
;			to be portable, i.e., able to be relocated throughout your computer
;			for whatever reason, make the path relative to the project by
;			ensuring that the "Relative Path" checkbox is selected.)
;******************************************************************************

; the following is what is generally known as a "header guard", and should
; generally be included within all "include" (e.g., "inc") files,
; but not other (e.g., "asm") files.

; a header guard is used to prevent certain definitions within a file from
; being included in an overall program more than once; having multiple 
; definitions for one given symbol is generally ambiguous, and thus, it is 
; common practice for this to be regarded as an error.

; the header guard essentially requires that it be checked whether or not
; a particular symbol (specifically, the one that follows the "ifndef"
; directive) has already been defined, and if this symbol has not
; been yet defined, it along with all that follows the "ifndef" and precedes
; the "endif" located at the end of the file (e.g., any code within the file) 
; will be included in an overall program that includes this file.

; note that the symbol chosen for a header guard is arbitrary but should not
; be common, as to decrease the likelihood of other files using the same symbol.

#ifndef ATXMEGA128A1U_EXTRA_INC		; beginning of header guard
#define ATXMEGA128A1U_EXTRA_INC

;*********************************INCLUDES*************************************
.include "ATxmega128a1udef.inc"
;******************************END OF INCLUDES*********************************

;******************************DEFINED SYMBOLS*********************************

.equ RESET_addr							=	(0x000)

.equ MAIN_PROGRAM_START_ADDR			=	(0xFD)

.equ STACK_POINTER_START_ADDR			=	(0x3FFF)

.equ DEFAULT_SYS_CLOCK_FREQ				=	(2000000)

; below some assembly macro(s) are given. in this course, the use of macros is
; NOT necessary, but doing so can sometimes be helpful (and sometimes hurtful,
; if time is wasted or errors are hidden through them.)

; macros are primarily used to (1) make program code more readable 
; and (2) generalize certain operations. (note that macros made with the 
; intention to generalize certain operations do not necessarily perform the
; most efficient operations.)

; NOTE (NOT IMPORTANT AT ALL): when macros that contain assembly instructions 
; are to be defined, it seems that the ".macro" directive should be used, 
; since it seems that when such macros are defined with the "#define" 
; directive, the assembly code is placed directly within program memory and 
; the intended text replacement is not performed (not completely sure of this, 
; but did not pursue further due to time).

;******************************************************************************
; Name: INIT_STACK_POINTER
; Purpose: To initialize the stack pointer to the highest memory address within
;		   data memory. This routine assumes that register R16 is available
;		   for use. In general, this routine should be called as soon as
;		   possible following a reset of the processor. 
; Input(s): N/A
; Output: N/A
;
; Usage: INIT_STACK_POINTER
;******************************************************************************
.macro INIT_STACK_POINTER

	ldi r16, ((STACK_POINTER_START_ADDR>>0) & 0xFF)		; equivalent to 0xFF
	out CPU_SPL, r16	
	ldi r16, ((STACK_POINTER_START_ADDR>>8) & 0xFF)		; equivalent to 0x3F
	out CPU_SPH, r16
	clr r16

.endmacro												    

;**************************END OF DEFINED SYMBOLS******************************

#endif		; end of header guard

;**********************END OF "atxmega128a1u_extra.inc"************************
