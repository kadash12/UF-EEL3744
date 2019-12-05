;******************************************************************************
;  File name: misc.inc
;  Author: Christopher Crary
;  Last Modified By: Christopher Crary
;  Last Modified On: 06 September 2019
;  Purpose: To define general miscellaneous, useful symbols, as pertains
;			to EEL3744C.
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

#ifndef TSFF_MISC_INC		; beginning of header guard
#define TSFF_MISC_INC		; "TSFF" = "Thirty-Seven, Forty-Four"

;*********************************INCLUDES*************************************
;******************************END OF INCLUDES*********************************

;******************************DEFINED SYMBOLS*********************************

.equ ALL_BITS_bm					=	(0xFF)

;**************************END OF DEFINED SYMBOLS******************************

#endif		; end of header guard

;*****************************END OF "misc.inc"********************************
