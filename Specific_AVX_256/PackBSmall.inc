;--- Pack Matrix B block function (V2) ----------------------------------------;
; Parm#1 = RCX  = Pointer to source block (matrix B)                           ;
; Parm#2 = RDX  = Pointer to destination block (packed block B)                ;
; Parm#3 = R8   = Matrix B line size, bytes (Xb)                               ;
; Parm#4 = R9   = Block B atoms per line (Wb)                                  ;
; Parm#5 = [RSP+40] = Block B lines (Hb)                                       ; 
; REJECTED PARAMETER #6                                                        ;
; Parm#6 = [RSP+48] = Number of B-blocks per series (Nb)                       ;
; Output = RAX = Not used for this function                                    ;
;------------------------------------------------------------------------------;

;---------- Save registers by Microsoft x64 calling convention ----------------;

push rbx rsi rdi            ; Registers, non-volatile by convention

;---------- Prepare parameters for main cycles --------------------------------;

lea r10,[r8*4]              ; R8  = Line , R10 = 4*Line, for 1 atom down
lea rsi,[r8*2]              ; RSI = 2*Line
lea rdi,[r8*3]              ; RDI = 3*Line

;---------- Main cycles -------------------------------------------------------;

mov eax,[rsp+40+24]         ; EAX = Lines per block , vertical count

PackBlkY:                   ; Start cycle for lines per B-block, by Y
mov r11,rcx                 ; R11 = Source base, reloaded at each line 
mov ebx,r9d                 ; EBX = Reload atoms counter

PackBlkX:                   ; Start cycle for atoms per line, by X
vmovapd ymm0,[r11]          ; Read line 1 of atom (address)
vmovapd ymm1,[r11+r8]       ; Read line 2 of atom (address + Xsize)
vmovapd ymm2,[r11+rsi]      ; Read line 3 of atom (address + Xsize*2)
vmovapd ymm3,[r11+rdi]      ; Read line 4 of atom (address + Xsize*3)
vmovapd [rdx+32*0],ymm0     ; Sequentally store to destination, bytes 000-031
vmovapd [rdx+32*1],ymm1     ; Sequentally store to destination, bytes 032-063
vmovapd [rdx+32*2],ymm2     ; Sequentally store to destination, bytes 064-095
vmovapd [rdx+32*3],ymm3     ; Sequentally store to destination, bytes 096-127

add r11,32                  ; Modify source address: X+1 , 1 atom right
add rdx,128                 ; Modify destination address: 128 bytes per B-atom
dec ebx                     ; Count unit = atom
jnz PackBlkX                ; Cycle for atoms per line, by X

add rcx,r10                 ; Modify source address: Y+4 , 1 atom down
dec eax                     ; Count unit = line
jnz PackBlkY                ; Cycle for lines, by Y

;--------- Restore registers by Microsoft x64 calling convention, return ------;

pop rdi rsi rbx
vzeroupper
ret



;------------------------------------------------------------------------------;

; This old variant - for series of blocks,
; new variant for one block.


;---------- Save registers by Microsoft x64 calling convention ----------------;
;
;push rbx rsi rdi rbp r12 r13   ; Registers, non-volatile by convention
;
;---------- Prepare parameters for main cycles --------------------------------;
;
;lea r10,[r8*4]              ; R8  = Line , R10 = 4*Line, for 1 atom down
;lea rsi,[r8*2]              ; RSI = 2*Line
;lea rdi,[r8*3]              ; RDI = 3*Line
;
;---------- Main cycles -------------------------------------------------------;
;
;mov ebp,[rsp+48+48]         ; EBP = Blocks per series
;lea r12d,[r8d*4]            ; 4 Lines = 1 atom
;imul r12d,[rsp+40+48]       ; R12 = Next block addend 
;mov r13,rcx                 ; R13 = Backup B-pointer
;
;PackBlkX2:
;mov eax,[rsp+40+48]         ; EAX = Lines per block , vertical count
;mov rcx,r13                 ; RCX = Reload B-pointer
;
;PackBlkY:                   ; Start cycle for lines per B-block, by Y
;mov r11,rcx                 ; R11 = Source base, reloaded at each line 
;mov ebx,r9d                 ; EBX = Reload atoms counter
;
;PackBlkX:                   ; Start cycle for atoms per line, by X
;vmovapd ymm0,[r11]          ; Read line 1 of atom (address)
;vmovapd ymm1,[r11+r8]       ; Read line 2 of atom (address + Xsize)
;vmovapd ymm2,[r11+rsi]      ; Read line 3 of atom (address + Xsize*2)
;vmovapd ymm3,[r11+rdi]      ; Read line 4 of atom (address + Xsize*3)
;vmovapd [rdx+32*0],ymm0     ; Sequentally store to destination, bytes 000-031
;vmovapd [rdx+32*1],ymm1     ; Sequentally store to destination, bytes 032-063
;vmovapd [rdx+32*2],ymm2     ; Sequentally store to destination, bytes 064-095
;vmovapd [rdx+32*3],ymm3     ; Sequentally store to destination, bytes 096-127
;
;add r11,32                  ; Modify source address: X+1 , 1 atom right
;add rdx,128                 ; Modify destination address: 128 bytes per B-atom
;dec ebx                     ; Count unit = atom
;jnz PackBlkX                ; Cycle for atoms per line, by X
;
;add rcx,r10                 ; Modify source address: Y+4 , 1 atom down
;dec eax                     ; Count unit = line
;jnz PackBlkY                ; Cycle for lines, by Y
;
;add r13,r12                 ; Modify B-pointer, next B-block by vertical 
;dec ebp                     ; Count blocks
;jnz PackBlkX2               ; Cycle for blocks, vertical move by B 
;
;--------- Restore registers by Microsoft x64 calling convention, return ------;
;
;pop r13 r12 rbp rdi rsi rbx
;ret
;
