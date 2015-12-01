;--- Detect CPU Caches size per 1 execution thread ----------------------------;
;                                                                              ;
; INPUT:  Parameter #1 = RCX = Pointer to output data block                    ;
;                                                                              ;
; OUTPUT: RAX = Detection status                                               ;
;                0 = Detection OK, output data valid                           ;
;                Other codes = error, output data not valid                    ;
;                                                                              ;
;         Output data, units = KB (valid if status OK, RAX=0)                  ;
;         QWORD (Bytes 00-07) = L1 Data Cache size per 1 thread                ;
;         QWORD (Bytes 08-15) = L2 Data or Unified Cache size per 1 thread     ;
;         QWORD (Bytes 16-23) = Reserved                                       ;
;         QWORD (Bytes 24-31) = Reserved                                       ; 
;                                                                              ;
;------------------------------------------------------------------------------;

;--- Verify CPUID support ---
; This place reserved for check EFLAGS.21 bit toggleable (CPUID indicator),
; assume CPUID supported if control at this point.

;--- Save registers ---
push rbx rsi rdi rcx
;--- Verify CPU and OS capabilities ---
xor eax,eax
cpuid
cmp eax,4                  ; EAX = Maximum supported function for standard CPUID
jb .BadCache               ; Go if CPUID#4 not supported
;--- Detect L1 Data Cache ---
mov esi,21h                ; Level=1 , Type=Data Cache
call Find_Cache_By_Type
push rax
call Calculate_Cache_Size
pop rbx
call Calculate_Cache_Threads
test rbx,rbx
jz .BadCache               ; Go error if this type of cache not valid 
;--- Calculate L1 per thread ---
xor edx,edx                ; Clear dividend.high , note operation with EDX also clear bits RDX.[63-32]
div rbx
;--- Save L1 Data Cache ---
mov rcx,[rsp]              ; RBX = Input RCX image, pointer to function output data
mov [rcx],rax              ; Update function output data
;--- Store number of threads ---
mov [rcx+24],rbx
;--- Detect L2 Data Cache ---
mov esi,43h                ; Level=2 , Type=Unified Cache
call Find_Cache_By_Type
push rax
call Calculate_Cache_Size
pop rbx
call Calculate_Cache_Threads
test rbx,rbx
jz .BadCache               ; Go error if this type of cache not valid 
xor edx,edx
div rbx
;--- Save L2 Unified Cache ---
mov rcx,[rsp]              ; RCX = Input RCX image, pointer to function output data
mov [rcx+8],rax            ; Update function output data
;--- Detect L3 cache ---
mov esi,63h                ; Level=3 , Type=Unified cache
call Find_Cache_By_Type
call Calculate_Cache_Size
mov rcx,[rsp]              ; RCX = Input RCX image, pointer to function output data
mov [rcx+16],rax           ; Update function output data
;--- Return point for no errors ---
xor eax,eax
jmp	.DoneCache
;--- Return point for error ---
.BadCache:
mov eax,1
.DoneCache:
;--- Restore registers and exit ---
pop rcx rdi rsi rbx
ret

;---------- Subroutines for cache detect -----------------------------------;

;---------- Find cache by type --------------------------------;
; INPUT:   ESI = Scan pattern:                                 ;
;                Bits [4-0] = Cache type                       ;
;                0=Null, 1=Data, 2=Instruction, 3=Unified      ;
;                4-31 = Reserved                               ;
;                Bits [7-5] = Cache level , 1-based            ;
;                                                              ;
; OUTPUT:  CPUID function 4 results (see CPUID description):   ;
;                EAX , EBX , ECX , EDX                         ;
;--------------------------------------------------------------;
Find_Cache_By_Type:
mov edi,-1
Find_Cache:
mov eax,4
inc edi
mov ecx,edi
cpuid
test al,00011111b
jz No_Cache
push rax
movzx eax,al
cmp eax,esi
pop rax
jne Find_Cache
ret
No_Cache:
xor eax,eax
xor ebx,ebx
xor ecx,ecx
xor edx,edx
ret

;---------- Calculate cache size --------------------;
; INPUT:   EBX = Result of CPUID function 4          ;
;          ECX = Result of CPUID function 4          ;
; OUTPUT:  EAX = Size, kilobytes                     ;
;----------------------------------------------------;
Calculate_Cache_Size:
mov eax,ebx         ; This also clear RAX[63-32]
mov ecx,ecx         ; This clear RCX[63-32]
mov rbx,rax
mov rdx,rax
;---
and eax,00000FFFh
inc eax             ; Extract bits [11-00] = Line Size
;---
shr ebx,12
and ebx,000003FFh
inc ebx             ; Extract bits [21-12] = Line partitions
;---
shr edx,22
and edx,000003FFh
inc edx             ; Extract bits [31-22] = Ways of associativity
;---
inc ecx             ; ECX = Cache sets
;---
mul rdx
mul rbx
mul rcx
shr rax,10          ; Convert Bytes to Kilobytes
ret

;---------- Calculate threads per cache level -------;
; INPUT:   EBX = Result EAX of CPUID function 4      ;
; OUTPUT:  EBX = Threads                             ;
;----------------------------------------------------;
Calculate_Cache_Threads:
shr ebx,14
and ebx,0FFFh
inc ebx
ret
