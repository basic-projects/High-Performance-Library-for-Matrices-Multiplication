; BUG FIX REQUIRED - CLEAR POINTER A FOR EXTERNAL ITERATION. QUESTION.
; OPTIMIZING REQUIRED - MOVE B-PREFETCH TO INNER CYCLE.QUESTION.
; OPTIMIZING REQUIRED - CORRECT A-PREFETCH.


;---------------------------------------------------------------------------;
;									                                                          ;
;	                          Functions debug sample.                         ;
;									                                                          ;
;---------------------------------------------------------------------------;

MemorySize  = 0C0000000h  ; 3 GBytes memory allocated (loss detected if 1GB)
MEASURE     = 1000        ; Number of measurement iterations
Wb          = 15+1        ; Atoms per A,B-line
Hb          = 3           ; Lines per B-block
Ha          = 48  ; * 10  ; Lines per A-macrocolumn
Nb          = 100         ; B-blocks per series
WorkCycles  = MEASURE * Wb * Hb * Ha * Nb 

; Normally for B-prefetch Hb * Wb = Ha = (15+1)*3 = 16 * 3 = 48
; For BC_V2 this equation means B-prefetch each B-block run
; For debug over-prefetch performance loss and prefetch limit logic,
; make Ha > Hb * Wb , make Ha > 48

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

;-------------------------------------------;
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
; call [MultiplyMatrixSmall_Wb3_1]
; add rsp,32 + 16
; ;---
; dec r15
; jnz MeasureCycle
;-------------------------------------------;
 mov r15d,MEASURE
 MeasureCycle:
 ;---
 mov rcx,rbx                 ; A = Base + 0
 lea rdx,[rbx+10000000h]     ; B = Base + 256MB
 lea r8, [rbx+20000000h]     ; C = Base + 512MB
 lea r9, [rbx+30000000h]     ; Prefetch = Base + 768MB
 push 0 Nb Ha 5
 sub rsp,32
 ; INT3
 call [MultiplyMatrixSmall_Hb3_3N1]
 add rsp,32 + 32
 ;---
 dec r15
 jnz MeasureCycle
;-------------------------------------------;

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
