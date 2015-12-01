;--- Big matrix multiply (horizontal add method) interconnect file ------------;

include 'MultiplyMatrixBig_BC_Macro.inc'
include 'MultiplyMatrixBig_SR_Macro.inc'

;------------------------------------------------------------------------------; 
;                                                                              ;
; Matrix multiply: horizontal add method for big matrix, variant without cycle ;
; Results ADDED (not simply stored) to output matrix C.                        ;
; All pointers must be aligned by 32.                                          ;  
;                                                                              ;
; INPUT:   Parm#1 = RCX = Pointer to input matrix A                            ;
;          Parm#2 = RDX = Pointer to input matrix B                            ;
;          Parm#3 = R8  = Pointer to output matrix C                           ;  
;          Parm#4 = R9  = Pointer for next B-block prefetch                    ;
;          Parm#5 = [RSP+40] = Ha , number of lines per A-macrocolumn          ;
;          Parm#6 = [RSP+48] = Nb , number of B-blocks per series              ;
;                                                                              ;
; OUTPUT:  None                                                                ;
;------------------------------------------------------------------------------;
;                                                                              ; 
; Matrix multiply: horizontal add method for big matrix, variant without cycle ;
; Results ADDED (not simply stored) to output matrix C.                        ;
; All pointers must be aligned by 32.                                          ;  
;                                                                              ;
; INPUT:   Parm#1 = RCX = Pointer to input matrix A                            ;
;          Parm#2 = RDX = Pointer to input matrix B                            ;
;          Parm#3 = R8  = Pointer to output matrix C                           ;  
;          Parm#4 = R9  = Pointer for next B-block prefetch                    ;
;          Parm#5 = [RSP+40] = N_x3_x4 , atoms unrolled cycle counter          ;
;          Parm#6 = [RSP+48] = Ha , number of lines per A-macrocolumn          ;
;          Parm#7 = [RSP+56] = Nb , number of B-blocks per series              ;
;                                                                              ;
; OUTPUT:  None                                                                ;
;------------------------------------------------------------------------------;

align 64
MultiplyMatrixBig_Wb1_1:
include 'MultiplyMatrixBig_Wb1_1.inc'

align 64
MultiplyMatrixBig_Wb1_2:
include 'MultiplyMatrixBig_Wb1_2.inc'

align 64
MultiplyMatrixBig_Wb1_3:
include 'MultiplyMatrixBig_Wb1_3.inc'

align 64
MultiplyMatrixBig_Wb1_4:
include 'MultiplyMatrixBig_Wb1_4.inc'

align 64
MultiplyMatrixBig_Wb1_4N1:
include 'MultiplyMatrixBig_Wb1_4N1.inc'

align 64
MultiplyMatrixBig_Wb1_4N2:
include 'MultiplyMatrixBig_Wb1_4N2.inc'

align 64
MultiplyMatrixBig_Wb1_4N3:
include 'MultiplyMatrixBig_Wb1_4N3.inc'

align 64
MultiplyMatrixBig_Wb1_4N4:
include 'MultiplyMatrixBig_Wb1_4N4.inc'

align 64
MultiplyMatrixBig_Wb2_1:
include 'MultiplyMatrixBig_Wb2_1.inc'

align 64
MultiplyMatrixBig_Wb2_2:
include 'MultiplyMatrixBig_Wb2_2.inc'

align 64
MultiplyMatrixBig_Wb2_3:
include 'MultiplyMatrixBig_Wb2_3.inc'

align 64
MultiplyMatrixBig_Wb2_4:
include 'MultiplyMatrixBig_Wb2_4.inc'

align 64
MultiplyMatrixBig_Wb2_4N1:
include 'MultiplyMatrixBig_Wb2_4N1.inc'

align 64
MultiplyMatrixBig_Wb2_4N2:
include 'MultiplyMatrixBig_Wb2_4N2.inc'

align 64
MultiplyMatrixBig_Wb2_4N3:
include 'MultiplyMatrixBig_Wb2_4N3.inc'

align 64
MultiplyMatrixBig_Wb2_4N4:
include 'MultiplyMatrixBig_Wb2_4N4.inc'

align 64
MultiplyMatrixBig_Wb3_1:
include 'MultiplyMatrixBig_Wb3_1.inc'

align 64
MultiplyMatrixBig_Wb3_2:
include 'MultiplyMatrixBig_Wb3_2.inc'

align 64
MultiplyMatrixBig_Wb3_3:
include 'MultiplyMatrixBig_Wb3_3.inc'

align 64
MultiplyMatrixBig_Wb3_3N1:
include 'MultiplyMatrixBig_Wb3_3N1.inc'

align 64
MultiplyMatrixBig_Wb3_3N2:
include 'MultiplyMatrixBig_Wb3_3N2.inc'

align 64
MultiplyMatrixBig_Wb3_3N3:
include 'MultiplyMatrixBig_Wb3_3N3.inc'

