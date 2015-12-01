;---------------------------------------------------------------------------;
;                                                                           ;
;             Linear Algebra Library, part 1 = Platform Detector            ;
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
DetectVXLib:
include 'DetectVXLib.inc'

align 64
DetectCache:
include 'DetectCache.inc'

;---------- Data section ---------------------------------------------------;
; Note windows failure if data section empty (exist but empty?)
section '.data' data readable writeable

MACRO LibEntry x1,x2,x3,x4
{
DB x1, x2, x3
DD x4
}

LibraryList:
LibEntry  '-' , '-' , 'AVX1_256_1' , 256
LibEntry  '-' , '-' , 10 DUP (0)   , 0
LibEntry  '-' , '-' , 10 DUP (0)   , 0
LibEntry  '-' , '-' , 10 DUP (0)   , 0
LibEntry  '-' , '-' , 10 DUP (0)   , 0
LibEntry  '-' , '-' , 10 DUP (0)   , 0
LibEntry  '-' , '-' , 10 DUP (0)   , 0
LibEntry  '-' , '-' , 10 DUP (0)   , 0
LibEntry  '-' , '-' , 10 DUP (0)   , 0
LibEntry  '-' , '-' , 10 DUP (0)   , 0
LibEntry  '-' , '-' , 10 DUP (0)   , 0
LibEntry  '-' , '-' , 10 DUP (0)   , 0
LibEntry  '-' , '-' , 10 DUP (0)   , 0
LibEntry  '-' , '-' , 10 DUP (0)   , 0
LibEntry  '-' , '-' , 10 DUP (0)   , 0
LibEntry  '-' , '-' , 10 DUP (0)   , 0
LibEntry  '-' , '-' , 10 DUP (0)   , 0
LibEntry  '-' , '-' , 10 DUP (0)   , 0
LibEntry  '-' , '-' , 10 DUP (0)   , 0
LibEntry  '-' , '-' , 10 DUP (0)   , 0

;---------- Export data section --------------------------------------------;
section '.edata' export data readable
export  'AVX_OS.dll',\
DetectVXLib  , 'DetectVXLib'  , \
DetectCache  , 'DetectCache'

;---------- Fixups section -------------------------------------------------;
data fixups
end data

