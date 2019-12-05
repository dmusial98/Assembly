.data

.code

;ARGUMENTS: RCX - bitmap, RDX - bitmap_copy, R8 - height_from, R9 - height_to, RAX - width -> R10
;
;VARIABLES:
;
;R11 <- wskaünik na wiersz i-1 bitmapy
;R12 <- wskaünik na wiersz i+1 bitmapy
;R13 <- wskaünik na i-ty wiersz bitmapy
;
;R14 <- wskaünik na obecny wiersz kopii bitmapy
;R15 <- roboczy
;
;RBX <- licznik kolumny w wierszu
;RSI <- obecny max do porÛwnywania

MaximalFilter proc bitmap: PTR, bitmap_copy: PTR, from_height: DWORD, to_height: DWORD, wid: DWORD

SUB RAX, 2h
MOV R10, RAX	; width -> R10

MOV R15, 0h
MOV RSI, R15	;inicjalizacja rejestru z maxem

MOV R15, 8h		;do wymnoøenia przez 8
IMUL R8, R15	;height_from * 8 -> ilosc bajtÛw do przeskoczenia do okreúlonego wiersza na poczπtku programu
IMUL R9, R15	;height_to * 8 -> iloúÊ bajtÛw do przeskoczenia dla wyliczenia adresu ostatniego wiersza

ADD R9, RCX		;w R9 wskaünik na ostatni wiersz bitmapy do przejúcia
ADD RCX, R8		;w RCX wskaünik na (height_from) wiersz bitmapy
ADD RDX, R8		;w RDX wskaünik na (height_from) wiersz kopii bitmapy

SUB RCX, 8h
MOV R11, [RCX]	;R11 <- wskaünik na i-1 wiersz bitmapy
ADD RCX, 10h	
MOV R12, [RCX]	;R12 <- wskaünik na i+1 wiersz bitmapy
SUB RCX, 8h	

MOV R13, [RCX]	;do R13 wskaünik na i-ty wiersz bitmapy

PETLA_WIERSZ:
		MOV RBX, R10
		MOV R14, [RDX]	;do R14 wskaünik na obecny wiersz kopii bitmapy
		ADD R14, 3h		;do zapisania piksela o indeksie 1 

		;RED
PETLA_KOLUMNA:	
		MOV RAX, 0h
		MOV R15, 0h
		MOV RSI, R15	;inicjalizacja rejestru z maxem
						;poprawic to przerzucanie zera do R15, bo niepotrzebne!!!!!!!!!!!!!!!!!!!!!
		XOR RAX, RAX
		MOV AL, [R11] ;[i-1][j-1] 
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_2_RED		
		MOV SIL, [R11];nowy MAX

PIXEL_2_RED:
		ADD R11, 3h		;[i-1][j]
		XOR RAX, RAX
		MOV AL, [R11]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_3_RED
		MOV SIL, [R11]

PIXEL_3_RED:
		ADD R11, 3h		;[i-1][j+1]
		XOR RAX, RAX
		MOV AL, [R11]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_4_RED
		MOV SIL, [R11]

PIXEL_4_RED:
		SUB R11, 5h		;[i][j-1]
		XOR RAX, RAX
		MOV AL, [R13]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_5_RED
		MOV SIL, [R13]

PIXEL_5_RED:
		ADD R13, 3h		;[i][j]
		XOR RAX, RAX
		MOV AL, [R13]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_6_RED
		MOV SIL, [R13]

PIXEL_6_RED:
		ADD R13, 3h		;[i][j+1]
		XOR RAX, RAX
		MOV AL, [R13]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_7_RED
		MOV SIL, [R13]

PIXEL_7_RED:
		SUB R13, 5h		;[i+1][j-1]
		XOR RAX, RAX
		MOV AL, [R12]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_8_RED
		MOV SIL, [R12]

PIXEL_8_RED:
		ADD R12, 3h		;[i+1][j]
		XOR RAX, RAX
		MOV AL, [R12]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_9_RED
		MOV SIL, [R12]

PIXEL_9_RED:
		ADD R12, 3h		;[i+1][j+1]
		XOR RAX, RAX
		MOV AL, [R12]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
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
		XOR RAX, RAX
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_2_GREEN		
		MOV SIL, [R11];nowy MAX

PIXEL_2_GREEN:
		ADD R11, 3h			;[i-1][j]
		XOR RAX, RAX
		MOV AL, [R11]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_3_GREEN
		MOV SIL, [R11]

PIXEL_3_GREEN:
		ADD R11, 3h		;[i-1][j+1]
		XOR RAX, RAX
		MOV AL, [R11]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_4_GREEN
		MOV SIL, [R11]

PIXEL_4_GREEN:
		SUB R11, 5h		;[i][j-1]
		XOR RAX, RAX
		MOV AL, [R13]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_5_GREEN
		MOV SIL, [R13]

PIXEL_5_GREEN:
		ADD R13, 3h		;[i][j]
		XOR RAX, RAX
		MOV AL, [R13]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_6_GREEN
		MOV SIL, [R13]

PIXEL_6_GREEN:
		ADD R13, 3h		;[i][j+1]
		XOR RAX, RAX
		MOV AL, [R13]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_7_GREEN
		MOV SIL, [R13]

PIXEL_7_GREEN:
		SUB R13, 5h		;[i+1][j-1]
		XOR RAX, RAX
		MOV AL, [R12]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_8_GREEN
		MOV SIL, [R12]

PIXEL_8_GREEN:
		ADD R12, 3h		;[i+1][j]
		XOR RAX, RAX
		MOV AL, [R12]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_9_GREEN
		MOV SIL, [R12]

PIXEL_9_GREEN:
		ADD R12, 3h		;[i+1][j+1]
		XOR RAX, RAX
		MOV AL, [R12]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE END_PIXEL_GREEN
		MOV SIL, [R12]

END_PIXEL_GREEN:
		SUB R12, 5h
		MOV [R14], SIL ;write value in bitmap_copy
		INC R14

		;BLUE
		MOV R15, 0h
		MOV RSI, R15	;inicjalizacja rejestru z maxem

		XOR RAX, RAX
		MOV AL, [R11]; [i-1][j-1] 
		SUB AL, SIL	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_2_BLUE		
		MOV SIL, [R11];nowy MAX

PIXEL_2_BLUE:
		ADD R11, 3h			;[i-1][j]
		XOR RAX, RAX
		MOV AL, [R11]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_3_BLUE
		MOV SIL, [R11]

PIXEL_3_BLUE:
		ADD R11, 3h		;[i-1][j+1]
		XOR RAX, RAX
		MOV AL, [R11]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_4_BLUE
		MOV SIL, [R11]

PIXEL_4_BLUE:
		SUB R11, 5h		;[i][j-1]
		XOR RAX, RAX
		MOV AL, [R13]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_5_BLUE
		MOV SIL, [R13]

PIXEL_5_BLUE:
		ADD R13, 3h		;[i][j]
		XOR RAX, RAX
		MOV AL, [R13]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_6_BLUE
		MOV SIL, [R13]

PIXEL_6_BLUE:
		ADD R13, 3h		;[i][j+1]
		XOR RAX, RAX
		MOV AL, [R13]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_7_BLUE
		MOV SIL, [R13]

PIXEL_7_BLUE:
		SUB R13, 5h		;[i+1][j-1]
		XOR RAX, RAX
		MOV AL, [R12]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_8_BLUE
		MOV SIL, [R12]

PIXEL_8_BLUE:
		ADD R12, 3h		;[i+1][j]
		XOR RAX, RAX
		MOV AL, [R12]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE PIXEL_9_BLUE
		MOV SIL, [R12]

PIXEL_9_BLUE:
		ADD R12, 3h		;[i+1][j+1]
		XOR RAX, RAX
		MOV AL, [R12]
		SUB AX, SI	  ;sprawdzanie czy mamy nowego MAXa
		JLE END_PIXEL_BLUE
		MOV SIL, [R12]

END_PIXEL_BLUE:
		SUB R12, 5h
		MOV [R14], SIL
		INC R14

		MOV RAX, RBX
		DEC RAX
		MOV RBX, RAX
		JNZ PETLA_KOLUMNA

		MOV R11, [RCX] ;R11 <- wiersz o indeksie i-1
		ADD RCX, 10h   ;przesuniÍcie o dwa wiersze niøej
		MOV R12, [RCX] ;R12 <- wiersz o indeksie i+1
		SUB RCX, 8h	   
		MOV R13, [RCX] ;R13 <- wiersz o indeksie i
		ADD RDX, 8h	   ;RDX <- adres wiersza do zapisywania dla kopii bitmpapy

		MOV RAX, R9 ;w RAX wskaünik na ostatni wiersz bitmapy
		SUB RAX, RCX
		JG	PETLA_WIERSZ


		RET

MaximalFilter endp

end