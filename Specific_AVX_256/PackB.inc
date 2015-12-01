;--- Pack Matrix B block function ---------------------------------------------;
; Parm#1 = RCX  = Pointer to source block (matrix B)                           ;
; Parm#2 = RDX  = Pointer to destination block (packed block B)                ;
; Parm#3 = R8   = Matrix B line size, bytes (Xb)                               ;
; Parm#4 = R9   = Block B atoms per line (Hb)                                  ;
; Parm#5 = [RSP+40] = Block B lines (Wb)                                       ;  
; Output = RAX = Not used for this function                                    ;
;------------------------------------------------------------------------------;

; Line Size = R8D = 4096*8 = Slow
; Line Size = R8D = 4032*8 = Fast
; Line Size = R8D = 4104*8 = Very Fast
; Cache associativity limit or extra-prefetches = f(page bounds), TLB miss ?
; Gathered Read cause this.
; Need optimal sequence for B-blocks.

;---------- Save registers by Microsoft x64 calling convention ----------------;

mov eax,[rsp+40]            ; EAX = Lines per block , vertical count
push rbx rsi rdi rbp
mov rbp,rsp                 ; RBP = Pointer to save XMM area
sub rsp,6*16+16             ; 6 XMM registers +16 for alignment
and rbp,0FFFFFFFFFFFFFFF0h  ; Address must be aligned by 16
sub rbp,6*16
vmovapd [rbp+16*00],xmm6    ; Save XMM context
vmovapd [rbp+16*01],xmm7
vmovapd [rbp+16*02],xmm8
vmovapd [rbp+16*03],xmm9
vmovapd [rbp+16*04],xmm10
vmovapd [rbp+16*05],xmm11

;---------- Prepare parameters for main cycles --------------------------------;

lea r10,[r8*2]              ; R8 = Line , R10 = 2*Line
lea r11,[r8+r10]            ; R11 = 3*Line
lea rdi,[r10*2]             ; RDI = 4*Line, for Y+4

;---------- Main cycles -------------------------------------------------------;

PackBlockY:                 ; Start cycle for lines per block, by Y

mov rbx,rcx                 ; RBX = Source base, reloaded at each line 
mov esi,r9d                 ; ESI = Reload atoms counter

PackBlockX:                 ; Start cycle for atoms per line, by X

;--- Variant 1: 128-bit writes without software write-combining ---
; vmovapd ymm0,[rbx]          ; YMM0  = 00, 01, 02, 03
; vmovapd ymm1,[rbx+r8]       ; YMM1  = 10, 11, 12, 13 
; vmovapd ymm2,[rbx+r10]      ; YMM2  = 20, 21, 22, 23
; vmovapd ymm3,[rbx+r11]      ; YMM3  = 30, 31, 32, 33
; vunpcklpd xmm4,xmm0,xmm1    ; YMM4  = 00, 10, - , -
; vunpcklpd xmm5,xmm2,xmm3    ; YMM5  = 20, 30, - , -
; vmovapd [rdx],xmm4          ; Store   00, 10 (128-bit write)
; vmovapd [rdx+16],xmm5       ; Store   20, 30 (128-bit write)
; vunpckhpd xmm6,xmm0,xmm1    ; YMM6  = 01, 11, - , -
; vunpckhpd xmm7,xmm2,xmm3    ; YMM7  = 21, 31, - , -       
; vmovapd [rdx+32],xmm6       ; Store   01, 11 (128-bit write)
; vmovapd [rdx+32+16],xmm7    ; Store   21, 31 (128-bit write)
; vextractf128 xmm0,ymm0,1    ; YMM0  = 02, 03, - , - 
; vextractf128 xmm1,ymm1,1    ; YMM1  = 12, 13, - , -
; vextractf128 xmm2,ymm2,1    ; YMM2  = 22, 23, - , -
; vextractf128 xmm3,ymm3,1    ; YMM3  = 32, 33, - , -
; vunpcklpd xmm8,xmm0,xmm1    ; YMM8  = 02, 12, - , -
; vunpcklpd xmm9,xmm2,xmm3    ; YMM9  = 22, 32, - , -
; vmovapd [rdx+64],xmm8       ; Store   02, 12 (128-bit write)
; vmovapd [rdx+64+16],xmm9    ; Store   22, 32 (128-bit write)
; vunpckhpd xmm10,xmm0,xmm1   ; YMM10 = 03, 13, - , - 
; vunpckhpd xmm11,xmm2,xmm3   ; YMM11 = 23, 33, - , -
; vmovapd [rdx+96],xmm10      ; Store   03, 13 (128-bit write)
; vmovapd [rdx+96+16],xmm11   ; Store   23, 33 (128-bit write)

;--- Variant 2: 256-bit writes with software write-combining ---
; vmovapd ymm0,[rbx]              ; YMM0  = 00, 01, 02, 03
; vmovapd ymm1,[rbx+r8]           ; YMM1  = 10, 11, 12, 13 
; vmovapd ymm2,[rbx+r10]          ; YMM2  = 20, 21, 22, 23
; vmovapd ymm3,[rbx+r11]          ; YMM3  = 30, 31, 32, 33
; vunpcklpd xmm4,xmm0,xmm1        ; YMM4  = 00, 10, - , -
; vunpcklpd xmm5,xmm2,xmm3        ; YMM5  = 20, 30, - , -
; vinsertf128 ymm4,ymm4,xmm5,1    ; YMM4  = 00, 10, 20, 30  
; vmovapd [rdx+32*0],ymm4         ; Store   00, 10, 20, 30 (256-bit write)
; vunpckhpd xmm6,xmm0,xmm1        ; YMM6  = 01, 11, - , -
; vunpckhpd xmm7,xmm2,xmm3        ; YMM7  = 21, 31, - , -       
; vinsertf128 ymm6,ymm6,xmm7,1    ; YMM6  = 01, 11, 21, 31
; vmovapd [rdx+32*1],ymm6         ; Store   01, 11, 21, 31 (256-bit write)
; vextractf128 xmm0,ymm0,1        ; YMM0  = 02, 03, - , - 
; vextractf128 xmm1,ymm1,1        ; YMM1  = 12, 13, - , -
; vextractf128 xmm2,ymm2,1        ; YMM2  = 22, 23, - , -
; vextractf128 xmm3,ymm3,1        ; YMM3  = 32, 33, - , -
; vunpcklpd xmm8,xmm0,xmm1        ; YMM8  = 02, 12, - , -
; vunpcklpd xmm9,xmm2,xmm3        ; YMM9  = 22, 32, - , -
; vinsertf128 ymm8,ymm8,xmm9,1    ; YMM8  = 02, 12, 22, 32 
; vmovapd [rdx+32*2],ymm8         ; Store   02, 12, 22, 32 (256-bit write)
; vunpckhpd xmm10,xmm0,xmm1       ; YMM10 = 03, 13, - , - 
; vunpckhpd xmm11,xmm2,xmm3       ; YMM11 = 23, 33, - , -
; vinsertf128 ymm10,ymm10,xmm11,1 ; YMM10  = 03, 13, 23, 33
; vmovapd [rdx+32*3],ymm10        ; Store   03, 13, 23, 33 (256-bit write)

;--- Variant 3: 256-bit writes with software write-combining+ ---
vmovapd ymm0,[rbx]              ; YMM0  = 00, 01, 02, 03
vmovapd ymm1,[rbx+r8]           ; YMM1  = 10, 11, 12, 13 
vmovapd ymm2,[rbx+r10]          ; YMM2  = 20, 21, 22, 23
vmovapd ymm3,[rbx+r11]          ; YMM3  = 30, 31, 32, 33
vunpcklpd xmm4,xmm0,xmm1        ; YMM4  = 00, 10, - , -
vunpcklpd xmm5,xmm2,xmm3        ; YMM5  = 20, 30, - , -
vinsertf128 ymm4,ymm4,xmm5,1    ; YMM4  = 00, 10, 20, 30  
vunpckhpd xmm6,xmm0,xmm1        ; YMM6  = 01, 11, - , -
vunpckhpd xmm7,xmm2,xmm3        ; YMM7  = 21, 31, - , -       
vinsertf128 ymm6,ymm6,xmm7,1    ; YMM6  = 01, 11, 21, 31
vextractf128 xmm0,ymm0,1        ; YMM0  = 02, 03, - , - 
vextractf128 xmm1,ymm1,1        ; YMM1  = 12, 13, - , -
vextractf128 xmm2,ymm2,1        ; YMM2  = 22, 23, - , -
vextractf128 xmm3,ymm3,1        ; YMM3  = 32, 33, - , -
vunpcklpd xmm8,xmm0,xmm1        ; YMM8  = 02, 12, - , -
vunpcklpd xmm9,xmm2,xmm3        ; YMM9  = 22, 32, - , -
vinsertf128 ymm8,ymm8,xmm9,1    ; YMM8  = 02, 12, 22, 32 
vunpckhpd xmm10,xmm0,xmm1       ; YMM10 = 03, 13, - , - 
vunpckhpd xmm11,xmm2,xmm3       ; YMM11 = 23, 33, - , -
vinsertf128 ymm10,ymm10,xmm11,1 ; YMM10  = 03, 13, 23, 33
vmovapd [rdx+32*0],ymm4         ; Store   00, 10, 20, 30 (256-bit write)
vmovapd [rdx+32*1],ymm6         ; Store   01, 11, 21, 31 (256-bit write)
vmovapd [rdx+32*2],ymm8         ; Store   02, 12, 22, 32 (256-bit write)
vmovapd [rdx+32*3],ymm10        ; Store   03, 13, 23, 33 (256-bit write)

;---

add rbx,rdi                 ; Modify source address: Y+4 , 1 atom down
add rdx,128                 ; Modify destination address: 128 bytes per B-atom
dec esi                     ; Atoms counter - 1
jnz PackBlockX              ; End cycle for atoms, by X

add rcx,32                  ; Modify source address: X+1 , 1 atom right
dec eax                     ; B-Lines counter - 1
jnz PackBlockY              ; End cycle for lines, by Y

;--------- Restore registers by Microsoft x64 calling convention, return ------;

vzeroupper
vmovapd xmm6, [rbp+16*00]   ; Restore XMM context
vmovapd xmm7, [rbp+16*01]
vmovapd xmm8, [rbp+16*02]
vmovapd xmm9, [rbp+16*03]
vmovapd xmm10,[rbp+16*04]
vmovapd xmm11,[rbp+16*05]
add rsp,6*16+16             ; Remove stack frame for XMM
pop rbp rdi rsi rbx
ret

