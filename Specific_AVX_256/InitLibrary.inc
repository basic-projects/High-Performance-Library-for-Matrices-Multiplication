;--- Detect AVX256 support by processor and OS --------------------------------;
;                                                                              ;
; INPUT:  None                                                                 ;
;                                                                              ;
; OUTPUT: RAX = Detection status                                               ;
;                0 = OK to use AVX, supported by processor and OS              ;
;                1 = Error, AVX not supported by processor                     ;
;                2 = Error, AVX supported by processor but not sup. by OS      ;
;                Other codes reserved                                          ;
;                                                                              ;
;------------------------------------------------------------------------------;

;--- Verify CPUID support ---
; This place reserved for check EFLAGS.21 bit toggleable (CPUID indicator)
; Can assume if control at this point, CPUID supported

;--- Save registers ---
push rbx
;--- Verify CPU capabilities ---
xor eax,eax
cpuid
cmp eax,1            ; EAX = Maximum supported function for standard CPUID
jb .BadCpu           ; Go if CPUID#1 not supported
mov eax,1
cpuid                ; Get ECX:EDX = CPU Features List
mov eax,018000000h
and ecx,eax
cmp ecx,eax
jne .BadCpu          ; Go if OSXSAVE(27) or AVX(28) not supported
;--- Verify OS capabilities ---
xor ecx,ecx
xgetbv
and al,00000110b
cmp al,00000110b     ; Go if SSE(128bit) context (bit1) or AVX(256bit) context (bit2) not supported
jne .BadOs
xor eax,eax          ; RAX=0 , AVX supported by CPU and OS , OK to use, note operation with EAX also clear bits RAX.[63-32]
jmp	.AvxDone
;--- Exit points ---
.BadCpu:
mov eax,1            ; RAX=1 , AVX not supported by CPU, note mov to EAX also clear bits RAX.[63-32]
jmp .AvxDone
.BadOs:
mov eax,2            ; RAX=2 , AVX supported by CPU but not supported by OS, note mov to EAX also clear bits RAX.[63-32]
.AvxDone:
;--- Restore registers and exit ---
pop rbx
ret
