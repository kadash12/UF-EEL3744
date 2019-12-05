;******************************************************************************
;  File name: ootb_upad_platform.inc
;  Author: Christopher Crary
;  Last Modified By: Christopher Crary
;  Last Modified On: 06 September 2019
;  Purpose: To define useful symbols for all things related to 
;			OOTB µPAD Platform.
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

#ifndef OOTB_UPAD_PLATFORM_INC		; beginning of header guard
#define OOTB_UPAD_PLATFORM_INC

;*********************************INCLUDES*************************************
;******************************END OF INCLUDES*********************************

;******************************DEFINED SYMBOLS*********************************

; below are "bit position symbols", "bit mask symbols", "group mask symbols",
; and other miscellaneous symbols to provide abstraction to components
; supported within the OOTB platform.

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

; below, some symbols are given to provide abstraction to port definitions,
; which are defined in "ATxmega128a1udef.inc". 

; NOTE (NOT IMPORTANT AT ALL): ideally, a single "#define" directive would 
; be used to succintly map the following symbols onto pre-existing 
; symbols, e.g., #define OOTB_SLB_LED_PORT(reg) (PORTC_##reg), however since 
; it does not seem that the current AVR assembler supports recursive macro
; expansions in conjunction with macros defined by the ".macro" directive 
; (which are desired for reasons explained in "atxmega128a1u_extra.inc"), 
; the following manner is chosen instead.

; SLB tactile switch (TS) symbols

.equ OOTB_SLB_S1_bp						=	(2)
.equ OOTB_SLB_S1_bm						=	(1<<OOTB_SLB_S1_bp)

.equ OOTB_SLB_S2_bp						=	(3)
.equ OOTB_SLB_S2_bm						=	(1<<OOTB_SLB_S2_bp)

.equ OOTB_SLB_TS_gm						=	(OOTB_SLB_S1_bm | OOTB_SLB_S2_bm)

.equ OOTB_SLB_TS_PORT_DIR				=	PORTF_DIR
.equ OOTB_SLB_TS_PORT_DIRSET			=	PORTF_DIRSET
.equ OOTB_SLB_TS_PORT_DIRCLR			=	PORTF_DIRCLR
.equ OOTB_SLB_TS_PORT_DIRTGL			=	PORTF_DIRTGL
.equ OOTB_SLB_TS_PORT_OUT				=	PORTF_OUT
.equ OOTB_SLB_TS_PORT_OUTSET			=	PORTF_OUTSET
.equ OOTB_SLB_TS_PORT_OUTCLR			=	PORTF_OUTCLR
.equ OOTB_SLB_TS_PORT_OUTTGL			=	PORTF_OUTTGL
.equ OOTB_SLB_TS_PORT_IN				=	PORTF_IN
.equ OOTB_SLB_TS_PORT_INTCTRL			=	PORTF_INTCTRL
.equ OOTB_SLB_TS_PORT_INT0MASK			=	PORTF_INT0MASK
.equ OOTB_SLB_TS_PORT_INT1MASK			=	PORTF_INT1MASK
.equ OOTB_SLB_TS_PORT_INTFLAGS			=	PORTF_INTFLAGS
.equ OOTB_SLB_TS_PORT_REMAP				=	PORTF_REMAP
.equ OOTB_SLB_TS_PORT_PIN0CTRL			=	PORTF_PIN0CTRL
.equ OOTB_SLB_TS_PORT_PIN1CTRL			=	PORTF_PIN1CTRL
.equ OOTB_SLB_TS_PORT_PIN2CTRL			=	PORTF_PIN2CTRL
.equ OOTB_SLB_TS_PORT_PIN3CTRL			=	PORTF_PIN3CTRL
.equ OOTB_SLB_TS_PORT_PIN4CTRL			=	PORTF_PIN4CTRL
.equ OOTB_SLB_TS_PORT_PIN5CTRL			=	PORTF_PIN5CTRL
.equ OOTB_SLB_TS_PORT_PIN6CTRL			=	PORTF_PIN6CTRL
.equ OOTB_SLB_TS_PORT_PIN7CTRL			=	PORTF_PIN7CTRL

; SLB DIP switch symbols

.equ OOTB_SLB_DIP_0_bp					=	(0)
.equ OOTB_SLB_DIP_0_bm					=	(1<<OOTB_SLB_DIP_0_bp)
.equ OOTB_SLB_DIP_1_bp					=	(1)
.equ OOTB_SLB_DIP_1_bm					=	(1<<OOTB_SLB_DIP_1_bp)
.equ OOTB_SLB_DIP_2_bp					=	(2)
.equ OOTB_SLB_DIP_2_bm					=	(1<<OOTB_SLB_DIP_2_bp)
.equ OOTB_SLB_DIP_3_bp					=	(3)
.equ OOTB_SLB_DIP_3_bm					=	(1<<OOTB_SLB_DIP_3_bp)
.equ OOTB_SLB_DIP_4_bp					=	(4)
.equ OOTB_SLB_DIP_4_bm					=	(1<<OOTB_SLB_DIP_4_bp)
.equ OOTB_SLB_DIP_5_bp					=	(5)
.equ OOTB_SLB_DIP_5_bm					=	(1<<OOTB_SLB_DIP_5_bp)
.equ OOTB_SLB_DIP_6_bp					=	(6)
.equ OOTB_SLB_DIP_6_bm					=	(1<<OOTB_SLB_DIP_6_bp)
.equ OOTB_SLB_DIP_7_bp					=	(7)
.equ OOTB_SLB_DIP_7_bm					=	(1<<OOTB_SLB_DIP_7_bp)

.equ OOTB_SLB_DIP_gm					=	(0xFF)

.equ OOTB_SLB_DIP_PORT_DIR				=	PORTA_DIR
.equ OOTB_SLB_DIP_PORT_DIRSET			=	PORTA_DIRSET
.equ OOTB_SLB_DIP_PORT_DIRCLR			=	PORTA_DIRCLR
.equ OOTB_SLB_DIP_PORT_DIRTGL			=	PORTA_DIRTGL
.equ OOTB_SLB_DIP_PORT_OUT				=	PORTA_OUT
.equ OOTB_SLB_DIP_PORT_OUTSET			=	PORTA_OUTSET
.equ OOTB_SLB_DIP_PORT_OUTCLR			=	PORTA_OUTCLR
.equ OOTB_SLB_DIP_PORT_OUTTGL			=	PORTA_OUTTGL
.equ OOTB_SLB_DIP_PORT_IN				=	PORTA_IN
.equ OOTB_SLB_DIP_PORT_INTCTRL			=	PORTA_INTCTRL
.equ OOTB_SLB_DIP_PORT_INT0MASK			=	PORTA_INT0MASK
.equ OOTB_SLB_DIP_PORT_INT1MASK			=	PORTA_INT1MASK
.equ OOTB_SLB_DIP_PORT_INTFLAGS			=	PORTA_INTFLAGS
.equ OOTB_SLB_DIP_PORT_REMAP			=	PORTA_REMAP
.equ OOTB_SLB_DIP_PORT_PIN0CTRL			=	PORTA_PIN0CTRL
.equ OOTB_SLB_DIP_PORT_PIN1CTRL			=	PORTA_PIN1CTRL
.equ OOTB_SLB_DIP_PORT_PIN2CTRL			=	PORTA_PIN2CTRL
.equ OOTB_SLB_DIP_PORT_PIN3CTRL			=	PORTA_PIN3CTRL
.equ OOTB_SLB_DIP_PORT_PIN4CTRL			=	PORTA_PIN4CTRL
.equ OOTB_SLB_DIP_PORT_PIN5CTRL			=	PORTA_PIN5CTRL
.equ OOTB_SLB_DIP_PORT_PIN6CTRL			=	PORTA_PIN6CTRL
.equ OOTB_SLB_DIP_PORT_PIN7CTRL			=	PORTA_PIN7CTRL

; SLB LED symbols

.equ OOTB_SLB_LED_0_bp					=	(0)
.equ OOTB_SLB_LED_0_bm					=	(1<<OOTB_SLB_LED_0_bp)
.equ OOTB_SLB_LED_1_bp					=	(1)
.equ OOTB_SLB_LED_1_bm					=	(1<<OOTB_SLB_LED_1_bp)
.equ OOTB_SLB_LED_2_bp					=	(2)
.equ OOTB_SLB_LED_2_bm					=	(1<<OOTB_SLB_LED_2_bp)
.equ OOTB_SLB_LED_3_bp					=	(3)
.equ OOTB_SLB_LED_3_bm					=	(1<<OOTB_SLB_LED_3_bp)
.equ OOTB_SLB_LED_4_bp					=	(4)
.equ OOTB_SLB_LED_4_bm					=	(1<<OOTB_SLB_LED_4_bp)
.equ OOTB_SLB_LED_5_bp					=	(5)
.equ OOTB_SLB_LED_5_bm					=	(1<<OOTB_SLB_LED_5_bp)
.equ OOTB_SLB_LED_6_bp					=	(6)
.equ OOTB_SLB_LED_6_bm					=	(1<<OOTB_SLB_LED_6_bp)
.equ OOTB_SLB_LED_7_bp					=	(7)
.equ OOTB_SLB_LED_7_bm					=	(1<<OOTB_SLB_LED_7_bp)

.equ OOTB_SLB_LED_gm					=	(0xFF)

.equ OOTB_SLB_LED_PORT_DIR				=	PORTC_DIR
.equ OOTB_SLB_LED_PORT_DIRSET			=	PORTC_DIRSET
.equ OOTB_SLB_LED_PORT_DIRCLR			=	PORTC_DIRCLR
.equ OOTB_SLB_LED_PORT_DIRTGL			=	PORTC_DIRTGL
.equ OOTB_SLB_LED_PORT_OUT				=	PORTC_OUT
.equ OOTB_SLB_LED_PORT_OUTSET			=	PORTC_OUTSET
.equ OOTB_SLB_LED_PORT_OUTCLR			=	PORTC_OUTCLR
.equ OOTB_SLB_LED_PORT_OUTTGL			=	PORTC_OUTTGL
.equ OOTB_SLB_LED_PORT_IN				=	PORTC_IN
.equ OOTB_SLB_LED_PORT_INTCTRL			=	PORTC_INTCTRL
.equ OOTB_SLB_LED_PORT_INT0MASK			=	PORTC_INT0MASK
.equ OOTB_SLB_LED_PORT_INT1MASK			=	PORTC_INT1MASK
.equ OOTB_SLB_LED_PORT_INTFLAGS			=	PORTC_INTFLAGS
.equ OOTB_SLB_LED_PORT_REMAP			=	PORTC_REMAP
.equ OOTB_SLB_LED_PORT_PIN0CTRL			=	PORTC_PIN0CTRL
.equ OOTB_SLB_LED_PORT_PIN1CTRL			=	PORTC_PIN1CTRL
.equ OOTB_SLB_LED_PORT_PIN2CTRL			=	PORTC_PIN2CTRL
.equ OOTB_SLB_LED_PORT_PIN3CTRL			=	PORTC_PIN3CTRL
.equ OOTB_SLB_LED_PORT_PIN4CTRL			=	PORTC_PIN4CTRL
.equ OOTB_SLB_LED_PORT_PIN5CTRL			=	PORTC_PIN5CTRL
.equ OOTB_SLB_LED_PORT_PIN6CTRL			=	PORTC_PIN6CTRL
.equ OOTB_SLB_LED_PORT_PIN7CTRL			=	PORTC_PIN7CTRL

; MB tactile switch (TS) symbols

.equ OOTB_MB_S1_bp						=	(1)
.equ OOTB_MB_S1_bm						=	(1<<OOTB_MB_S1_bp)

.equ OOTB_MB_S2_bp						=	(0)
.equ OOTB_MB_S2_bm						=	(1<<OOTB_MB_S1_bm)

.equ OOTB_MB_TS_gm						=	(OOTB_MB_S1_bm | OOTB_MB_S2_bm)

.equ OOTB_MB_TS_PORT_DIR				=	PORTE_DIR
.equ OOTB_MB_TS_PORT_DIRSET				=	PORTE_DIRSET
.equ OOTB_MB_TS_PORT_DIRCLR				=	PORTE_DIRCLR
.equ OOTB_MB_TS_PORT_DIRTGL				=	PORTE_DIRTGL
.equ OOTB_MB_TS_PORT_OUT				=	PORTE_OUT
.equ OOTB_MB_TS_PORT_OUTSET				=	PORTE_OUTSET
.equ OOTB_MB_TS_PORT_OUTCLR				=	PORTE_OUTCLR
.equ OOTB_MB_TS_PORT_OUTTGL				=	PORTE_OUTTGL
.equ OOTB_MB_TS_PORT_IN					=	PORTE_IN
.equ OOTB_MB_TS_PORT_INTCTRL			=	PORTE_INTCTRL
.equ OOTB_MB_TS_PORT_INT0MASK			=	PORTE_INT0MASK
.equ OOTB_MB_TS_PORT_INT1MASK			=	PORTE_INT1MASK
.equ OOTB_MB_TS_PORT_INTFLAGS			=	PORTE_INTFLAGS
.equ OOTB_MB_TS_PORT_REMAP				=	PORTE_REMAP
.equ OOTB_MB_TS_PORT_PIN0CTRL			=	PORTE_PIN0CTRL
.equ OOTB_MB_TS_PORT_PIN1CTRL			=	PORTE_PIN1CTRL
.equ OOTB_MB_TS_PORT_PIN2CTRL			=	PORTE_PIN2CTRL
.equ OOTB_MB_TS_PORT_PIN3CTRL			=	PORTE_PIN3CTRL
.equ OOTB_MB_TS_PORT_PIN4CTRL			=	PORTE_PIN4CTRL
.equ OOTB_MB_TS_PORT_PIN5CTRL			=	PORTE_PIN5CTRL
.equ OOTB_MB_TS_PORT_PIN6CTRL			=	PORTE_PIN6CTRL
.equ OOTB_MB_TS_PORT_PIN7CTRL			=	PORTE_PIN7CTRL

; EBIBB tactile switch (TS) symbols (if a previous version of the kit is used)

.equ OOTB_EBIBB_S1_bp					=	(0)
.equ OOTB_EBIBB_S1_bm					=	(1<<OOTB_EBIBB_S1_bp)

.equ OOTB_EBIBB_S2_bp					=	(1)
.equ OOTB_EBIBB_S2_bm					=	(1<<OOTB_EBIBB_S1_bm)

.equ OOTB_EBIBB_TS_gm				=	(OOTB_EBIBB_S1_bm | OOTB_EBIBB_S2_bm)

.equ OOTB_EBIBB_TS_PORT_DIR				=	PORTE_DIR
.equ OOTB_EBIBB_TS_PORT_DIRSET			=	PORTE_DIRSET
.equ OOTB_EBIBB_TS_PORT_DIRCLR			=	PORTE_DIRCLR
.equ OOTB_EBIBB_TS_PORT_DIRTGL			=	PORTE_DIRTGL
.equ OOTB_EBIBB_TS_PORT_OUT				=	PORTE_OUT
.equ OOTB_EBIBB_TS_PORT_OUTSET			=	PORTE_OUTSET
.equ OOTB_EBIBB_TS_PORT_OUTCLR			=	PORTE_OUTCLR
.equ OOTB_EBIBB_TS_PORT_OUTTGL			=	PORTE_OUTTGL
.equ OOTB_EBIBB_TS_PORT_IN				=	PORTE_IN
.equ OOTB_EBIBB_TS_PORT_INTCTRL			=	PORTE_INTCTRL
.equ OOTB_EBIBB_TS_PORT_INT0MASK		=	PORTE_INT0MASK
.equ OOTB_EBIBB_TS_PORT_INT1MASK		=	PORTE_INT1MASK
.equ OOTB_EBIBB_TS_PORT_INTFLAGS		=	PORTE_INTFLAGS
.equ OOTB_EBIBB_TS_PORT_REMAP			=	PORTE_REMAP
.equ OOTB_EBIBB_TS_PORT_PIN0CTRL		=	PORTE_PIN0CTRL
.equ OOTB_EBIBB_TS_PORT_PIN1CTRL		=	PORTE_PIN1CTRL
.equ OOTB_EBIBB_TS_PORT_PIN2CTRL		=	PORTE_PIN2CTRL
.equ OOTB_EBIBB_TS_PORT_PIN3CTRL		=	PORTE_PIN3CTRL
.equ OOTB_EBIBB_TS_PORT_PIN4CTRL		=	PORTE_PIN4CTRL
.equ OOTB_EBIBB_TS_PORT_PIN5CTRL		=	PORTE_PIN5CTRL
.equ OOTB_EBIBB_TS_PORT_PIN6CTRL		=	PORTE_PIN6CTRL
.equ OOTB_EBIBB_TS_PORT_PIN7CTRL		=	PORTE_PIN7CTRL

;**************************END OF DEFINED SYMBOLS******************************

#endif		; end of header guard

;**********************END OF "ootb_upad_platform.inc"*************************