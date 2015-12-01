;---------------------------------------------------------------------------;
;									                                                          ;
;	                          Complex DLL debug sample.                       ;
;									                                                          ;
;---------------------------------------------------------------------------;

;--- For Set and Copy ---
MEASURE    = 1
MemorySize = 0C0000000h        ; 3 GBytes memory allocated
Length     = 512 
WorkCycles = 1 

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

; INT3


; mov eax,[L0]
; mov rbx,[L1]
; jmp @f
; L0:
; DD -133.71
; L1:
; DQ 543.0651
; @@:


call [InitLibrary]
;call [DetectAVX]


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

;---------- Import data section --------------------------------------------;
IMPORT_SECTION