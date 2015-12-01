;--- Macro for procedures entry and exit (for V1=Big) -------------------------;

MACRO PROCEDURE_ENTRY_V1_6P  ; For 6 input parameters
{
;--- Store registers, non-volatile by Microsoft x64 calling convention ---
vzeroupper
push rbx rsi rdi rbp r12 r13 r14 r15
sub rsp,10*16+8              ; 10 XMM registers, required align 16
vmovapd [rsp+16*00],xmm6     ; Save XMM context
vmovapd [rsp+16*01],xmm7
vmovapd [rsp+16*02],xmm8
vmovapd [rsp+16*03],xmm9
vmovapd [rsp+16*04],xmm10
vmovapd [rsp+16*05],xmm11
vmovapd [rsp+16*06],xmm12
vmovapd [rsp+16*07],xmm13
vmovapd [rsp+16*08],xmm14
vmovapd [rsp+16*09],xmm15
;--- Store input parameters in the parameters shadow ---
ShadowRCX  EQU  [ rsp + 10*16+8 + 64 + 08 + 00 ]
ShadowRDX  EQU  [ rsp + 10*16+8 + 64 + 08 + 08 ]
ShadowR8   EQU  [ rsp + 10*16+8 + 64 + 08 + 16 ]
ShadowR9   EQU  [ rsp + 10*16+8 + 64 + 08 + 24 ]
Ha         EQU  [ rsp + 10*16+8 + 64 + 08 + 32 ]
Nb         EQU  [ rsp + 10*16+8 + 64 + 08 + 40 ]  
}


MACRO PROCEDURE_ENTRY_V1_7P  ; For 7 input parameters
{
;--- Store registers, non-volatile by Microsoft x64 calling convention ---
vzeroupper
push rbx rsi rdi rbp r12 r13 r14 r15
sub rsp,10*16+8              ; 10 XMM registers, required align 16
vmovapd [rsp+16*00],xmm6     ; Save XMM context
vmovapd [rsp+16*01],xmm7
vmovapd [rsp+16*02],xmm8
vmovapd [rsp+16*03],xmm9
vmovapd [rsp+16*04],xmm10
vmovapd [rsp+16*05],xmm11
vmovapd [rsp+16*06],xmm12
vmovapd [rsp+16*07],xmm13
vmovapd [rsp+16*08],xmm14
vmovapd [rsp+16*09],xmm15
;--- Store input parameters in the parameters shadow ---
ShadowRCX  EQU  [ rsp + 10*16+8 + 64 + 08 + 00 ]
ShadowRDX  EQU  [ rsp + 10*16+8 + 64 + 08 + 08 ]
ShadowR8   EQU  [ rsp + 10*16+8 + 64 + 08 + 16 ]
ShadowR9   EQU  [ rsp + 10*16+8 + 64 + 08 + 24 ]
N_x3_x4    EQU  [ rsp + 10*16+8 + 64 + 08 + 32 ]
Ha         EQU  [ rsp + 10*16+8 + 64 + 08 + 40 ]
Nb         EQU  [ rsp + 10*16+8 + 64 + 08 + 48 ]  
}


MACRO PROCEDURE_EXIT_V1
{
;--- Restore registers, non-volatile by Microsoft x64 calling convention ---
vmovapd xmm6,[rsp+16*00]     ; Restore XMM context
vmovapd xmm7,[rsp+16*01]
vmovapd xmm8,[rsp+16*02]
vmovapd xmm9,[rsp+16*03]
vmovapd xmm10,[rsp+16*04]
vmovapd xmm11,[rsp+16*05]
vmovapd xmm12,[rsp+16*06]
vmovapd xmm13,[rsp+16*07]
vmovapd xmm14,[rsp+16*08]
vmovapd xmm15,[rsp+16*09]
add rsp,10*16+8              ; Remove stack frame for XMM
pop r15 r14 r13 r12 rbp rdi rsi rbx
vzeroupper
ret
}
