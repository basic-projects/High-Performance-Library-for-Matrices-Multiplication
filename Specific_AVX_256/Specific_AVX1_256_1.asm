;---------------------------------------------------------------------------;
;                                                                           ;
;                   Linear Algebra Functions Library                        ;
;                                                                           ;
;---------------------------------------------------------------------------;

format PE64 GUI 4.0 DLL
entry  DllMain
;---------- Connect FASM and local definitions -----------------------------;
; Reserved for extensions  
include 'win64a.inc'

;---------- Macro ----------------------------------------------------------;
; Reserved for extensions 

;---------- Code section ---------------------------------------------------;
section '.text' code readable executable
;--- OS-called routine entry point ---
align 64
DllMain:
mov	eax,1     ; RAX=1 means call when start process
ret           ; Note operation with EAX clear bits RAX.[63-32]

;--- Target subroutines ---
; Detail comments about input/output parameters
; in the functions modules (include files).
; Alignment note, Sandy Bridge cache line size is 64 bytes.
;---

align 64
InitLibrary:
include 'InitLibrary.inc'

align 64
PackA:
;- include 'PackA.inc'
include 'PackA_TRANSP.inc'

align 64
PackB:
include 'PackB.inc'

align 64
UnPackA:
include 'UnPackA.inc'

align 64
UnPackB:
include 'UnPackB.inc'

align 64
PackASmall:
include 'PackASmall.inc'

align 64
PackBSmall:
include 'PackBSmall.inc'

align 64
UnPackASmall:
include 'UnPackASmall.inc'

align 64
UnPackBSmall:
include 'UnPackBSmall.inc'


;--- Matrix functions with multi-entry points ---

include 'MMB\MultiplyMatrixBig.inc'
include 'MMS\MultiplyMatrixSmall.inc'
;
;--- At this point space for extensions ---
;
;---------- Data section ---------------------------------------------------;
section '.data' data readable writeable
; Windows failure if data section empty (exist but empty?)
align 64
DB 0 ; Reserve space

;---------- Export data section --------------------------------------------;
section '.edata' export data readable
export  'Specific_AVX1_256_1.dll',\
InitLibrary                  , 'InitLibrary'                 , \
PackA                        , 'PackA'                       , \
PackB                        , 'PackB'                       , \
UnPackA                      , 'UnPackA'                     , \
UnPackB                      , 'UnPackB'                     , \
PackASmall                   , 'PackASmall'                  , \
PackBSmall                   , 'PackBSmall'                  , \
UnPackASmall                 , 'UnPackASmall'                , \
UnPackBSmall                 , 'UnPackBSmall'                , \
MultiplyMatrixBig_Wb1_1      , 'MultiplyMatrixBig_Wb1_1'     , \
MultiplyMatrixBig_Wb1_2      , 'MultiplyMatrixBig_Wb1_2'     , \
MultiplyMatrixBig_Wb1_3      , 'MultiplyMatrixBig_Wb1_3'     , \
MultiplyMatrixBig_Wb1_4      , 'MultiplyMatrixBig_Wb1_4'     , \
MultiplyMatrixBig_Wb1_4N1    , 'MultiplyMatrixBig_Wb1_4N1'   , \
MultiplyMatrixBig_Wb1_4N2    , 'MultiplyMatrixBig_Wb1_4N2'   , \
MultiplyMatrixBig_Wb1_4N3    , 'MultiplyMatrixBig_Wb1_4N3'   , \
MultiplyMatrixBig_Wb1_4N4    , 'MultiplyMatrixBig_Wb1_4N4'   , \
MultiplyMatrixBig_Wb2_1      , 'MultiplyMatrixBig_Wb2_1'     , \
MultiplyMatrixBig_Wb2_2      , 'MultiplyMatrixBig_Wb2_2'     , \
MultiplyMatrixBig_Wb2_3      , 'MultiplyMatrixBig_Wb2_3'     , \
MultiplyMatrixBig_Wb2_4      , 'MultiplyMatrixBig_Wb2_4'     , \
MultiplyMatrixBig_Wb2_4N1    , 'MultiplyMatrixBig_Wb2_4N1'   , \
MultiplyMatrixBig_Wb2_4N2    , 'MultiplyMatrixBig_Wb2_4N2'   , \
MultiplyMatrixBig_Wb2_4N3    , 'MultiplyMatrixBig_Wb2_4N3'   , \
MultiplyMatrixBig_Wb2_4N4    , 'MultiplyMatrixBig_Wb2_4N4'   , \
MultiplyMatrixBig_Wb3_1      , 'MultiplyMatrixBig_Wb3_1'     , \
MultiplyMatrixBig_Wb3_2      , 'MultiplyMatrixBig_Wb3_2'     , \
MultiplyMatrixBig_Wb3_3      , 'MultiplyMatrixBig_Wb3_3'     , \
MultiplyMatrixBig_Wb3_3N1    , 'MultiplyMatrixBig_Wb3_3N1'   , \
MultiplyMatrixBig_Wb3_3N2    , 'MultiplyMatrixBig_Wb3_3N2'   , \
MultiplyMatrixBig_Wb3_3N3    , 'MultiplyMatrixBig_Wb3_3N3'   , \
MultiplyMatrixSmall_Hb1_1    , 'MultiplyMatrixSmall_Hb1_1'   , \
MultiplyMatrixSmall_Hb1_2    , 'MultiplyMatrixSmall_Hb1_2'   , \
MultiplyMatrixSmall_Hb1_3    , 'MultiplyMatrixSmall_Hb1_3'   , \
MultiplyMatrixSmall_Hb1_4    , 'MultiplyMatrixSmall_Hb1_4'   , \
MultiplyMatrixSmall_Hb1_4N1  , 'MultiplyMatrixSmall_Hb1_4N1' , \
MultiplyMatrixSmall_Hb1_4N2  , 'MultiplyMatrixSmall_Hb1_4N2' , \
MultiplyMatrixSmall_Hb1_4N3  , 'MultiplyMatrixSmall_Hb1_4N3' , \
MultiplyMatrixSmall_Hb1_4N4  , 'MultiplyMatrixSmall_Hb1_4N4' , \
MultiplyMatrixSmall_Hb2_1    , 'MultiplyMatrixSmall_Hb2_1'   , \
MultiplyMatrixSmall_Hb2_2    , 'MultiplyMatrixSmall_Hb2_2'   , \
MultiplyMatrixSmall_Hb2_3    , 'MultiplyMatrixSmall_Hb2_3'   , \
MultiplyMatrixSmall_Hb2_4    , 'MultiplyMatrixSmall_Hb2_4'   , \
MultiplyMatrixSmall_Hb2_4N1  , 'MultiplyMatrixSmall_Hb2_4N1' , \
MultiplyMatrixSmall_Hb2_4N2  , 'MultiplyMatrixSmall_Hb2_4N2' , \  
MultiplyMatrixSmall_Hb2_4N3  , 'MultiplyMatrixSmall_Hb2_4N3' , \
MultiplyMatrixSmall_Hb2_4N4  , 'MultiplyMatrixSmall_Hb2_4N4' , \
MultiplyMatrixSmall_Hb3_1    , 'MultiplyMatrixSmall_Hb3_1'   , \
MultiplyMatrixSmall_Hb3_2    , 'MultiplyMatrixSmall_Hb3_2'   , \
MultiplyMatrixSmall_Hb3_3    , 'MultiplyMatrixSmall_Hb3_3'   , \
MultiplyMatrixSmall_Hb3_3N1  , 'MultiplyMatrixSmall_Hb3_3N1' , \
MultiplyMatrixSmall_Hb3_3N2  , 'MultiplyMatrixSmall_Hb3_3N2' , \
MultiplyMatrixSmall_Hb3_3N3  , 'MultiplyMatrixSmall_Hb3_3N3'

;---------- Fixups section -------------------------------------------------;
data fixups
end data

