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

SUB RAX, 2h
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

		;RED
PETLA:	MOV R15, 0h
		MOV RSI, R15	;inicjalizacja rejestru z maxem

		MOV AL, [R11]; [i-1][j-1] 
		SUB AL, SIL	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_2_RED		
		MOV SIL, [R11];nowy MAX

PIXEL_2_RED:
		ADD R11, 3h			;[i-1][j]
		MOV AL, [R11]
		SUB AL, SIL
		JLE PIXEL_3_RED
		MOV SIL, [R11]

PIXEL_3_RED:
		ADD R11, 3h		;[i-1][j+1]
		MOV AL, [R11]
		SUB AL, SIL
		JLE PIXEL_4_RED
		MOV SIL, [R11]

PIXEL_4_RED:
		SUB R11, 5h		;[i][j-1]
		MOV AL, [R13]
		SUB AL, SIL
		JLE PIXEL_5_RED
		MOV SIL, [R13]

PIXEL_5_RED:
		ADD R13, 3h		;[i][j]
		MOV AL, [R13]
		SUB AL, SIL
		JLE PIXEL_6_RED
		MOV SIL, [R13]

PIXEL_6_RED:
		ADD R13, 3h		;[i][j+1]
		MOV AL, [R13]
		SUB AL, SIL
		JLE PIXEL_7_RED
		MOV SIL, [R13]

PIXEL_7_RED:
		SUB R13, 5h		;[i+1][j-1]
		MOV AL, [R12]
		SUB AL, SIL
		JLE PIXEL_8_RED
		MOV SIL, [R12]

PIXEL_8_RED:
		ADD R12, 3h		;[i+1][j]
		MOV AL, [R12]
		SUB AL, SIL
		JLE PIXEL_9_RED
		MOV SIL, [R12]

PIXEL_9_RED:
		ADD R12, 3h		;[i+1][j+1]
		MOV AL, [R12]
		SUB AL, SIL
		JLE END_PIXEL_RED
		MOV SIL, [R12]

END_PIXEL_RED:
		SUB R12, 5h
		MOV [R14], SIL ;write value in bitmap_copy	
		INC R14

		;GREEN
		MOV R15, 0h
		MOV RSI, R15	;inicjalizacja rejestru z maxem

		MOV AL, [R11]; [i-1][j-1] 
		SUB AL, SIL	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_2_GREEN		
		MOV SIL, [R11];nowy MAX

PIXEL_2_GREEN:
		ADD R11, 3h			;[i-1][j]
		MOV AL, [R11]
		SUB AL, SIL
		JLE PIXEL_3_GREEN
		MOV SIL, [R11]

PIXEL_3_GREEN:
		ADD R11, 3h		;[i-1][j+1]
		MOV AL, [R11]
		SUB AL, SIL
		JLE PIXEL_4_GREEN
		MOV SIL, [R11]

PIXEL_4_GREEN:
		SUB R11, 5h		;[i][j-1]
		MOV AL, [R13]
		SUB AL, SIL
		JLE PIXEL_5_GREEN
		MOV SIL, [R13]

PIXEL_5_GREEN:
		ADD R13, 3h		;[i][j]
		MOV AL, [R13]
		SUB AL, SIL
		JLE PIXEL_6_GREEN
		MOV SIL, [R13]

PIXEL_6_GREEN:
		ADD R13, 3h		;[i][j+1]
		MOV AL, [R13]
		SUB AL, SIL
		JLE PIXEL_7_GREEN
		MOV SIL, [R13]

PIXEL_7_GREEN:
		SUB R13, 5h		;[i+1][j-1]
		MOV AL, [R12]
		SUB AL, SIL
		JLE PIXEL_8_GREEN
		MOV SIL, [R12]

PIXEL_8_GREEN:
		ADD R12, 3h		;[i+1][j]
		MOV AL, [R12]
		SUB AL, SIL
		JLE PIXEL_9_GREEN
		MOV SIL, [R12]

PIXEL_9_GREEN:
		ADD R12, 3h		;[i+1][j+1]
		MOV AL, [R12]
		SUB AL, SIL
		JLE END_PIXEL_GREEN
		MOV SIL, [R12]

END_PIXEL_GREEN:
		SUB R12, 5h
		MOV [R14], SIL ;write value in bitmap_copy
		INC R14

		MOV R15, 0h
		MOV RSI, R15	;inicjalizacja rejestru z maxem

		MOV AL, [R11]; [i-1][j-1] 
		SUB AL, SIL	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_2_BLUE		
		MOV SIL, [R11];nowy MAX

PIXEL_2_BLUE:
		ADD R11, 3h			;[i-1][j]
		MOV AL, [R11]
		SUB AL, SIL
		JLE PIXEL_3_BLUE
		MOV SIL, [R11]

PIXEL_3_BLUE:
		ADD R11, 3h		;[i-1][j+1]
		MOV AL, [R11]
		SUB AL, SIL
		JLE PIXEL_4_BLUE
		MOV SIL, [R11]

PIXEL_4_BLUE:
		SUB R11, 5h		;[i][j-1]
		MOV AL, [R13]
		SUB AL, SIL
		JLE PIXEL_5_BLUE
		MOV SIL, [R13]

PIXEL_5_BLUE:
		ADD R13, 3h		;[i][j]
		MOV AL, [R13]
		SUB AL, SIL
		JLE PIXEL_6_BLUE
		MOV SIL, [R13]

PIXEL_6_BLUE:
		ADD R13, 3h		;[i][j+1]
		MOV AL, [R13]
		SUB AL, SIL
		JLE PIXEL_7_BLUE
		MOV SIL, [R13]

PIXEL_7_BLUE:
		SUB R13, 5h		;[i+1][j-1]
		MOV AL, [R12]
		SUB AL, SIL
		JLE PIXEL_8_BLUE
		MOV SIL, [R12]

PIXEL_8_BLUE:
		ADD R12, 3h		;[i+1][j]
		MOV AL, [R12]
		SUB AL, SIL
		JLE PIXEL_9_BLUE
		MOV SIL, [R12]

PIXEL_9_BLUE:
		ADD R12, 3h		;[i+1][j+1]
		MOV AL, [R12]
		SUB AL, SIL
		JLE END_PIXEL_BLUE
		MOV SIL, [R12]

END_PIXEL_BLUE:
		SUB R12, 5h
		MOV [R14], SIL
		INC R14

		MOV RAX, RBX
		DEC RAX
		MOV RBX, RAX
		JNZ PETLA

		RET

		

MaximalFilter endp

end