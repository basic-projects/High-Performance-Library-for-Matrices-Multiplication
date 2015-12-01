;---------------------------------------------------------------------------;
;									                                                          ;
;	                          Complex DLL debug sample.                       ;
;									                                                          ;
;---------------------------------------------------------------------------;
 
;--- For UnPack A ---
MemorySize = 0C0000000h                             ; 3 GBytes memory allocated
MEASURE = 1024 * 1024 * 1024 / Hb / Ha / 64 * 100
WorkCycles = Hb * Wb * Ha * Nb * Na * Nf * MEASURE  ; For measurement
Hb = 32
Wb = 1
Ha = 64
Nb = 1
Na = 1
Nf = 1

;---------- Header ---------------------------------------------------------;
format PE64 GUI 5.0
entry start
include 'win64a.inc'
include 'Debug_Context.inc'
;---------- Code section ---------------------------------------------------;
section '.text' code readable executable
start:
;--- Prepare stack ---
sub	rsp,8*5 			   ; Reserve stack for API use and make stack dqword aligned
;--- Allocate memory ---
ALLOCATE_MEMORY
;--- Pre-set buffer ---
PRE_SET_BUFFER
;--- Get start time (OS time and CPU Time Stamp Counter) ---
DEBUG_START


;---------- Start target debug fragment ---------------------------------------;
; RBX = Base adress, aligned
; RSI = 0
; RDI = 40000000h
;xor rsi,rsi
;mov rdi,040000000h
;---
;mov r15d,MEASURE 
;MeasureCycle:
;--- PackA ---
; INT3
;---
;lea rcx,[rbx+rsi]         ; RCX = Parm#1 = Base  = Source Buffer
;lea rdx,[rbx+rdi]         ; RDX = Parm#2 = Base+1GB = Destination Buffer
;mov r8d,4032 * 8          ; R8  = Parm#3 = Size of Matrix A line, bytes (Xa)
;mov r9d,Hb                ; R9  = Parm#4 = Atoms per Block B line (Hb)
;push 0                    ; [RSP+48] = Parm#6 = Transpose Flag = FALSE 
;push Ha                   ; [RSP+40] = Parm#5 = Lines per Macro Column A (Ha)
;sub rsp,32                ; Make stack frame
;call [PackA]
;add rsp,32+16
;----------
;dec r15
;jnz MeasureCycle

;--- Debug second variant with A transposed ---
; INT3
;---

; REMOVED
; push 1 ; 0                 ; [RSP+48] = Parm#6 = Transpose Flag 

push 3 ; 3 ; 2             ; [RSP+40] = Parm#5 = Lines per Macro Column A (Ha)
mov r9d,2 ; 1 ; 3          ; R9D      = Parm#4 = Atoms per Line (Hb)
mov r8d,128                ; R8D      = Parm#3 = Size of Matrix A line (Xa)
lea rdx,[DstMatrix]        ; RDX      = Parm#2 = Pointer to destination
lea rcx,[SrcMatrix]        ; RCX      = Parm#1 = Pointer to source
sub rsp,32                 ; Make stack frame
call [UnPackA]
add rsp,32+8 ; +16


;---------- Start visual data ----------------------------------------------;

;--- Visualize registers, get stop time, wait user press key, termination ---
DEBUG_STOP
;---------- Service subroutines --------------------------------------------;
DEBUG_SERVICE
;---------- Data section ---------------------------------------------------;
section '.data' data readable writeable
;--- Visualizer, initializer, timings measure support data ---
DEBUG_DATA

; Used for matrixes re-pack debug
;align 32
;Data_00   DQ  00.00000001 , 01.00000001 , 02.00000001 , 03.00000001 
;Data_10   DQ  10.00000001 , 11.00000001 , 12.00000001 , 13.00000001
;Data_20   DQ  20.00000001 , 21.00000001 , 22.00000001 , 23.00000001
;Data_30   DQ  30.00000001 , 31.00000001 , 32.00000001 , 33.00000001

; Used for A_t mode debug
align 32
SrcMatrix:

DQ  00.00 , 10.00 , 20.00 , 30.00
DQ  01.00 , 11.00 , 21.00 , 31.00 
DQ  8 DUP (1.0)
DQ  01.01 , 02.01 , 1.0 , 1.0 , 12 DUP (0.0)
DQ  10.01 , 11.01 , 1.0 , 1.0 , 12 DUP (0.0) 
DQ  20.01 , 21.01 , 1.0 , 1.0 , 12 DUP (0.0) 
DQ  30.01 , 31.01 , 1.0 , 1.0 , 12 DUP (0.0) 
DQ  40.01 , 41.01 , 1.0 , 1.0 , 12 DUP (0.0) 
DQ  50.01 , 51.01 , 1.0 , 1.0 , 12 DUP (0.0) 
DQ  60.01 , 61.01 , 1.0 , 1.0 , 12 DUP (0.0) 
DQ  70.01 , 71.01 , 1.0 , 1.0 , 12 DUP (0.0) 
DQ  1024 DUP (-1)

DstMatrix:
DQ  1024 DUP (-1)




;---------- Import data section --------------------------------------------;
IMPORT_SECTION