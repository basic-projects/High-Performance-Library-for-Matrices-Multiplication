;--- Variant Wb=3 , Hb=3N+3 ---------------------------------------------------;

;--- Entry procedure by Microsoft x64 calling convention ---;
PROCEDURE_ENTRY_V1_7P
;--- Parameters for external cycle ---
mov r11,rcx               ; R11 = Store base address - Pointer A
mov r13d,Nb               ; R13D = Counter of B-blocks
;--- Start of B-blocks series cycle ---
.L3:
mov rcx,r11               ; Restore macro column address
mov eax,N_x3_x4
lea eax,[eax*3+3]
imul eax,128*3
lea r10,[rdx+rax]         ; R10  = B-prefetch pointer, for th. fnc. Hb,Wb=fixed
cmp r13d,1                ; R13D = Counter, if R13D=1 last block used
cmove r10,r9              ; R10  = Overload by external pointer = R9               
xor r12d,r12d             ; R12D = Clear latent store flag
mov r15d,Ha               ; R15D = Counter of lines in the A-macrocolumn
;--- Start of A-lines series cycle (one macro-column , one block cycle) ---
.L2:                     
mov eax,N_x3_x4
lea eax,[eax*3+3]         ; Can't use EAX repeately: cycle iterations
shl eax,6                 ; 2^6 = 64 bytes per atom
add rax,rcx               ; RAX = Prefetch pointer
cmp r15d,1                ; Detect last line in the macrocolumn
cmove rax,r11             ; If last line, reset prefetch to start of block
mov rdi,rdx               ; RDI = Reload pointer B, use same B-block
mov ebx,3                 ; EBX = B-lines counter, Wb=3
;--- Start of B-lines sequence, not used: Wb=1 ---
.L1:                      ; Cycle for lines of B-block
mov rsi,rcx               ; RSI = Reload pointer A, use same A-line
;--- Atoms sequence: RSI=A, RDI=B, R8=C, RAX=A-pref., R12D=Lat. store flag ---
PREFETCH_128_B            ; Use R10
HORIZONTAL_ADD
STEPS_1_10_PREFETCH_A
STEPS_33_54
STEPS_33_54
mov ebp,N_x3_x4           ; EBP = Counter of atoms, for unrolled cycle 
.L0:                      ; Cycle for atoms, this function not used this cycle
STEPS_33_54
STEPS_33_54_PREFETCH_A
STEPS_33_54
dec ebp                   ; EBP = Counter of atoms, for unrolled cycle
jnz .L0                   ; Cycle for atoms in the line
STEPS_61_74
;--- End of atoms sequence ---
dec ebx                   ; EBX = Counter of B-lines
jnz .L1                   ; Cycle for B-lines
;--- End of B-lines sequence ---
mov rcx,rsi               ; Next A-line
dec r15d                  ; R15D = Counter of lines in the A-macrocolumn 
jnz .L2                   ; Cycle for lines in the A-macrocolumn 
HORIZONTAL_ADD
STORE_ATOM
;--- End of A-lines series cycle (one macro-column , one block cycle) ---
mov rdx,rdi               ; Next B-block
dec r13d                  ; R13D = Counter of B-blocks
jnz .L3                   ; Cycle for B-blocks
;--- End of B-blocks series cycle ---
;--- Exit procedure by Microsoft x64 calling convention ---;
PROCEDURE_EXIT_V1
;--- End ---;


