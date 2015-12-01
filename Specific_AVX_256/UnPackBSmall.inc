;--- UnPack Matrix B block function (V2) --------------------------------------;
; Parm#1 = RCX  = Pointer to source block (matrix B)                           ;
; Parm#2 = RDX  = Pointer to destination block (unpacked block B)              ;
; Parm#3 = R8   = Matrix B line size, bytes (Xb)                               ;
; Parm#4 = R9   = Block B atoms per line (Wb)                                  ;
; Parm#5 = [RSP+40] = Block B lines (Hb)                                       ; 
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

UnPackBlkY:                 ; Start cycle for lines per B-block, by Y
mov r11,rdx                 ; R11 = Destination base, reloaded at each line 
mov ebx,r9d                 ; EBX = Reload atoms counter

UnPackBlkX:                 ; Start cycle for atoms per line, by X
vmovapd ymm0,[rcx+32*0]     ; Sequentally load from source, bytes 000-031
vmovapd ymm1,[rcx+32*1]     ; Sequentally load from source, bytes 032-063
vmovapd ymm2,[rcx+32*2]     ; Sequentally load from source, bytes 064-095
vmovapd ymm3,[rcx+32*3]     ; Sequentally load from source, bytes 096-127
vmovapd [r11],ymm0          ; Write line 1 of atom (address)
vmovapd [r11+r8],ymm1       ; Write line 2 of atom (address + Xsize)
vmovapd [r11+rsi],ymm2      ; Write line 3 of atom (address + Xsize*2)
vmovapd [r11+rdi],ymm3      ; Write line 4 of atom (address + Xsize*3)

add r11,32                  ; Modify destination address: X+1 , 1 atom right
add rcx,128                 ; Modify source address: 128 bytes per B-atom
dec ebx                     ; Count unit = atom
jnz UnPackBlkX              ; Cycle for atoms per line, by X

add rdx,r10                 ; Modify destination address: Y+4 , 1 atom down
dec eax                     ; Count unit = line
jnz UnPackBlkY              ; Cycle for lines, by Y

;--------- Restore registers by Microsoft x64 calling convention, return ------;

pop rdi rsi rbx
vzeroupper
ret


