;--- Small matrix multiply (broadcast method) interconnect file ---------------;

include 'MultiplyMatrixSmall_BC_Macro.inc'
include 'MultiplyMatrixSmall_SR_Macro.inc'

;------------------------------------------------------------------------------; 
;                                                                              ;
; Matrix multiply: broadcast method for small matrix, variant without cycle    ;
; Results ADDED (not simply stored) to output matrix C.                        ;
; All pointers must be aligned by 32.                                          ;  
;                                                                              ;
; INPUT:   Parm#1 = RCX = Pointer to input matrix A                            ;
;          Parm#2 = RDX = Pointer to input matrix B                            ;
;          Parm#3 = R8  = Pointer to output matrix C                           ;  
;          Parm#4 = R9  = Pointer for next B-block prefetch                    ;
;          Parm#5 = [RSP+40] = Ha , number of lines per C-block                ;
;          Parm#6 = [RSP+48] = Nb , number of B-blocks per series              ;
;                                                                              ;
; OUTPUT:  None                                                                ;
;------------------------------------------------------------------------------;
;                                                                              ; 
; Matrix multiply: broadcast method for small matrix, variant without cycle    ;
; Results ADDED (not simply stored) to output matrix C.                        ;
; All pointers must be aligned by 32.                                          ;  
;                                                                              ;
; INPUT:   Parm#1 = RCX = Pointer to input matrix A                            ;
;          Parm#2 = RDX = Pointer to input matrix B                            ;
;          Parm#3 = R8  = Pointer to output matrix C                           ;  
;          Parm#4 = R9  = Pointer for next B-block prefetch                    ;
;          Parm#5 = [RSP+40] = N_x3_x4 , atoms unrolled cycle counter          ;
;          Parm#6 = [RSP+48] = Ha , number of lines per C-block                ;
;          Parm#7 = [RSP+56] = Nb , number of B-blocks per series              ;
;                                                                              ;
; OUTPUT:  None                                                                ;
;------------------------------------------------------------------------------;

align 64
MultiplyMatrixSmall_Hb1_1:
include 'MultiplyMatrixSmall_Hb1_1.inc'

align 64
MultiplyMatrixSmall_Hb1_2:
include 'MultiplyMatrixSmall_Hb1_2.inc'

align 64
MultiplyMatrixSmall_Hb1_3:
include 'MultiplyMatrixSmall_Hb1_3.inc'

align 64
MultiplyMatrixSmall_Hb1_4:
include 'MultiplyMatrixSmall_Hb1_4.inc'

align 64
MultiplyMatrixSmall_Hb1_4N1:
include 'MultiplyMatrixSmall_Hb1_4N1.inc'

align 64
MultiplyMatrixSmall_Hb1_4N2:
include 'MultiplyMatrixSmall_Hb1_4N2.inc'

align 64
MultiplyMatrixSmall_Hb1_4N3:
include 'MultiplyMatrixSmall_Hb1_4N3.inc'

align 64
MultiplyMatrixSmall_Hb1_4N4:
include 'MultiplyMatrixSmall_Hb1_4N4.inc'

align 64
MultiplyMatrixSmall_Hb2_1:
include 'MultiplyMatrixSmall_Hb2_1.inc'

align 64
MultiplyMatrixSmall_Hb2_2:
include 'MultiplyMatrixSmall_Hb2_2.inc'

align 64
MultiplyMatrixSmall_Hb2_3:
include 'MultiplyMatrixSmall_Hb2_3.inc'

align 64
MultiplyMatrixSmall_Hb2_4:
include 'MultiplyMatrixSmall_Hb2_4.inc'

align 64
MultiplyMatrixSmall_Hb2_4N1:
include 'MultiplyMatrixSmall_Hb2_4N1.inc'

align 64
MultiplyMatrixSmall_Hb2_4N2:
include 'MultiplyMatrixSmall_Hb2_4N2.inc'

align 64
MultiplyMatrixSmall_Hb2_4N3:
include 'MultiplyMatrixSmall_Hb2_4N3.inc'

align 64
MultiplyMatrixSmall_Hb2_4N4:
include 'MultiplyMatrixSmall_Hb2_4N4.inc'

align 64
MultiplyMatrixSmall_Hb3_1:
include 'MultiplyMatrixSmall_Hb3_1.inc'

align 64
MultiplyMatrixSmall_Hb3_2:
include 'MultiplyMatrixSmall_Hb3_2.inc'

align 64
MultiplyMatrixSmall_Hb3_3:
include 'MultiplyMatrixSmall_Hb3_3.inc'

align 64
MultiplyMatrixSmall_Hb3_3N1:
include 'MultiplyMatrixSmall_Hb3_3N1.inc'

align 64
MultiplyMatrixSmall_Hb3_3N2:
include 'MultiplyMatrixSmall_Hb3_3N2.inc'

align 64
MultiplyMatrixSmall_Hb3_3N3:
include 'MultiplyMatrixSmall_Hb3_3N3.inc'

