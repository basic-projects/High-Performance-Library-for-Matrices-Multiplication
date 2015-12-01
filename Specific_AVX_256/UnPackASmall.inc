;--- UnPack Matrix A macro column function (with A optional transposed) -------;
; This variant for v2-based method, for small matrixes                         ;
;                                                                              ;
; Parm#1 = RCX  = Pointer to source block (matrix A)                           ;
; Parm#2 = RDX  = Pointer to destination block (unpacked macro column A)       ;
; Parm#3 = R8D  = Matrix A line size, bytes (Xa)                               ;
; Parm#4 = R9D  = Macro column A atoms per line (Hb)                           ;
; Parm#5 = [RSP+40] = (DWORD) Macro column A lines (Ha)                        ;
; Output = RAX = Not used for this function                                    ;
;------------------------------------------------------------------------------;


;---------- Save registers by Microsoft x64 calling convention ----------------;
push rbx                    ; RBX, RSI is non-volatile by convention
;---------- Prepare parameters for main cycles --------------------------------;
lea r10,[r8*2]              ; R8 = LineFull , R10 = 2*LineFull (bytes)
;---------- Main cycles -------------------------------------------------------;
mov eax,[rsp+40+8]          ; EAX = Lines per block , vertical count
;---
SUnPackMaColY:              ; Start cycle for lines per A macro column, by Y
mov r11,rdx                 ; R11 = Destination base, reloaded at each line 
mov ebx,r9d                 ; EBX = Reload atoms counter
SUnPackMaColX:              ; Start cycle for atoms per line, by X
;---
vmovapd ymm0,[rcx+32*0]     ; Sequentally read from source, bytes 00-31
vmovapd ymm1,[rcx+32*1]     ; Sequentally read from source, bytes 32-63
vmovapd [r11],ymm0          ; Write first line of atom (address)
vmovapd [r11+r8],ymm1       ; Write second line of atom (address + Xsize)
;---
add rcx,64                  ; Modify source address: 64 bytes per A-atom 
add r11,32                  ; Modify destination address: 1 atom right, X+1 
dec ebx                     ; Count unit = atom
jnz SUnPackMaColX           ; Cycle for atoms per line, by X
;---
add rdx,r10                 ; Modify source address: Y+2 , 1 atom down
dec eax                     ; Count unit = line
jnz SUnPackMaColY           ; Cycle for lines, by Y
;--------- Restore registers by Microsoft x64 calling convention, return ------;
pop rbx
vzeroupper
ret

