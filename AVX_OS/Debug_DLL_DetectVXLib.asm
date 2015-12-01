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

mov rdi,rbx
mov ecx,320+32
mov al,11h
rep stosb

mov rcx,rbx

; INT3

call [DetectVXLib]

; INT3
; nop


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