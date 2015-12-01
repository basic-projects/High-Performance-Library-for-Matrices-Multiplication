;--- UnPack Matrix A macro column function (with A optional transposed) -------;
; Parm#1 = RCX  = Pointer to source block (matrix A)                           ;
; Parm#2 = RDX  = Pointer to destination block (packed macro column A)         ;
; Parm#3 = R8D  = Matrix A line size, bytes (Xa)                               ;
; Parm#4 = R9D  = Macro column A atoms per line (Hb)                           ;
; Parm#5 = [RSP+40] = (DWORD) Macro column A lines (Ha)                        ;
; Parm#6 = REMOVED !                                                           ;
;          [RSP+48] = (DWORD) Transpose flag: FALSE=None, TRUE=Transpose       ;  
; Output = RAX = Not used for this function                                    ;
;------------------------------------------------------------------------------;

;---------- Select method = F (A transpose) -----------------------------------;
; cmp dword [rsp+48],0        ; Check transpose flag
; jne ModeUnTranspA           ; Go if A transposed

;---------- Save registers by Microsoft x64 calling convention ----------------;
mov eax,[rsp+40]            ; EAX = Lines per block , vertical count
push rbx                    ; RBX is non-volatile by convention
;---------- Prepare parameters for main cycles --------------------------------;
lea r10,[r8*2]              ; R8 = Line , R10 = 2*Line
;---------- Main cycles -------------------------------------------------------;
UnPackMaColY:               ; Start cycle for lines per A macro column, by Y
mov r11,rdx                 ; R11 = Destination base, reloaded at each line 
mov ebx,r9d                 ; EBX = Reload atoms counter
UnPackMaColX:               ; Start cycle for atoms per line, by X
;---

;--- SOURCE AND DESTINATION EXCHANGED ---

;- vmovapd ymm0,[r11]          ; Read first line of atom (address)
;- vmovapd ymm1,[r11+r8]       ; Read second line of atom (address + Xsize)
;- vmovapd [rdx+32*0],ymm0     ; Sequentally store to destination, bytes 00-31
;- vmovapd [rdx+32*1],ymm1     ; Sequentally store to destination, bytes 32-63

vmovapd ymm0,[rcx+32*0]     ; Sequentally load from source, bytes 00-31
vmovapd ymm1,[rcx+32*1]     ; Sequentally load from source, bytes 32-63
vmovapd [r11],ymm0          ; Store first line of atom (address)
vmovapd [r11+r8],ymm1       ; Store second line of atom (address + Xsize)

;--- SOURCE AND DESTINATION EXCHANGED ---

;---
add r11,32                  ; Modify destination address: X+1 , 1 atom right
add rcx,64                  ; Modify source address: 64 bytes per A-atom
dec ebx                     ; Count unit = atom
jnz UnPackMaColX            ; Cycle for atoms per line, by X
;---
add rdx,r10                 ; Modify destination address: Y+2 , 1 atom down
dec eax                     ; Count unit = line
jnz UnPackMaColY            ; Cycle for lines, by Y
;--------- Restore registers by Microsoft x64 calling convention, return ------;
pop rbx
ret


;---------- This branch used for A_t mode -------------------------------------;
;ModeUnTranspA:

;---------- Save registers by Microsoft x64 calling convention ----------------;
;mov eax,[rsp+40]              ; EAX = Lines per block , vertical count
;push rbx rsi rdi rbp
;mov rbp,rsp                   ; RBP = Pointer to save XMM area
;sub rsp,2*16+16 ; old=6*16+16 ; 6 XMM registers +16 for alignment
;and rbp,0FFFFFFFFFFFFFFF0h    ; Address must be aligned by 16
;sub rbp,2*16    ; old=6*16
;vmovapd [rbp+16*00],xmm6      ; Save XMM context
;vmovapd [rbp+16*01],xmm7
;- vmovapd [rbp+16*02],xmm8
;- vmovapd [rbp+16*03],xmm9
;- vmovapd [rbp+16*04],xmm10
;- vmovapd [rbp+16*05],xmm11
;---------- Prepare parameters for main cycles --------------------------------;
;lea r10,[r8*2]                ; R8 = Line , R10 = 2*Line
;lea r11,[r8+r10]              ; R11 = 3*Line
;lea rdi,[r10*2]               ; RDI = 4*Line, for Y+4
;---------- Main cycles -------------------------------------------------------;
;UnPackBlockYa:                ; Start cycle for lines per block, by Y
;mov rbx,rcx                   ; RBX = Source base, reloaded at each line 
;mov esi,r9d                   ; ESI = Reload atoms counter
;UnPackBlockXa:                ; Start cycle for atoms per line, by X
;--- Repack, variant with 256-bit writes with software write-combining ---

;--- SOURCE AND DESTINATION EXCHANGED ---

;- vmovapd xmm0,[rbx]            ; XMM0  = 00, 01
;- vmovapd xmm1,[rbx+r8]         ; XMM1  = 10, 11 
;- vmovapd xmm2,[rbx+r10]        ; XMM2  = 20, 21
;- vmovapd xmm3,[rbx+r11]        ; XMM3  = 30, 31
;- vunpcklpd xmm4,xmm0,xmm1      ; XMM4  = 00, 10
;- vunpcklpd xmm5,xmm2,xmm3      ; XMM5  = 20, 30
;- vunpckhpd xmm6,xmm0,xmm1      ; XMM6  = 01, 11
;- vunpckhpd xmm7,xmm2,xmm3      ; XMM7  = 21, 31
;- vinsertf128 ymm4,ymm4,xmm5,1  ; YMM4  = 00, 10, 20, 30
;- vinsertf128 ymm6,ymm6,xmm7,1  ; YMM6  = 01, 11, 21, 31        
;- vmovapd [rdx],ymm4            ; Store   00, 10, 20, 30 (256-bit write)
;- vmovapd [rdx+32],ymm6         ; Store   01, 11, 21, 31 (256-bit write)


;vmovapd ymm4,[rbx]            ; YMM4 = Load 00, 10, 20, 30 (256-bit read)
;vmovapd ymm6,[rbx+32]         ; YMM6 = Load 01, 11, 21, 31 (256-bit read)
;vunpcklpd xmm0,xmm4,xmm6      ; XMM0 = 00, 01
;vunpckhpd xmm1,xmm4,xmm6      ; XMM1 = 10, 11
;vextractf128 xmm4,ymm4,1      ; XMM4 = 20, 30
;vextractf128 xmm6,ymm6,1      ; XMM6 = 21, 31
;vunpcklpd xmm2,xmm4,xmm6      ; XMM2 = 20, 21
;vunpckhpd xmm3,xmm4,xmm6      ; XMM3 = 30, 31
;vmovapd [rdx],xmm0            ; Store row 0 to column: XMM0  = 00, 01
;vmovapd [rdx+r8],xmm1         ; Store row 1 to column: XMM1  = 10, 11 
;vmovapd [rdx+r10],xmm2        ; Store row 2 to column: XMM2  = 20, 21
;vmovapd [rdx+r11],xmm3        ; Store row 3 to column: XMM3  = 30, 31

;--- SOURCE AND DESTINATION EXCHANGED ---

;---
;add rdx,rdi                   ; Modify destination address: Y+4 , 1 atom down
;add rbx,64                    ; Modify source address: 64 bytes per A-atom
;dec esi                       ; Atoms counter - 1
;jnz UnPackBlockXa             ; End cycle for atoms, by X
;---
;add rcx,16                    ; Modify destination address: X+1 , 1 atom right
;dec eax                       ; A-Lines counter - 1
;jnz UnPackBlockYa             ; End cycle for lines, by Y
;--------- Restore registers by Microsoft x64 calling convention, return ------;
;vzeroupper
;vmovapd xmm6, [rbp+16*00]     ; Restore XMM context
;vmovapd xmm7, [rbp+16*01]
;- vmovapd xmm8, [rbp+16*02]
;- vmovapd xmm9, [rbp+16*03]
;- vmovapd xmm10,[rbp+16*04]
;- vmovapd xmm11,[rbp+16*05]
;add rsp,2*16+16 ; old=6*16+16 ; Remove stack frame for XMM
;pop rbp rdi rsi rbx
;ret


