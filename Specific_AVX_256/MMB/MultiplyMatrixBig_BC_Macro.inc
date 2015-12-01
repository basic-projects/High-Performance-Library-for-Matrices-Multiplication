;--- Macro for Base Cycle (for V1=Big) ----------------------------------------;

MACRO HORIZONTAL_ADD
{
vhaddpd ymm0,ymm8,ymm9          ; YMM0  = C0[0,1] , C1[0,1] , C0[2,3] , C1[2,3]
vhaddpd ymm1,ymm10,ymm11        ; YMM1  = C2[0,1] , C3[0,1] , C2[2,3] , C3[2,3]
vhaddpd ymm2,ymm12,ymm13        ; YMM2  = C4[0,1] , C5[0,1] , C4[2,3] , C5[2,3]
vhaddpd ymm3,ymm14,ymm15        ; YMM3  = C6[0,1] , C7[0,1] , C6[2,3] , C7[2,3]
vextractf128 xmm4,ymm0,1        ; YMM4  = C0[2,3] , C1[2,3] , 0       , 0
vextractf128 xmm5,ymm1,1        ; YMM5  = C2[2,3] , C3[2,3] , 0       , 0
vextractf128 xmm6,ymm2,1        ; YMM6  = C4[2,3] , C5[2,3] , 0       , 0
vextractf128 xmm7,ymm3,1        ; YMM7  = C6[2,3] , C7[2,3] , 0       , 0
vaddpd xmm8,xmm0,xmm4           ; YMM8  = C0[all] , C1[all] , 0       , 0
vaddpd xmm9,xmm1,xmm5           ; YMM9  = C2[all] , C3[all] , 0       , 0
vaddpd xmm10,xmm2,xmm6          ; YMM10 = C4[all] , C5[all] , 0       , 0
vaddpd xmm11,xmm3,xmm7          ; YMM11 = C6[all] , C7[all] , 0       , 0
vinsertf128 ymm3,ymm8,xmm9,1    ; YMM3  = C0 , C1 , C2 , C3
vinsertf128 ymm4,ymm10,xmm11,1  ; YMM4  = C4 , C5 , C6 , C7 
}


MACRO STORE_ATOM
{
vmovapd [r8+32*00-64],ymm3       ; 5.  Write b3 -> C
vmovapd [r8+32*01-64],ymm4       ; 6.  Write b4 -> C
}


MACRO STEPS_1_10                ; Prefix
{
;--- Prefix ---
vmovapd ymm1,[rdi+32*00]        ; 1.  Read b1
vmovapd ymm2,[rdi+32*01]        ; 2.  Read b2
vmovapd ymm0,[rsi+32*00]        ; 3.  Read a1
vmovapd ymm7,[rsi+32*01]        ; 4.  Read a2
vmulpd ymm5,ymm0,ymm1           ; 7.  y5 = a1*b1
vmulpd ymm1,ymm7,ymm1           ; 8.  b1 = a2*b1
vmulpd ymm6,ymm0,ymm2           ; 9.  y6 = a1*b2
vmulpd ymm2,ymm7,ymm2           ; 10. b2 = a2*b2
;---
vmovsd xmm8, [r8+08*00]         ; y8  <- C11
vmovsd xmm9, [r8+08*01]         ; y9  <- C21
vmovsd xmm10,[r8+08*02]         ; y10 <- C31
vmovsd xmm11,[r8+08*03]         ; y11 <- C41 
vmovsd xmm12,[r8+08*04]         ; y12 <- C12
vmovsd xmm13,[r8+08*05]         ; y13 <- C22
vmovsd xmm14,[r8+08*06]         ; y14 <- C32
vmovsd xmm15,[r8+08*07]         ; y15 <- C42 
;---
test r12d,r12d
jz @f
vmovapd [r8+32*00-64],ymm3      ; 5.  Write b3 -> C
vmovapd [r8+32*01-64],ymm4      ; 6.  Write b4 -> C
@@:
;---
add rsi,32*02                   ; Modify A-pointer
add rdi,32*02                   ; Modify B-pointer
add r8,32*02                    ; Modify C-pointer
mov r12d,1
}

MACRO STEPS_1_10_PREFETCH_A    ; Prefix
{
STEPS_1_10
prefetcht0 [rax]
add rax,64 
}


MACRO STEPS_61_74               ; Tail
{
vmovapd ymm3,[rdi+32*00]        ; 61. Read b3_next 
vmovapd ymm4,[rdi+32*01]        ; 62. Read b4_next
vaddpd ymm8,ymm8,ymm5           ; 63. C11 = C11 + y5
vmulpd ymm5,ymm0,ymm3           ; 64. y5 = a1*b3
vaddpd ymm12,ymm12,ymm1         ; 65. C21 = C21 + b1
vmulpd ymm3,ymm7,ymm3           ; 66. b3 = a2*b3
vaddpd ymm9,ymm9,ymm6           ; 67. C12 = C12 + y6
vmulpd ymm6,ymm0,ymm4           ; 68. y6 = a1*b4
vaddpd ymm13,ymm13,ymm2         ; 69. C22 = C22 + b2
vmulpd ymm4,ymm7,ymm4           ; 70. b4 = a2*b4
vaddpd ymm10,ymm10,ymm5         ; 71. C13 = C13 + y5
vaddpd ymm14,ymm14,ymm3         ; 72. C23 = C23 + b3
vaddpd ymm11,ymm11,ymm6         ; 73. C14 = Ñ14 + y6
vaddpd ymm15,ymm15,ymm4         ; 74. C24 = C24 + b4
add rdi,32*02                   ; Modify B-pointer		
}


MACRO STEPS_33_54          ; Cycle iteration
{
vmovapd ymm3,[rdi+32*00]   ; 33. Read b3_next 
vmovapd ymm4,[rdi+32*01]   ; 34. Read b4_next
vaddpd ymm8,ymm8,ymm5      ; 35. C11 = C11 + y5
vmulpd ymm5,ymm0,ymm3      ; 36. y5 = a1*b3
vaddpd ymm12,ymm12,ymm1    ; 37. C21 = C21 + b1
vmulpd ymm3,ymm7,ymm3      ; 38. b3 = a2*b3
vaddpd ymm9,ymm9,ymm6      ; 39. C12 = C12 + y6
vmulpd ymm6,ymm0,ymm4      ; 40. y6 = a1*b4
vaddpd ymm13,ymm13,ymm2    ; 41. C22 = C22 + b2
vmulpd ymm4,ymm7,ymm4      ; 42. b4 = a2*b4
vmovapd ymm1,[rdi+32*02]   ; 43. Read b1_next
vmovapd ymm2,[rdi+32*03]   ; 44. Read b2_next
vmovapd ymm0,[rsi+32*00]   ; 45. Read a1_next
vmovapd ymm7,[rsi+32*01]   ; 46. Read a2_next
vaddpd ymm10,ymm10,ymm5    ; 47. C13 = C13 + y5
vmulpd ymm5,ymm0,ymm1      ; 48. y5 = a1*b1 
vaddpd ymm14,ymm14,ymm3    ; 49. C23 = C23 + b3
vmulpd ymm1,ymm7,ymm1      ; 50. b1 = a2*b1
vaddpd ymm11,ymm11,ymm6    ; 51. C14 = Ñ14 + y6
vmulpd ymm6,ymm0,ymm2      ; 52. y6 = a1*b2
vaddpd ymm15,ymm15,ymm4    ; 53. C24 = C24 + b4
vmulpd ymm2,ymm7,ymm2      ; 54. b2 = a2*b2
add rsi,32*02              ; Modify A-pointer
add rdi,32*04              ; Modify B-pointer
}


MACRO STEPS_33_54_PREFETCH_A ; Cycle iteration
{
;--- Prefetch A ---
prefetcht0 [rax]
add rax,64 
;--- Iteration ---
vmovapd ymm3,[rdi+32*00]   ; 33. Read b3_next 
vmovapd ymm4,[rdi+32*01]   ; 34. Read b4_next
vaddpd ymm8,ymm8,ymm5      ; 35. C11 = C11 + y5
vmulpd ymm5,ymm0,ymm3      ; 36. y5 = a1*b3
vaddpd ymm12,ymm12,ymm1    ; 37. C21 = C21 + b1
vmulpd ymm3,ymm7,ymm3      ; 38. b3 = a2*b3
vaddpd ymm9,ymm9,ymm6      ; 39. C12 = C12 + y6
vmulpd ymm6,ymm0,ymm4      ; 40. y6 = a1*b4
vaddpd ymm13,ymm13,ymm2    ; 41. C22 = C22 + b2
vmulpd ymm4,ymm7,ymm4      ; 42. b4 = a2*b4
vmovapd ymm1,[rdi+32*02]   ; 43. Read b1_next
vmovapd ymm2,[rdi+32*03]   ; 44. Read b2_next
vmovapd ymm0,[rsi+32*00]   ; 45. Read a1_next
vmovapd ymm7,[rsi+32*01]   ; 46. Read a2_next
vaddpd ymm10,ymm10,ymm5    ; 47. C13 = C13 + y5
vmulpd ymm5,ymm0,ymm1      ; 48. y5 = a1*b1 
vaddpd ymm14,ymm14,ymm3    ; 49. C23 = C23 + b3
vmulpd ymm1,ymm7,ymm1      ; 50. b1 = a2*b1
vaddpd ymm11,ymm11,ymm6    ; 51. C14 = Ñ14 + y6
vmulpd ymm6,ymm0,ymm2      ; 52. y6 = a1*b2
vaddpd ymm15,ymm15,ymm4    ; 53. C24 = C24 + b4
vmulpd ymm2,ymm7,ymm2      ; 54. b2 = a2*b2
add rsi,32*02              ; Modify A-pointer
add rdi,32*04              ; Modify B-pointer
}

MACRO PREFETCH_128_B
{
prefetchnta [r10]
add r10,128 
}
