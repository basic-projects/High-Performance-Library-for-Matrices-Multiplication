;--- Pack Matrix B block function ---------------------------------------------;
; Parm#1 = RCX  = Pointer to source block (matrix B)                           ;
; Parm#2 = RDX  = Pointer to destination block (packed block B)                ;
; Parm#3 = R8   = Matrix B line size, bytes (Xb)                               ;
; Parm#4 = R9   = Block B atoms per line (Hb)                                  ;
; Parm#5 = [RSP+40] = Block B lines (Wb)                                       ;  
; Output = RAX = Not used for this function                                    ;
;------------------------------------------------------------------------------;

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

UnPackBlockY:               ; Start cycle for lines per block, by Y

mov rbx,rdx                 ; RBX = Destination base, reloaded at each line 
mov esi,r9d                 ; ESI = Reload atoms counter

UnPackBlockX:               ; Start cycle for atoms per line, by X

;--- Variant 3: 256-bit writes with software write-combining+ ---

vmovapd ymm0,[rcx+32*0]         ; Load YMM0  00, 10, 20, 30 (256-bit write)
vmovapd ymm1,[rcx+32*1]         ; Load YMM1  01, 11, 21, 31 (256-bit write)
vmovapd ymm2,[rcx+32*2]         ; Load YMM2  02, 12, 22, 32 (256-bit write)
vmovapd ymm3,[rcx+32*3]         ; Load YMM3  03, 13, 23, 33 (256-bit write)

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

vmovapd [rbx],ymm4              ; Store row 0 to column: YMM4  = 00, 01, 02, 03
vmovapd [rbx+r8],ymm6           ; Store row 1 to column: YMM6  = 10, 11, 12, 13 
vmovapd [rbx+r10],ymm8          ; Store row 2 to column: YMM8  = 20, 21, 22, 23
vmovapd [rbx+r11],ymm10         ; Store row 3 to column: YMM10 = 30, 31, 32, 33

;---

add rbx,rdi                 ; Modify destination address: Y+4 , 1 atom down
add rcx,128                 ; Modify source address: 128 bytes per B-atom
dec esi                     ; Atoms counter - 1
jnz UnPackBlockX            ; End cycle for atoms, by X

add rdx,32                  ; Modify destination address: X+1 , 1 atom right
dec eax                     ; B-Lines counter - 1
jnz UnPackBlockY            ; End cycle for lines, by Y

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


