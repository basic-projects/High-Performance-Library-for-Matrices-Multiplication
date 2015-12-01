;--- Variant Hb=2 , Wb=4N+2 ---------------------------------------------------;

;--- Entry procedure by Microsoft x64 calling convention ---;
PROCEDURE_ENTRY_V2_7P
;--- Parameters of external cycle ---
;- mov r11,rcx              ; R11 = Store pointer A backup
mov r15,r8                  ; R15 = Store pointer C
mov r13d,Nb                 ; R13D = Global number of B-blocks
;--- Start series of B-blocks ---;
.L3:                        ; Start cycle for series of B-blocks
;- mov rcx,r11              ; RCX = Reload pointer A , same for each iteration
mov r8,r15                  ; R8 = Reload pointer C , for each iteration
mov ebp,Ha                  ; EBP = Reload counter of C-lines
mov r14d,N_x3_x4            ; R14D = Unrolled counter
shl r14d,2                  ; R14D = Normal counter
add r14d,2                  ; R14D = Normal counter + 2
shl r14d,7+1                ; R14D = Set units = B-atoms, Wb=2 means + 1
;- mov r10,r14              ; R10  = Prepare for calculate B-prefetch limit
lea r14,[rdx+r14]           ; Next block = Current blk. + Block size
cmp r13d,1                  ; Conditional change address for B-block prefetch
cmove r14,r9
;- lea r10,[r14+r10]        ; R10 = Limit B-blocks prefetch (for LIMITED PREF.)
;--- Start next C-line ---;
.L2:                        ; Start cycle for next C-line
mov ebx,2                   ; Number of B-lines, Hb=2
mov rsi,rdx                 ; RSI = Reload dynamic pointer B
mov r12d,N_x3_x4
shl r12d,6+2                ; C-atom = 64 bytes , +2(*4) because unrolled
lea r12,[r8+r12 + 64*2 ]    ; R12 = Calculate C-prefetch address , +2 atoms
;--- Start B-block of lines ---;
.L1:                        ; Start cycle for B-lines
mov rdi,r8                  ; RDI = Reload dynamic pointer C
;--- Start line of B,C-atoms, this procedure for 1 atom ---;
mov eax,N_x3_x4             ; EAX = Reload dynamic length of line
BROADCASTS_PREFETCH_A       ; Load A, include RCX+64
SHIFTED_1_18_PREFETCH_C     ; Prefix part
SHIFTED_19_42               ; UNROLL
.L0:                        ; Start cycle for atoms
SHIFTED_19_42_PREFETCH_C    ; UNROLL
SHIFTED_19_42               ; UNROLL
SHIFTED_19_42_PREFETCH_C    ; UNROLL
SHIFTED_19_42               ; UNROLL
dec eax                     ; Atoms count - 1
jnz .L0                     ; End cycle for atoms
SHIFTED_43_75               ; Tail part
;--- End line of B,C-atoms, this procedure for 2 atoms ---;
dec ebx                     ; B-lines count - 1
jnz .L1                     ; End cycle of B-lines
PREFETCH_B_R14PLUS          ; B-prefetch by R14, advance R14 (alt. _LIMITED)
;--- End series of B-lines ---;
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




