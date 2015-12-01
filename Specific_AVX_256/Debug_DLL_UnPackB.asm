;---------------------------------------------------------------------------;
;									                                                          ;
;	                          Complex DLL debug sample.                       ;
;									                                                          ;
;---------------------------------------------------------------------------;

;--- For Pack B ---
MemorySize = 0C0000000h                             ; 3 GBytes memory allocated
MEASURE = 1024 * 1024 * 1024 / Hb / Wb / 128 ; * 10
WorkCycles = Hb * Wb * Ha * Nb * Na * Nf * MEASURE  ; For measurement
Hb = 32
Wb = 4
Ha = 1
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
xor rsi,rsi
mov rdi,040000000h
;---

mov r15d,MEASURE 
MeasureCycle:
;--- Pack B ---
; R8D = 4096*8 = Slow
; R8D = 4032*8 = Fast
; R8D = 4104*8 = Very Fast
; Cache associativity limit or extra-prefetches f(page bounds), TLB miss ?
; Gathered Read cause this.
; Need optimal sequence for B-blocks.
; INT3
;---
lea rax,[Data_00]
vmovapd ymm0,[rax+32*00]
vmovapd ymm1,[rax+32*01]
vmovapd ymm2,[rax+32*02]
vmovapd ymm3,[rax+32*03]
vmovapd [rbx+4096*8*0],ymm0
vmovapd [rbx+4096*8*1],ymm1
vmovapd [rbx+4096*8*2],ymm2
vmovapd [rbx+4096*8*3],ymm3
vzeroall
;---

;INT3

lea rcx,[rbx+rsi]         ; RCX = Parm#1 = Base  = Source Buffer
;lea rcx,[Data_00]

lea rdx,[rbx+rdi]         ; RDX = Parm#2 = Base+1GB = Destination Buffer
mov r8d,4032 * 8          ; R8  = Parm#3 = Size of Matrix B line, bytes (Xb)
mov r9d,Hb                ; R9  = Parm#4 = Atoms per Block B line (Hb)
push 0                    ; Stack frame alignment
push Wb                   ; [RSP+40] = Parm#5 = Lines per Block B (Wb)
sub rsp,32                ; Make stack frame
call [UnPackB]
add rsp,32+16
;add rsi,4*32
;add rdi,16384
;----------

;dec r15
;jnz MeasureCycle


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
align 32
Data_00   DQ  00.00000001 , 01.00000001 , 02.00000001 , 03.00000001 
Data_10   DQ  10.00000001 , 11.00000001 , 12.00000001 , 13.00000001
Data_20   DQ  20.00000001 , 21.00000001 , 22.00000001 , 23.00000001
Data_30   DQ  30.00000001 , 31.00000001 , 32.00000001 , 33.00000001

;---------- Import data section --------------------------------------------;
IMPORT_SECTION