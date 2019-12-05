.nolist  
.include "ATxmega128A1Udef.inc"  
.list     
.org 0x00  
 
.set IN_PORT = 0x019000  ;configuring the EBI to start at 0x370000 in memory  
.set IN_PORT_END = 0x01903F  ;and end at 0x377FFF  
.set IN_PORT1 = 0x370000  ;configuring the EBI to start at 0x370000 in memory  
.set IN_PORT_END1 = 0x377FFF  ;and end at 0x377FFF      

.org 0xF3D5  ;Define Table size in program memory  

;My table 
TABLE: 
.db 0x4d, 0x61, 0x6b, 0x69, 0x6e, 0x67, 0x20, 0x79, 0x6f, 0x75 
.db 0x72, 0x20, 0x77, 0x61, 0x79, 0x20, 0x69, 0x6e, 0x20, 0x74 
.db 0x75, 0x72, 0x20, 0x6e, 0x61, 0x6d, 0x65, 0x2e, 0x20, 0x20 
.db 0x00, 0x00

 .CSEG 
 .org 0x100 
 rjmp MAIN 
 
MAIN: 
 
 ldi r16, 0b00110111 ;Loads PORTH with RE, WE,CS1,CSO, and ALE1  
 sts PORTH_DIRSET, r16 ;Store certain pin directions into PORTH  
 ldi r16, 0b00110011 ;Loads PORTH with RE, WE,CS1,CSO, and ALE1  
 sts PORTH_OUTSET, r16 ;Store certain bits as Output 

 ;This sets portJ and PortK to be outputs   
 ldi r16, 0xFF ;Load r16 with 0xFF  
 sts PORTK_DIRSET, r16 ;Store into PortK  
 sts PORTJ_DIRSET, r16 ;Store into PortJ 
 
;For EBI   
ldi r16, 0b00000001  
sts EBI_CTRL, r16 

;Sets the Control Register for Chip Select CS0 (SRAM - 32K)   
ldi r16, 0b00011101    
sts EBI_CS0_CTRLA, r16 
 
;Sets the Control Register for Chip Select CS1 (256B I/0)  
ldi r16, 0b0000001  
sts EBI_CS1_CTRLA, r16 
 
;Points to the table in Program Memory   
ldi ZL, BYTE3(TABLE << 1)    
out CPU_RAMPZ, ZL    
ldi ZL, low(TABLE << 1)    
ldi ZH, high(TABLE << 1)  
 
;Points to a table in Data Memory   
ldi r16, byte3(IN_PORT)  
sts CPU_RAMPY, r16   
ldi YL, low(IN_PORT)    
ldi YH, high(IN_PORT)  
 
;Point to the IN_PORT with X (SRAM)  
ldi r16, byte3(IN_PORT1)  
sts CPU_RAMPX, r16  
ldi XH, high(IN_PORT1)  
ldi XL, low(IN_PORT1) 

;Initialize middle byte (A15:8) of BASEADDR for CS1, EBI_CS1_BASEADDR  
ldi r16, byte2(IN_PORT)  
sts EBI_CS1_BASEADDR, r16

;Initialize high byte (A23:A16) of BASEADDR for CS1, EBI_CS1_BASEADDR+1  
ldi r16, byte3(IN_PORT)  
sts EBI_CS1_BASEADDR+1, r16 
 
;Initialize middle byte (A15:8) of BASEADDR for CS0, EBI_CS0_BASEADDR  
ldi r16, byte2(IN_PORT1)  
sts EBI_CS0_BASEADDR, r16 
 
;Initialize high byte (A23:A16) of BASEADDR for CS0, EBI_CS0_BASEADDR+1  
ldi r16, byte3(IN_PORT1)  
sts EBI_CS0_BASEADDR+1, r16 
 
LOOP: ;Writes to SRAM  
elpm r16, Z+  
st X+, r16    
cpi r16, 0  
brne LOOP 
 
 ;Point to the IN_PORT with X (SRAM)  
 ldi r16, byte3(IN_PORT1)  
 sts CPU_RAMPX, r16  
 ldi XH, high(IN_PORT1)  
 ldi XL, low(IN_PORT1) 

 RESET: ;Reads from SRAM and displays Output  
 ld r16, X+  
 st Y, r16 
 
;TC 20HZ  
;Initialize Counter  
ldi r22, 0b0111 ;Prescaler stored into r22  
sts TCC0_CTRLA, r22 ;Sets the Clock Select from Register A 
;Load high and low byte of Period  
ldi r22, 0x62 ;Load r16 with 0x62  
sts TCC0_PER, r22 ;Store low byte first  
ldi r22, 00;Load r16 with 01  
sts TCC0_PER + 1, r22 ;Store high byte 

;Check Overflow 
FLAG1:  
lds r20, TCC0_INTFLAGS ;Loads the Flag into r20  
sbrs r20, 0 ;Compares r20 with 0  
rjmp FLAG1 ;rjmp to FLAG1 until SET  
sts TCC0_INTFLAGS, r20 ;Store this value into the INTFLAGS Register    
cpi r16, 0 ;Compare r16 with 0  
brne READ
;Branches to repeat if not equal to zero  
rjmp LOOP ;Branches to loop if it is 0 
 
 