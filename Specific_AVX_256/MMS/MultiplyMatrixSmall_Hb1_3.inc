;--- Variant Hb=1 , Wb=3 ------------------------------------------------------;

;--- Entry procedure by Microsoft x64 calling convention ---;
PROCEDURE_ENTRY_V2_6P
;--- Parameters of external cycle ---
;- mov r11,rcx              ; R11 = Store pointer A backup
mov r15,r8                  ; R15 = Store pointer C
mov r13d,Nb                 ; R13D = Global number of B-blocks
;--- Start series of B-blocks ---;
.L3:                        ; Start cycle for series of B-blocks
;- mov rcx,r11              ; RCX = Reload pointer A , same for each iteration
mov r8,r15                  ; R8 = Reload pointer C , for each iteration
mov ebp,Ha                  ; EBP = Reload counter of C-lines
lea r14,[rdx + 128*3 ]      ; Next block = Current block + Block size 2 atoms
cmp r13d,1                  ; Conditional change address for B-block prefetch
cmove r14,r9
;- lea r10,[r14+128*3-64]   ; R10 = Limit B-blocks prefetch (for LIMITED PREF.)
;--- Start next C-line ---;
.L2:                        ; Start cycle for next C-line
mov rsi,rdx                 ; RSI = Reload dynamic pointer B
lea r12,[r8 + 64*3 ]        ; R12 = Calculate C-prefetch address , +2 atoms
;--- Start B-block of lines (not used for this function, Hb=1) ---;
.L1:                        ; Start cycle for B-lines
mov rdi,r8                  ; RDI = Reload dynamic pointer C
;--- Start line of B,C-atoms, this procedure for 1 atom ---;
BROADCASTS_PREFETCH_A       ; Load A, include RCX+64
SHIFTED_1_18_PREFETCH_C     ; Prefix part
SHIFTED_19_42_PREFETCH_C    ; UNROLL
SHIFTED_19_42_PREFETCH_C    ; UNROLL
.L0:                        ; Start cycle for atoms (not used for this f. Hb=1)
SHIFTED_43_75               ; Tail part
;--- End line of B,C-atoms, this procedure for 3 atoms ---;
PREFETCH_B_R14PLUS          ; B-prefetch by R14, advance R14
;--- End series of B-lines (not used for this function, Hb=1) ---;
mov r8,rdi                  ; R8 = Advance to next C-line
dec ebp                     ; C-lines count - 1
jnz .L2                     ; End cycle of C-lines
;--- End series of C-lines ---;
mov rdx,rsi                 ; RDX = Advance to next B-Block
dec r13d                    ; B-blocks count - 1
jnz .L3                     ; End cycle of B-blocks
;--- End series of B-blocks ---;
;--- Exit procedure by Microsoft x64 calling convention ---;
PROCEDURE_EXIT_V2
;--- End ---;



