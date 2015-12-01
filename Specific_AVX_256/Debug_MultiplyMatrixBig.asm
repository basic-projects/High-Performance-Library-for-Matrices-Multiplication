; OPTIMIZING REQUIRED - CORRECT A-PREFETCH.

;---------------------------------------------------------------------------;
;									                                                          ;
;	                         Functions debug sample.                          ;
;									                                                          ;
;---------------------------------------------------------------------------;

MemorySize  = 0C0000000h  ; 3 GBytes memory allocated (loss detected if 1GB)
MEASURE     = 1000        ; Number of measurement iterations
Hb          = 44+4 ; 45+3 ; Atoms per A,B-line
Wb          = 2 ; 3       ; Lines per B-block , for _Wb3_3N3 , 15
Ha          = 48          ; Lines per A-macrocolumn
Nb          = 336         ; B-blocks per series
WorkCycles  = MEASURE * Hb * Wb * Ha * Nb 

; Normally for B-prefetch Hb = Ha = 45+3 = 48
; For BC_V1 this equation means B-prefetch each base cycle run
; For debug over-prefetch performance loss and prefetch limit logic,
; make Ha > Hb , make Ha > 48

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
; ALLOCATE_MEMORY_LARGE
;--- Pre-set buffer ---
PRE_SET_BUFFER
;--- Get start time (OS time and CPU Time Stamp Counter) ---
DEBUG_START

;---------- Debug function ----------------------------------------------------;

;test rbx,0FFFh
;jz @f
;add rbx,1000h
;and rbx,0FFFFFFFFFFFFF000h
;@@:

;-------------------------------;
; mov r15d,MEASURE
; MeasureCycle:
; ;---
; mov rcx,rbx                ; A = Base + 0
; lea rdx,[rbx+10000000h]    ; B = Base + 256MB
; lea r8, [rbx+20000000h]    ; C = Base + 512MB
; lea r9, [rbx+30000000h]    ; Prefetch = Base + 768MB 
; push Nb Ha
; sub rsp,32
; ; INT3
; call [MultiplyMatrixBig_Wb3_3]
; add rsp,32 + 16
; ;---
; dec r15
; jnz MeasureCycle
;-------------------------------;
mov r15d,MEASURE
MeasureCycle:
;---
mov rcx,rbx                  ; A = Base + 0
lea rdx,[rbx+10000000h]      ; B = Base + 256MB
lea r8, [rbx+20000000h]      ; C = Base + 512MB
lea r9, [rbx+30000000h]      ; Prefetch = Base + 768MB
push 0 Nb Ha 11
sub rsp,32
; INT3
call [MultiplyMatrixBig_Wb2_4N4]
add rsp,32 + 32
;---
dec r15
jnz MeasureCycle
;-------------------------------;

;---------- Start visual data ----------------------------------------------;
;--- Visualize registers, get stop time, wait user press key, termination ---
DEBUG_STOP
;---------- Service subroutines --------------------------------------------;
DEBUG_SERVICE
;---------- Data section ---------------------------------------------------;
section '.data' data readable writeable
;--- Visualizer, initializer, timings measure support data ---
DEBUG_DATA
;---------- Import data section --------------------------------------------;
IMPORT_SECTION
