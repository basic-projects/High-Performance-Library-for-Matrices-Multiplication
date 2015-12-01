;--- Macro for Base Cycle (for V2=Small) --------------------------------------;

;--------------------------------
; y5 = YMM0
; y6 = YMM7
;
; b1 = YMM1
; b2 = YMM2
; b3 = YMM3
; b4 = YMM4
;
; Ï1 = YMM5
; Ï2 = YMM6
;
; 11 = YMM8
; 12 = YMM9
; 13 = YMM10
; 14 = YMM11
; 21 = YMM12
; 22 = YMM13
; 23 = YMM14
; 24 = YMM15
;--------------------------------
; RCX = Pointer A
; RSI = Pointer B
; RDI = Pointer C = Ï
;--------------------------------


MACRO BROADCASTS
{
vbroadcastsd ymm8, [rcx+8*0]     ; YMM8  = 11
vbroadcastsd ymm9, [rcx+8*1]     ; YMM9  = 12
vbroadcastsd ymm10,[rcx+8*2]     ; YMM10 = 13
vbroadcastsd ymm11,[rcx+8*3]     ; YMM11 = 14
vbroadcastsd ymm12,[rcx+8*4]     ; YMM12 = 21
vbroadcastsd ymm13,[rcx+8*5]     ; YMM13 = 22
vbroadcastsd ymm14,[rcx+8*6]     ; YMM14 = 23
vbroadcastsd ymm15,[rcx+8*7]     ; YMM15 = 24
add rcx,8*8                      ; Pointer A + 64 (8 numbers)
}

MACRO STEPS_1_8
{
vmovapd ymm1,[rsi+32*0]          ; 1.  Read b1
vmovapd ymm2,[rsi+32*1]          ; 2.  Read b2
vmovapd ymm5,[rdi+32*0]          ; 3.  Read Ï1
vmovapd ymm6,[rdi+32*1]          ; 4.  Read Ï2
vmulpd ymm0,ymm8,ymm1            ; 5.  y5 = 11*b1
vmulpd ymm1,ymm12,ymm1           ; 6.  b1 = 21*b1
vmulpd ymm7,ymm9,ymm2            ; 7.  y6 = 12*b2
vmulpd ymm2,ymm13,ymm2           ; 8.  b2 = 22*b2
}

MACRO STEPS_9_32
{
vmovapd ymm3,[rsi+32*2]          ; 9.  Read b3_next 
vmovapd ymm4,[rsi+32*3]          ; 10. Read b4_next
add rsi,32*4                     ; Pointer B + 128 (16 numbers)
vaddpd ymm5,ymm5,ymm0            ; 11. Ï1 = Ï1 + y5
vmulpd ymm0,ymm10,ymm3           ; 12. y5 = 13*b3
vaddpd ymm6,ymm6,ymm1            ; 13. Ï2 = Ï2 + b1
vmulpd ymm3,ymm14,ymm3           ; 14. b3 = 23*b3
vaddpd ymm5,ymm5,ymm7            ; 15. Ï1 = Ï1 + y6
vmulpd ymm7,ymm11,ymm4           ; 16. y6 = 14*b4
vaddpd ymm6,ymm6,ymm2            ; 17. Ï2 = Ï2 + b2
vmulpd ymm4,ymm15,ymm4           ; 18. b4 = 24*b4
vmovapd ymm1,[rsi+32*0]          ; 19. Read b1_next
vmovapd ymm2,[rsi+32*1]          ; 20. Read b2_next
vaddpd ymm5,ymm5,ymm0            ; 21. Ï1 = Ï1 + y5
vmulpd ymm0,ymm8,ymm1            ; 22. y5 = 11*b1 
vaddpd ymm6,ymm6,ymm3            ; 23. Ï2 = Ï2 + b3
vmulpd ymm1,ymm12,ymm1           ; 24. b1 = a2*b1            21 ?
vaddpd ymm5,ymm5,ymm7            ; 25. Ï1 = Ï1 + y6
vmulpd ymm7,ymm9,ymm2            ; 26. y6 = a1*b2            12 ?
vaddpd ymm6,ymm6,ymm4            ; 27. Ï2 = Ï2 + b4
vmulpd ymm2,ymm13,ymm2           ; 28. b2 = a2*b2            22 ?
vmovapd [rdi+32*0],ymm5          ; 29. Write Ï1
vmovapd [rdi+32*1],ymm6          ; 30. Write Ï2
add rdi,32*2                     ; Pointer C + 64 (8 numbers)
vmovapd ymm5,[rdi+32*0]          ; 31. Read Ï1_next
vmovapd ymm6,[rdi+32*1]          ; 32. Read Ï2_next
}

MACRO STEPS_60_75
{
vmovapd ymm3,[rsi+32*2]          ; 60. Read b3_next 
vmovapd ymm4,[rsi+32*3]          ; 61. Read b4_next
add rsi,32*4                     ; Pointer B + 128 (16 numbers)
vaddpd ymm5,ymm5,ymm0            ; 62. Ï1 = Ï1 + y5
vmulpd ymm0,ymm10,ymm3           ; 63. y5 = 13*b3
vaddpd ymm6,ymm6,ymm1            ; 64. Ï2 = Ï2 + b1
vmulpd ymm3,ymm14,ymm3           ; 65. b3 = 23*b3
vaddpd ymm5,ymm5,ymm7            ; 66. Ï1 = Ï1 + y6
vmulpd ymm7,ymm11,ymm4           ; 67. y6 = 14*b4
vaddpd ymm6,ymm6,ymm2            ; 68. Ï2 = Ï2 + b2
vmulpd ymm4,ymm15,ymm4           ; 69. b4 = 24*b4
vaddpd ymm5,ymm5,ymm0            ; 70. Ï1 = Ï1 + y5
vaddpd ymm6,ymm6,ymm3            ; 71. Ï2 = Ï2 + b3
vaddpd ymm5,ymm5,ymm7            ; 72. Ï1 = Ï1 + y6
vaddpd ymm6,ymm6,ymm4            ; 73. Ï2 = Ï2 + b4
vmovapd [rdi+32*0],ymm5          ; 74. Write Ï1
vmovapd [rdi+32*1],ymm6          ; 75. Write Ï2
add rdi,32*2                     ; Pointer C + 64 (8 numbers)
}

;------------------------------------------------------------------------------;


MACRO SHIFTED_1_18
{
vmovapd ymm1,[rsi+32*0]          ; 1.  Read b1
vmovapd ymm2,[rsi+32*1]          ; 2.  Read b2
vmovapd ymm5,[rdi+32*0]          ; 3.  Read Ï1
vmovapd ymm6,[rdi+32*1]          ; 4.  Read Ï2
vmulpd ymm0,ymm8,ymm1            ; 5.  y5 = 11*b1
vmulpd ymm1,ymm12,ymm1           ; 6.  b1 = 21*b1
vmulpd ymm7,ymm9,ymm2            ; 7.  y6 = 12*b2
vmulpd ymm2,ymm13,ymm2           ; 8.  b2 = 22*b2
vmovapd ymm3,[rsi+32*2]          ; 9.  Read b3_next 
vmovapd ymm4,[rsi+32*3]          ; 10. Read b4_next
add rsi,32*4                     ; Pointer B + 128 (16 numbers)
vaddpd ymm5,ymm5,ymm0            ; 11. Ï1 = Ï1 + y5
vmulpd ymm0,ymm10,ymm3           ; 12. y5 = 13*b3
vaddpd ymm6,ymm6,ymm1            ; 13. Ï2 = Ï2 + b1
vmulpd ymm3,ymm14,ymm3           ; 14. b3 = 23*b3
vaddpd ymm5,ymm5,ymm7            ; 15. Ï1 = Ï1 + y6
vmulpd ymm7,ymm11,ymm4           ; 16. y6 = 14*b4
vaddpd ymm6,ymm6,ymm2            ; 17. Ï2 = Ï2 + b2
vmulpd ymm4,ymm15,ymm4           ; 18. b4 = 24*b4
}

MACRO SHIFTED_19_42
{
vmovapd ymm1,[rsi+32*0]          ; 19. Read b1_next
vmovapd ymm2,[rsi+32*1]          ; 20. Read b2_next
vaddpd ymm5,ymm5,ymm0            ; 21. Ï1 = Ï1 + y5
vmulpd ymm0,ymm8,ymm1            ; 22. y5 = 11*b1 
vaddpd ymm6,ymm6,ymm3            ; 23. Ï2 = Ï2 + b3
vmulpd ymm1,ymm12,ymm1           ; 24. b1 = a2*b1            21 ?
vaddpd ymm5,ymm5,ymm7            ; 25. Ï1 = Ï1 + y6
vmulpd ymm7,ymm9,ymm2            ; 26. y6 = a1*b2            12 ?
vaddpd ymm6,ymm6,ymm4            ; 27. Ï2 = Ï2 + b4
vmulpd ymm2,ymm13,ymm2           ; 28. b2 = a2*b2            22 ?
vmovapd [rdi+32*0],ymm5          ; 29. Write Ï1
vmovapd [rdi+32*1],ymm6          ; 30. Write Ï2
add rdi,32*2                     ; Pointer C + 64 (8 numbers)
vmovapd ymm5,[rdi+32*0]          ; 31. Read Ï1_next
vmovapd ymm6,[rdi+32*1]          ; 32. Read Ï2_next
vmovapd ymm3,[rsi+32*2]          ; 33. Read b3_next 
vmovapd ymm4,[rsi+32*3]          ; 34. Read b4_next
add rsi,32*4                     ; Pointer B + 128 (16 numbers)
vaddpd ymm5,ymm5,ymm0            ; 35. Ï1 = Ï1 + y5
vmulpd ymm0,ymm10,ymm3           ; 36. y5 = 13*b3
vaddpd ymm6,ymm6,ymm1            ; 37. Ï2 = Ï2 + b1
vmulpd ymm3,ymm14,ymm3           ; 38. b3 = 23*b3
vaddpd ymm5,ymm5,ymm7            ; 39. Ï1 = Ï1 + y6
vmulpd ymm7,ymm11,ymm4           ; 40. y6 = 14*b4
vaddpd ymm6,ymm6,ymm2            ; 41. Ï2 = Ï2 + b2
vmulpd ymm4,ymm15,ymm4           ; 42. b4 = 24*b4
}

MACRO SHIFTED_43_75
{
; vmovapd ymm1,[rsi+32*0]          ; 43. Read b1_next
; vmovapd ymm2,[rsi+32*1]          ; 44. Read b2_next
; vaddpd ymm5,ymm5,ymm0            ; 45. Ï1 = Ï1 + y5
; vmulpd ymm0,ymm8,ymm1            ; 46. y5 = 11*b1 
; vaddpd ymm6,ymm6,ymm3            ; 47. Ï2 = Ï2 + b3
; vmulpd ymm1,ymm12,ymm1           ; 48. b1 = a2*b1
; vaddpd ymm5,ymm5,ymm7            ; 49. Ï1 = Ï1 + y6
; vmulpd ymm7,ymm9,ymm2            ; 50. y6 = a1*b2
; vaddpd ymm6,ymm6,ymm4            ; 51. Ï2 = Ï2 + b4
; vmulpd ymm2,ymm13,ymm2           ; 52. b2 = a2*b2
; vmovapd [rdi+32*0],ymm5          ; 53. Write Ï1
; vmovapd [rdi+32*1],ymm6          ; 54. Write Ï2
; add rdi,32*2                     ; Pointer C + 64 (8 numbers)
; vmovapd ymm5,[rdi+32*0]          ; 55. Read Ï1_next
; vmovapd ymm6,[rdi+32*1]          ; 56. Read Ï2_next
;                                 ; 57,58,59 not used
; vmovapd ymm3,[rsi+32*2]          ; 60. Read b3_next 
; vmovapd ymm4,[rsi+32*3]          ; 61. Read b4_next
; add rsi,32*4                     ; Pointer B + 128 (16 numbers)
; vaddpd ymm5,ymm5,ymm0            ; 62. Ï1 = Ï1 + y5
; vmulpd ymm0,ymm10,ymm3           ; 63. y5 = 13*b3
; vaddpd ymm6,ymm6,ymm1            ; 64. Ï2 = Ï2 + b1
; vmulpd ymm3,ymm14,ymm3           ; 65. b3 = 23*b3
; vaddpd ymm5,ymm5,ymm7            ; 66. Ï1 = Ï1 + y6
; vmulpd ymm7,ymm11,ymm4           ; 67. y6 = 14*b4
; vaddpd ymm6,ymm6,ymm2            ; 68. Ï2 = Ï2 + b2
; vmulpd ymm4,ymm15,ymm4           ; 69. b4 = 24*b4
vaddpd ymm5,ymm5,ymm0            ; 70. Ï1 = Ï1 + y5
vaddpd ymm6,ymm6,ymm3            ; 71. Ï2 = Ï2 + b3
vaddpd ymm5,ymm5,ymm7            ; 72. Ï1 = Ï1 + y6
vaddpd ymm6,ymm6,ymm4            ; 73. Ï2 = Ï2 + b4
vmovapd [rdi+32*0],ymm5          ; 74. Write Ï1
vmovapd [rdi+32*1],ymm6          ; 75. Write Ï2
add rdi,32*2                     ; Pointer C + 64 (8 numbers)
}


;------------------------------------------------------------------------------;


MACRO BROADCASTS_PREFETCH_A
{
BROADCASTS             ; Broadcast load 8 registers
prefetcht0 [rcx]       ; A-prefetch (L1)
}


MACRO PREFETCH_B_R14PLUS
{
prefetchnta [r14]      ; B-prefetch (L2)
add r14,64             ; Modify pointer
}


MACRO PREFETCH_B_R14PLUS_LIMITED
{
;---
prefetchnta [r14]      ; B-prefetch (L2)
add r14,64             ; Prefetch pointer + 64
cmp r14,r10            ; Compare R14=Pointer and R10=Limit
cmova r14,r10          ; Overload R14 if R14>=R10 (if limit detected)
;---
;xor eax,eax
;cmp r14,r10
;cmovbe rax,r14
;prefetchnta [rax]
;add r14,64
;---
;mov rax,rsi
;cmp r14,r10
;cmovbe rax,r14
;prefetchnta [rax]
;add r14,64
;---
}


MACRO SHIFTED_1_18_PREFETCH_C
{
prefetcht0 [r12]            ; C-prefetch
add r12,64
SHIFTED_1_18
}


MACRO SHIFTED_19_42_PREFETCH_C
{
prefetcht0 [r12]            ; C-prefetch
add r12,64
SHIFTED_19_42
}

