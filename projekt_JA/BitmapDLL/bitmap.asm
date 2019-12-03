.data

.code

Function1 proc G: DWORD, H: DWORD, I: DWORD, J: DWORD, K: DWORD, L: DWORD, M: DWORD, N: DWORD, O: DWORD, P: DWORD, R: DWORD

mov eax, edx
add eax, ecx ;A+B
ret

Function1 endp

;ARGUMENTS: RCX - bitmap, RDX - bitmap_copy, R8 - height_from, R9 - height_to, RAX - width -> R10
;
;VARIABLES:
;R11 <- wiersz przed
;R12 <- wiersz po
;R15 <- roboczy

MaximalFilter proc bitmap: PTR, bitmap_copy: PTR, from_height: DWORD, to_height: DWORD, wid: DWORD

MOV R10, RAX	; width -> R10

;pierwszy wiersz bitmapy w programie

MOV R15, 8h		;do wymno�enia przez 8
IMUL R8, R15	;height_from * 8 -> ilosc bajt�w do przeskoczenia dla danego wiersza
ADD RCX, R8		;w RCX wska�nik na (height_from) wiersz

MOV R13, [RCX]	;do R13 wska�nik na ca�y wiersz

;petla z odczytaniem x argument�w 

PETLA:

MOV AL, [R13]
ADD R13, 1h
MOV AL, [R13]
ADD R13, 1h
MOV AL, [R13] 
ADD R13, 1h

MOV RAX, R10
SUB RAX, 1h
MOV R10, RAX
JNZ PETLA

PMAXSB 

ZERO:
ret

MaximalFilter endp

end