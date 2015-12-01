;--- Pack Matrix A macro column function (with A optional transposed) ---------;
; This variant for v2-based method, for small matrixes                         ;
;                                                                              ;
; Parm#1 = RCX  = Pointer to source block (matrix A)                           ;
; Parm#2 = RDX  = Pointer to destination block (packed macro column A)         ;
; Parm#3 = R8D  = Matrix A line size, bytes (Xa)                               ;
; Parm#4 = R9D  = Macro column A atoms per line (Hb)                           ;
; Parm#5 = [RSP+40] = (DWORD) Macro column A lines (Ha)                        ;
; REJECTED PARAMETER #6, NOW #7 IS #6.                                         ;
; REJECTED: Parm#6 = [RSP+48] = (DWORD) Number of A-blocks per series (Nb)     ;
; RSP+48 INSTEAD RSP+56.                                                       ;
; Parm#7 = [RSP+56] = (DWORD) Transpose flag: FALSE=None, TRUE=Transpose       ;  
; Output = RAX = Not used for this function                                    ;
;------------------------------------------------------------------------------;

;---------- Select method = F (A transpose) -----------------------------------;
cmp dword [rsp+48],0        ; Check transpose flag
jne SModeTranspA            ; Go if A transposed

;---------- Save registers by Microsoft x64 calling convention ----------------;
push rbx                    ; RBX, RSI is non-volatile by convention
;---------- Prepare parameters for main cycles --------------------------------;
lea r10,[r8*2]              ; R8 = LineFull , R10 = 2*LineFull (bytes)
;---------- Main cycles -------------------------------------------------------;
mov eax,[rsp+40+8]          ; EAX = Lines per block , vertical count
;---
SPackMaColY:                ; Start cycle for lines per A macro column, by Y
mov r11,rcx                 ; R11 = Source base, reloaded at each line 
mov ebx,r9d                 ; EBX = Reload atoms counter
SPackMaColX:                ; Start cycle for atoms per line, by X
;---
vmovapd ymm0,[r11]          ; Read first line of atom (address)
vmovapd ymm1,[r11+r8]       ; Read second line of atom (address + Xsize)
vmovapd [rdx+32*0],ymm0     ; Sequentally store to destination, bytes 00-31
vmovapd [rdx+32*1],ymm1     ; Sequentally store to destination, bytes 32-63
;---
add r11,32                  ; Modify source address: X+1 , 1 atom right
add rdx,64                  ; Modify destination address: 64 bytes per A-atom
dec ebx                     ; Count unit = atom
jnz SPackMaColX             ; Cycle for atoms per line, by X
;---
add rcx,r10                 ; Modify source address: Y+2 , 1 atom down
dec eax                     ; Count unit = line
jnz SPackMaColY             ; Cycle for lines, by Y
;--------- Restore registers by Microsoft x64 calling convention, return ------;
pop rbx
ret


;---------- This branch used for A_t mode -------------------------------------;
SModeTranspA:

;---------- Save registers by Microsoft x64 calling convention ----------------;
push rbx rsi rdi rbp
mov rbp,rsp                   ; RBP = Pointer to save XMM area
sub rsp,2*16+16               ; 6 XMM registers +16 for alignment
and rbp,0FFFFFFFFFFFFFFF0h    ; Address must be aligned by 16
sub rbp,2*16
vmovapd [rbp+16*00],xmm6      ; Save XMM context
vmovapd [rbp+16*01],xmm7
;---------- Prepare parameters for main cycles --------------------------------;
lea r10,[r8*2]                ; R8 = Line , R10 = 2*Line
lea r11,[r8+r10]              ; R11 = 3*Line
lea rdi,[r10*2]               ; RDI = 4*Line, for Y+4
;---------- Main cycles -------------------------------------------------------;
mov eax,[rsp+40+32+2*16+16]   ; EAX = Lines per block , vertical count
SPackBlockYa:                 ; Start cycle for lines per block, by Y
mov rbx,rcx                   ; RBX = Source base, reloaded at each line 
mov esi,r9d                   ; ESI = Reload atoms counter
SPackBlockXa:                 ; Start cycle for atoms per line, by X
;--- Repack, variant with 256-bit writes with software write-combining ---
vmovapd xmm0,[rbx]            ; XMM0  = 00, 01
vmovapd xmm1,[rbx+r8]         ; XMM1  = 10, 11 
vmovapd xmm2,[rbx+r10]        ; XMM2  = 20, 21
vmovapd xmm3,[rbx+r11]        ; XMM3  = 30, 31
vunpcklpd xmm4,xmm0,xmm1      ; XMM4  = 00, 10
vunpcklpd xmm5,xmm2,xmm3      ; XMM5  = 20, 30
vunpckhpd xmm6,xmm0,xmm1      ; XMM6  = 01, 11
vunpckhpd xmm7,xmm2,xmm3      ; XMM7  = 21, 31
vinsertf128 ymm4,ymm4,xmm5,1  ; YMM4  = 00, 10, 20, 30
vinsertf128 ymm6,ymm6,xmm7,1  ; YMM6  = 01, 11, 21, 31        
vmovapd [rdx],ymm4            ; Store   00, 10, 20, 30 (256-bit write)
vmovapd [rdx+32],ymm6         ; Store   01, 11, 21, 31 (256-bit write)
;---
add rbx,rdi                   ; Modify source address: Y+4 , 1 atom down
add rdx,64                    ; Modify destination address: 64 bytes per A-atom
dec esi                       ; Atoms counter - 1
jnz SPackBlockXa              ; End cycle for atoms, by X
;---
add rcx,16                    ; Modify source address: X+1 , 1 atom right
dec eax                       ; A-Lines counter - 1
jnz SPackBlockYa              ; End cycle for lines, by Y
;--------- Restore registers by Microsoft x64 calling convention, return ------;
vzeroupper
vmovapd xmm6, [rbp+16*00]     ; Restore XMM context
vmovapd xmm7, [rbp+16*01]
add rsp,2*16+16               ; Remove stack frame for XMM
pop rbp rdi rsi rbx
ret



;------------------------------------------------------------------------------;

; This old variant - for series of blocks,
; new variant for one block.



;---------- Select method = F (A transpose) -----------------------------------;
;cmp dword [rsp+56],0        ; Check transpose flag
;jne SModeTranspA            ; Go if A transposed
;
;---------- Save registers by Microsoft x64 calling convention ----------------;
;push rbx rsi rdi rbp        ; RBX, RSI, RDI is non-volatile by convention
;---------- Prepare parameters for main cycles --------------------------------;
;lea r10,[r8*2]              ; R8 = LineFull , R10 = 2*LineFull (bytes)
;mov esi,r9d
;shl esi,5                   ; RSI = LineBlock (bytes)
;mov rdi,rcx                 ; RDI = A-pointer backup  
;---------- Main cycles -------------------------------------------------------;
;mov ebp,[rsp+48+32]         ; EBP = Number of blocks (Na)
;---
;SPackNextMc:
;mov eax,[rsp+40+32]         ; EAX = Lines per block , vertical count
;mov rcx,rdi                 ; Reload A-pointer
;---
;SPackMaColY:                ; Start cycle for lines per A macro column, by Y
;mov r11,rcx                 ; R11 = Source base, reloaded at each line 
;mov ebx,r9d                 ; EBX = Reload atoms counter
;SPackMaColX:                ; Start cycle for atoms per line, by X
;---
;vmovapd ymm0,[r11]          ; Read first line of atom (address)
;vmovapd ymm1,[r11+r8]       ; Read second line of atom (address + Xsize)
;vmovapd [rdx+32*0],ymm0     ; Sequentally store to destination, bytes 00-31
;vmovapd [rdx+32*1],ymm1     ; Sequentally store to destination, bytes 32-63
;---
;add r11,32                  ; Modify source address: X+1 , 1 atom right
;add rdx,64                  ; Modify destination address: 64 bytes per A-atom
;dec ebx                     ; Count unit = atom
;jnz SPackMaColX             ; Cycle for atoms per line, by X
;---
;add rcx,r10                 ; Modify source address: Y+2 , 1 atom down
;dec eax                     ; Count unit = line
;jnz SPackMaColY             ; Cycle for lines, by Y
;---
;add rdi,rsi                 ; Shift A-source pointer to next block
;dec ebp                     ; Count blocks
;jnz SPackNextMc             ; Cycle for A-blocks series
;--------- Restore registers by Microsoft x64 calling convention, return ------;
;pop rbp rdi rsi rbx
;ret


;---------- This branch used for A_t mode -------------------------------------;
;SModeTranspA:

;---------- Save registers by Microsoft x64 calling convention ----------------;
;push rbx rsi rdi rbp r12 r13 r14
;mov rbp,rsp                   ; RBP = Pointer to save XMM area
;sub rsp,2*16+16               ; 6 XMM registers +16 for alignment
;and rbp,0FFFFFFFFFFFFFFF0h    ; Address must be aligned by 16
;sub rbp,2*16
;vmovapd [rbp+16*00],xmm6      ; Save XMM context
;vmovapd [rbp+16*01],xmm7
;---------- Prepare parameters for main cycles --------------------------------;
;lea r10,[r8*2]                ; R8 = Line , R10 = 2*Line
;lea r11,[r8+r10]              ; R11 = 3*Line
;lea rdi,[r10*2]               ; RDI = 4*Line, for Y+4
;mov r12d,r9d
;imul r12d,r8d                 ; R12 = LineBlock (bytes)
;mov r13,rcx                   ; R13 = A-pointer backup  
;---------- Main cycles -------------------------------------------------------;
;mov r14d,[rsp+48+56+2*16+16]  ; EBP = Number of blocks
;---
;SPackNextBlk:
;mov eax,[rsp+40+56+2*16+16]   ; EAX = Lines per block , vertical count
;mov rcx,r13                   ; Reload A-pointer
;SPackBlockYa:                 ; Start cycle for lines per block, by Y
;mov rbx,rcx                   ; RBX = Source base, reloaded at each line 
;mov esi,r9d                   ; ESI = Reload atoms counter
;SPackBlockXa:                 ; Start cycle for atoms per line, by X
;--- Repack, variant with 256-bit writes with software write-combining ---
;vmovapd xmm0,[rbx]            ; XMM0  = 00, 01
;vmovapd xmm1,[rbx+r8]         ; XMM1  = 10, 11 
;vmovapd xmm2,[rbx+r10]        ; XMM2  = 20, 21
;vmovapd xmm3,[rbx+r11]        ; XMM3  = 30, 31
;vunpcklpd xmm4,xmm0,xmm1      ; XMM4  = 00, 10
;vunpcklpd xmm5,xmm2,xmm3      ; XMM5  = 20, 30
;vunpckhpd xmm6,xmm0,xmm1      ; XMM6  = 01, 11
;vunpckhpd xmm7,xmm2,xmm3      ; XMM7  = 21, 31
;vinsertf128 ymm4,ymm4,xmm5,1  ; YMM4  = 00, 10, 20, 30
;vinsertf128 ymm6,ymm6,xmm7,1  ; YMM6  = 01, 11, 21, 31        
;vmovapd [rdx],ymm4            ; Store   00, 10, 20, 30 (256-bit write)
;vmovapd [rdx+32],ymm6         ; Store   01, 11, 21, 31 (256-bit write)
;---
;add rbx,rdi                   ; Modify source address: Y+4 , 1 atom down
;add rdx,64                    ; Modify destination address: 64 bytes per A-atom
;dec esi                       ; Atoms counter - 1
;jnz SPackBlockXa              ; End cycle for atoms, by X
;---
;add rcx,16                    ; Modify source address: X+1 , 1 atom right
;dec eax                       ; A-Lines counter - 1
;jnz SPackBlockYa              ; End cycle for lines, by Y
;---
;add r13,r12                   ; Shift A-source pointer to next block
;dec r14d                      ; Count blocks
;jnz SPackNextBlk              ; Cycle for A-blocks series
;--------- Restore registers by Microsoft x64 calling convention, return ------;
;vzeroupper
;vmovapd xmm6, [rbp+16*00]     ; Restore XMM context
;vmovapd xmm7, [rbp+16*01]
;add rsp,2*16+16               ; Remove stack frame for XMM
;pop r14 r13 r12 rbp rdi rsi rbx
;ret



