;******************************************************************************
;  File name: lab2_4.inc
;  Author: Christopher Crary
;  Last Modified By: Christopher Crary
;  Last Modified On: 06 September 2019
;  Purpose: To define some useful symbols for the "lab2_4" program.
;
;			NOTE: The use of this file and all other ".inc" files referenced
;			below is NOT required! These files are just given as an example
;			for how to potentially write code more effectively.
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
; be common, as to decrease the likelihood of other include files using the 
; same symbol.

#ifndef LAB2_4_INC		; beginning of header guard
#define LAB2_4_INC


;*********************************INCLUDES*************************************
.include "ATxmega128a1udef.inc"

; files to increase "modularity" of this file
.include "atxmega128a1u_extra.inc"
.include "misc.inc"
.include "ootb_upad_platform.inc"
;******************************END OF INCLUDES*********************************

;******************************DEFINED SYMBOLS*********************************

; NOTE: THE FOLLOWING SYMBOLS ARE IMPLEMENTED PRIMARILY FOR THE PURPOSE OF
;		MAKING THE APPLICATION CODE MORE READABLE. THEY ARE NOT NECESSARY!

.equ ANIMATION_START_ADDR				=	INTERNAL_SRAM_START	; = 0x2000
.equ ANIMATION_SIZE						=	INTERNAL_SRAM_SIZE	; = 0x2000

; below, "bit position symbols" and "bit mask symbols" are provided. 
; the use of these symbols allow for both easier readability of code
; and a reduced amount changes to code if another system was to be used
; (these concerns are not necessary for our course, although they are
;  in general, so exposure is provided).

; a "bit position symbol" (or just "bit position") is meant to be a symbol
; that meaningfully identifies the position of some singular bit within a set 
; of bits. a bit position is used to more easily define a "bit mask symbol" 
; (see below).
; a bit position symbol is given a suffix of "_bp".

; a "bit mask symbol" (or just "bit mask") is meant to be a symbol that
; meaningfully identifies the corresponding value of some set of bits that has 
; a singular bit set to one.
; a bit mask symbol is given a suffix of "_bm".

; a "group mask symbol" (or just "group mask") is meant to be a symbol that
; meaningfully identifies the corresponding value of some number that has 
; some non-singular subset of bits set to one. 
; a group mask is used to perform "bit masking" (see above).
; a group mask symbol is given a suffix of "_gm".

; NOTE: both "bit masks" and "group masks" are typically used to perform 
; what is known as "bit masking", where a logical AND operation is performed 
; between some set of bits and any appropriate "mask" (either a bit mask or 
; group mask); ultimately, this procedure often readily allows for some subset
; of bits (or also often, the relevant complement of these bits) to be 
; manipulated.

.equ STORE_FRAME_SWITCH_bp				=	OOTB_SLB_S1_bp
.equ STORE_FRAME_SWITCH_bm				=	(1<<STORE_FRAME_SWITCH_bp)

.equ CHANGE_MODE_SWITCH_EDIT_bp			=	OOTB_MB_S1_bp
.equ CHANGE_MODE_SWITCH_EDIT_bm			=	(1<<CHANGE_MODE_SWITCH_EDIT_bp)

.equ CHANGE_MODE_SWITCH_PLAY_bp			=	OOTB_MB_S2_bp
.equ CHANGE_MODE_SWITCH_PLAY_bm			=	(1<<CHANGE_MODE_SWITCH_PLAY_bp)

.equ ANIMATION_DISPLAY_gm				=	OOTB_SLB_LED_gm

; below are some symbols to be used when configuring the relevant 
; timer/counters.

; note that the reciprocals of each period (i.e., the corresponding
; frequency) are given and that each period is calculated with this reciprocal 
; form since the equivalent period values would need a floating-point
; representation and thus, would be truncated by the preprocessor.

.equ FRAME_RATE_TC_FREQ				=	(20)
.equ FRAME_RATE_TC_PRE				=	(256)
.equ FRAME_RATE_TC_PER_VAL			=	(DEFAULT_SYS_CLOCK_FREQ/ \
										 FRAME_RATE_TC_PRE)/ \
										(FRAME_RATE_TC_FREQ)

.equ DEBOUNCE_TC_FREQ				=	(50)
.equ DEBOUNCE_TC_PRE				=	(256)
.equ DEBOUNCE_TC_PER_VAL			=	(DEFAULT_SYS_CLOCK_FREQ/\
										 FRAME_RATE_TC_PRE)/\
										(FRAME_RATE_TC_FREQ)

; below, some symbols are given to provide abstraction to port definitions,
; which are defined in "ATxmega128a1udef.inc". 

; NOTE (NOT IMPORTANT AT ALL): ideally, a single "#define" directive would 
; be used to succintly map the following symbols onto pre-existing 
; symbols, e.g., #define OOTB_SLB_LED_PORT(reg) (PORTC_##reg), however since 
; it does not seem that the current AVR assembler supports recursive macro
; expansions in conjunction with macros defined by the ".macro" directive 
; (which are desired for reasons explained in "atxmega128a1u_extra.inc"), 
; the following manner is chosen instead.

; this kind of abstraction is OVERKILL for this minor application,
; but is given in preparation for the rest of the course

; note that the following is MUCH simpler in the C programming language

.equ DEBOUNCE_TC_CTRLA					=	TCC0_CTRLA
.equ DEBOUNCE_TC_CTRLB					=	TCC0_CTRLB
.equ DEBOUNCE_TC_CTRLC					=	TCC0_CTRLC
.equ DEBOUNCE_TC_CTRLD					=	TCC0_CTRLD
.equ DEBOUNCE_TC_CTRLE					=	TCC0_CTRLE
.equ DEBOUNCE_TC_INTCTRLA				=	TCC0_INTCTRLA
.equ DEBOUNCE_TC_INTCTRLB				=	TCC0_INTCTRLB
.equ DEBOUNCE_TC_CTRLFCLR				=	TCC0_CTRLFCLR
.equ DEBOUNCE_TC_CTRLFSET				=	TCC0_CTRLFSET
.equ DEBOUNCE_TC_CTRLGCLR				=	TCC0_CTRLGCLR
.equ DEBOUNCE_TC_CTRLGSET				=	TCC0_CTRLGSET
.equ DEBOUNCE_TC_INTFLAGS				=	TCC0_INTFLAGS
.equ DEBOUNCE_TC_TEMP					=	TCC0_TEMP
.equ DEBOUNCE_TC_CNT					=	TCC0_CNT
.equ DEBOUNCE_TC_PER					=	TCC0_PER
.equ DEBOUNCE_TC_CCA					=	TCC0_CCA
.equ DEBOUNCE_TC_CCB					=	TCC0_CCB
.equ DEBOUNCE_TC_CCC					=	TCC0_CCC
.equ DEBOUNCE_TC_CCD					=	TCC0_CCD
.equ DEBOUNCE_TC_PERBUF					=	TCC0_PERBUF
.equ DEBOUNCE_TC_CCABUF					=	TCC0_CCABUF
.equ DEBOUNCE_TC_CCBBUF					=	TCC0_CCBBUF
.equ DEBOUNCE_TC_CCCBUF					=	TCC0_CCCBUF
.equ DEBOUNCE_TC_CCDBUF					=	TCC0_CCDBUF

.equ FRAME_RATE_TC_CTRLA				=	TCC1_CTRLA
.equ FRAME_RATE_TC_CTRLB				=	TCC1_CTRLB
.equ FRAME_RATE_TC_CTRLC				=	TCC1_CTRLC
.equ FRAME_RATE_TC_CTRLD				=	TCC1_CTRLD
.equ FRAME_RATE_TC_CTRLE				=	TCC1_CTRLE
.equ FRAME_RATE_TC_INTCTRLA				=	TCC1_INTCTRLA
.equ FRAME_RATE_TC_INTCTRLB				=	TCC1_INTCTRLB
.equ FRAME_RATE_TC_CTRLFCLR				=	TCC1_CTRLFCLR
.equ FRAME_RATE_TC_CTRLFSET				=	TCC1_CTRLFSET
.equ FRAME_RATE_TC_CTRLGCLR				=	TCC1_CTRLGCLR
.equ FRAME_RATE_TC_CTRLGSET				=	TCC1_CTRLGSET
.equ FRAME_RATE_TC_INTFLAGS				=	TCC1_INTFLAGS
.equ FRAME_RATE_TC_TEMP					=	TCC1_TEMP
.equ FRAME_RATE_TC_CNT					=	TCC1_CNT
.equ FRAME_RATE_TC_PER					=	TCC1_PER
.equ FRAME_RATE_TC_CCA					=	TCC1_CCA
.equ FRAME_RATE_TC_CCB					=	TCC1_CCB
.equ FRAME_RATE_TC_PERBUF				=	TCC1_PERBUF
.equ FRAME_RATE_TC_CCABUF				=	TCC1_CCABUF
.equ FRAME_RATE_TC_CCBBUF				=	TCC1_CCBBUF

;**************************END OF DEFINED SYMBOLS******************************

#endif		; end of header guard

;***************************END OF "lab2_4.inc"********************************
