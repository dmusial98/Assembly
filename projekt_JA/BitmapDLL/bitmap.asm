.data

.code

;ARGUMENTS: RCX - bitmap, RDX - bitmap_copy, R8 - height_from, R9 - height_to, RAX - width -> R10
;
;VARIABLES:
;
;R11 <- wskaŸnik na wiersz i-1 bitmapy
;R12 <- wskaŸnik na wiersz i+1 bitmapy
;R13 <- wskaŸnik na i-ty wiersz bitmapy
;
;R14 <- wskaŸnik na obecny wiersz kopii bitmapy
;R15 <- roboczy
;
;RBX <- licznik kolumny w wierszu
;RSI <- obecny max do porównywania

MaximalFilter proc bitmap: PTR, bitmap_copy: PTR, from_height: DWORD, to_height: DWORD, wid: DWORD

MOV R10, RAX	; width -> R10
MOV RBX, RAX	; RBX <- licznik do przeskakiwania dla danego wiersza

MOV R15, 0h
MOV RSI, R15	;inicjalizacja rejestru z maxem

MOV R15, 8h		;do wymno¿enia przez 8
IMUL R8, R15	;height_from * 8 -> ilosc bajtów do przeskoczenia do okreœlonego wiersza na pocz¹tku programu
IMUL R9, R15	;height_to * 8 -> iloœæ bajtów do przeskoczenia dla wyliczenia adresu ostatniego wiersza

ADD RCX, R8		;w RCX wskaŸnik na (height_from) wiersz bitmapy
ADD RDX, R8		;w RDX wskaŸnik na (height_from) wiersz kopii bitmapy

SUB RCX, 8h
MOV R11, [RCX]	;R11 <- wskaŸnik na i-1 wiersz bitmapy
ADD RCX, 10h	
MOV R12, [RCX]	;R12 <- wskaŸnik na i+1 wiersz bitmapy
SUB RCX, 8h	

MOV R13, [RCX]	;do R13 wskaŸnik na i-ty wiersz bitmapy
MOV R14, [RDX]	;do R14 wskaŸnik na obecny wiersz kopii bitmapy
ADD R14, 3h		;do zapisania piksela o indeksie 1 

		;petla z odczytaniem argumentów w pojedynczym wierszu 

PETLA:	MOV AL, [R11]; [i-1][j-1] 
		SUB AL, SIL	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_2		
		MOV SIL, [R11];nowy MAX

PIXEL_2:ADD R11, 3h			;[i-1][j]
		MOV AL, [R11]
		SUB AL, SIL
		JLE PIXEL_3
		MOV SIL, [R11]

PIXEL_3:ADD R11, 3h		;[i-1][j+1]
		MOV AL, [R11]
		SUB AL, SIL
		JLE PIXEL_4
		MOV SIL, [R11]

PIXEL_4:SUB R11, 6h		;[i][j-1]
		MOV AL, [R13]
		SUB AL, SIL
		JLE PIXEL_5
		MOV SIL, [R13]

PIXEL_5:ADD R13, 3h		;[i][j]
		MOV AL, [R13]
		SUB AL, SIL
		JLE PIXEL_6
		MOV SIL, [R13]

PIXEL_6:ADD R13, 3h		;[i][j+1]
		MOV AL, [R13]
		SUB AL, SIL
		JLE PIXEL_7
		MOV SIL, [R13]

PIXEL_7:SUB R13, 6h		;[i+1][j-1]
		MOV AL, [R12]
		SUB AL, SIL
		JLE PIXEL_8
		MOV SIL, [R12]

PIXEL_8:ADD R12, 3h		;[i+1][j]
		MOV AL, [R12]
		SUB AL, SIL
		JLE PIXEL_9
		MOV SIL, [R12]

PIXEL_9:ADD R12, 3h		;[i+1][j+1]
		MOV AL, [R12]
		SUB AL, SIL
		JLE END_PIXEL
		MOV SIL, [R12]

END_PIXEL:
		SUB R12, 6h
		MOV RAX, RBX
		DEC RAX
		MOV RBX, RAX
		JNZ PETLA	

WRITE_VALUE:
		MOV [R14], SIL
		ret

		

MaximalFilter endp

end