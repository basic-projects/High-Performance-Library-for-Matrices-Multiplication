;--- Detect AVX256/512 features support by processor and OS -------------------;
;                                                                              ;
; INPUT:  Parm#1 = RCX = Pointer to output buffer                              ;
;                                                                              ;
; OUTPUT: RAX = Reserved detection status, 0=OK, otherwise error               ;
;         Output buffer for DLL selection, 320 bytes = 16 * 20                 ;
;         Bytes per descriptor = 16 , Number of descriptors = 20               ;
;         Each descriptor format:                                              ;
;         BYTE SupportCPU : '-' not support , '+' support                      ;
;         BYTE SupportOS :  '-' not support , '+' support                      ;
;         10 BYTES = Library suffix, ASCII string, all chars must be valid     ;
;         DWORD = AVX vector register width, bits                              ;
;                                                                              ;
;------------------------------------------------------------------------------;

push rbx rsi rdi r14 r15
mov rdi,rcx
;--- Pre-clear R14, R15 used as features bitmaps ---
xor r14d,r14d
xor r15d,r15d
;--- Detect minimum ---
; Don't check EFLAGS.21, if control at this point, means CPUID supported
xor eax,eax         ; Function = 00000000h, check standard CPUID functions
cpuid
cmp eax,1
jb Lerror           ; Go error if CPUID function 80000001h not supported 
;--- Check support AVX (Sandy Bridge) ---
; AVX feature = CPUID#1 bit ECX.28
;---
mov eax,1
cpuid           ; Get ECX:EDX = CPU Standard Features List
mov r14d,ecx    ; R14[31-0] = Storage for CPUID#1 ECX
;--- Check support OS context management features ---
; XMM128     context = XCR0.1
; YMM256     context = XCR0.2
; ZMM[0-15]  context = XCR0.6
; ZMM[16-31] context = XCR0.7
; K[0-7]     context = XCR0.5
;---
mov eax,008000000h
and ecx,eax
cmp ecx,eax
jne L1          ; Go skip if OSXSAVE(ECX.27) not supported
xor ecx,ecx
xgetbv
shl rax,32
or r14,rax      ; R14[63-32] = Storage for XCR0[31-00]
L1:
;--- Check support AVX2 (Haswell) ---
; AVX2 feature = CPUID#7 Subfunction#0 EBX.5 
;--- Check support AVX3 / AVX512F=Foundation (Skylake Xeon) ---
; AVX512F    feature = CPUID#7 Subfunction#0 EBX.16
; AVX512CD   feature = CPUID#7 Subfunction#0 EBX.28
; AVX512PF   feature = CPUID#7 Subfunction#0 EBX.26
; AVX512ER   feature = CPUID#7 Subfunction#0 EBX.27
; AVX512VL   feature = CPUID#7 Subfunction#0 EBX.31
; AVX512BW   feature = CPUID#7 Subfunction#0 EBX.30
; AVX512DQ   feature = CPUID#7 Subfunction#0 EBX.17
; AVX512IFMA feature = CPUID#7 Subfunction#0 EBX.21
; AVX512VBM  feature = CPUID#7 Subfunction#0 ECX.1
;---
xor eax,eax
cpuid
cmp eax,7
jb L2
mov eax,7
xor ecx,ecx
cpuid
mov r15d,ebx    ; R15[31-00] = Storage for CPUID#7 EBX
shl rcx,32
or r15,rcx      ; R15[63-32] = Storage for CPUID#7 ECX
L2:
;--- End of detect ---
Lerror:
;--- Copy library list to caller buffer ---
mov ecx,320/8
lea rsi,[LibraryList]
mov rbx,rdi
cld
rep movsq
;--- Patch library list = F(Processor Features) ---
bt r14d,28
jnc @f
mov byte [rbx+00],'+'
@@:
;--- Patch library list = F(OS Features) ---
mov rax,0000000600000000h
and r14,rax
cmp r14,rax
jne @f
mov byte [rbx+01],'+'
@@:
;--- Exit ---
pop r15 r14 rdi rsi rbx
xor eax,eax
ret


