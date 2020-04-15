.data

.code
;ARGUMENTS: RCX - height_from, RDX - height_to, R8 - width_, R9 - bitmap_arrays
;
;R8 <- póŸniej rejestr do wysy³ania pojedynczych bajtów do pamiêci
;R9 <- licznik wierszy
;R10 <- iloœæ 15-bajtowych paczek
;R11 <- wskaŸnik na wiersz i-1 bitmapy
;R12 <- wskaŸnik na wiersz i+1 bitmapy
;R13 <- wskaŸnik na i-ty wiersz bitmapy
;R14 <- wskaŸnik na obecny wiersz kopii bitmapy
;R15 <- reszta z ostatniej paczki 15-bajtowej do pamiêci
;RBX <- iloœæ 15-bajtowych paczek do wys³ania do pamiêci - licznik
;RSI <- rejestr roboczy, póŸniej licznik ³adowania pojedynczych bitów do pamiêci
;RDI <- licznik koloru
;RCX <- wskaŸnik na wiersz oryginalnej bitmapy
;RDX <- wskaŸnik na wiersz kopii bitmapy
;XMM0, XMM1, XMM2 <- zawartoœci bitmapy z wierszy kolejno i-1, i, i+ 1
;XMM3, XMM4, XMM5 <- wartoœci maksymalne z kolumn j-1, j, j+1
;XMM6, XMM7, XMM8 <- wartoœci maksymalne dla koloru z masek 3x3
;XMM8 <- pozniej rejestr do zapisywania do pamieci przetworzonych danych
;XMM13, XMM14, XMM15 <- maski do ustawiania do rejestrów XMM3, XMM4, XMM5 maxów z pojedynczych kolumn


MaximalFilter proc from_height: DWORD, to_height: DWORD, width_: DWORD, bitmap_arrays: PTR

;rzucenie na stos wartoœci rejestrów sprzed wywo³ania funkcji
PUSH RCX ;RSP+56
PUSH RDX ;RSP+48
PUSH R8  ;RSP+40
PUSH R9	 ;RSP+32
PUSH R14 ;RSP+24
PUSH RSI ;RSP+16
PUSH RDI ;RSP+8
PUSH RBX ;RSP

;³adowanie masek do rejestrów XMM
MOV RAX, 8006808003808000h ;mniej znacz¹ca po³owa maski
MOVQ XMM15, RAX

MOV RAX, 8080800C80800980h ;bardziej znacz¹ca po³owa maski
MOVQ XMM0, RAX
MOVLHPS XMM15, XMM0		   ;w XMM15 pierwsza maska

MOV RAX, 8007808004808001h ;mniej znacz¹ca po³owa maski
MOVQ XMM14, RAX
MOV RAX, 8080800D80800A80h ;bardziej znacz¹ca po³owa maski
MOVQ XMM0, RAX
MOVLHPS XMM14, XMM0		   ;w XMM14 druga maska

MOV RAX, 8008808005808002h ;mniej znacz¹ca po³owa maski
MOVQ XMM13, RAX
MOV RAX, 8080800E80800B80h ;bardziej znacz¹ca po³owa maski
MOVQ XMM0, RAX
MOVLHPS XMM13, XMM0        ;w XMM13 trzecia maska

MOV RAX, [RSP + 40] ;RAX <- width_
MOV RSI, 15
CDQ					;wyrównanie RDX do 32 bitowego rejestru
DIV ESI
INC RAX
MOV R10, RAX		;R10 <- iloœæ 15-bajtowych paczek
MOV R15, RDX		;R15 <- reszta z ostatniej paczki 15-bajtowej do pamiêci

MOV RDI, 3 ;licznik kolorów

	PETLA_KOLOR:
	MOV R13, [RSP + 32] ;R13 <- bitmap_arrays
	MOV RCX, [R13]      ;RCX <- wskaŸnik na wiersz oryginalnej bitampy
	ADD R13, 24			;przesuniêcie na bitmap_copy
	MOV RDX, [R13]		;RDX <- bitmap_copy

	SUB R13, 16
	MOV QWORD ptr [RSP + 32], R13 ;zapis bitmap_arrays do pamiêci

	MOV R8, [RSP + 56]	;R8 <- height_from 
	MOV R9, [RSP + 48]	;R9 <- height_to
	SHL R8, 3			;height_from * 8 -> ilosc bajtów do przeskoczenia do okreœlonego wiersza na pocz¹tku
	SHL R9, 3			;height_to * 8 -> iloœæ bajtów do przeskoczenia dla wyliczenia adresu ostatniego wiersza

	ADD R9, RCX		;w R9 wskaŸnik na ostatni wiersz bitmapy do przejœcia
	ADD RCX, R8		;w RCX wskaŸnik na (height_from) wiersz bitmapy
	ADD RDX, R8		;w RDX wskaŸnik na (height_from) wiersz kopii bitmapy

	SUB RCX, 8h
	MOV R11, [RCX]	;R11 <- wskaŸnik na i-1 wiersz bitmapy
	ADD RCX, 10h	
	MOV R12, [RCX]	;R12 <- wskaŸnik na i+1 wiersz bitmapy
	SUB RCX, 8h	

	MOV R13, [RCX]	;do R13 wskaŸnik na i-ty wiersz bitmapy

		PETLA_WIERSZ:
		MOV RSI, R15
		MOV RBX, R10			   ;iloœæ paczek 15-bajtowych do wys³ania do pamiêci -> RBX
		MOV R14, [RDX]			   ;do R14 wskaŸnik na obecny wiersz kopii bitmapy
		ADD R14, 1h				   ;do zapisania piksela o indeksie 1 

			POWROT_Z_PETLI_PACZKA:			
			DEC RBX						;dekrementacja licznika 15-bajtowych paczek
			JL POJEDYNCZE_BAJTY

			VMOVDQU XMM0, XMMWORD ptr[R11]	;za³adowanie i-1 wiersza do XMM
			VMOVDQU XMM1, XMMWORD ptr[R13] ;za³adowanie i wierza do XMM
			VMOVDQU XMM2, XMMWORD ptr[R12] ;za³adowanie i+1 wiersza do XMM

			VPMAXUB XMM0, XMM1, XMM0
			VPMAXUB XMM0, XMM0, XMM2 ; w XMM0 jest max w kolumnie

			VPSHUFB XMM3, XMM0, XMM15
			VPSHUFB XMM4, XMM0, XMM14
			VPSHUFB XMM5, XMM0, XMM13 ;ustawienie wartoœci paczkowanych po 3 w wierszu do wspólnej kolumny

			VPMAXUB XMM3, XMM4, XMM3
			VPMAXUB XMM6, XMM3, XMM5 ;w XMM6 pierwsze MAXy dla koloru

			INC R11				 ;przesuniêcie po bitmapie w prawo
			INC R12
			INC R13

			VMOVDQU XMM0, XMMWORD ptr[R11]	;za³adowanie i-1 wiersza do XMM
			VMOVDQU XMM1, XMMWORD ptr[R13] ;za³adowanie i wierza do XMM
			VMOVDQU XMM2, XMMWORD ptr[R12] ;za³adowanie i+1 wiersza do XMM

			VPMAXUB XMM0, XMM1, XMM0
			VPMAXUB XMM0, XMM0, XMM2 ; w XMM0 jest max w kolumnie

			VPSHUFB XMM3, XMM0, XMM15
			VPSHUFB XMM4, XMM0, XMM14
			VPSHUFB XMM5, XMM0, XMM13 ;ustawienie wartoœci paczkowanych po 3 w wierszu do wspólnej kolumny

			VPMAXUB XMM3, XMM4, XMM3
			VPMAXUB XMM7, XMM3, XMM5 ;w XMM7 drugie MAXy dla koloru

			INC R11				 ;przesuniêcie po bitmapie w prawo
			INC R12
			INC R13

			VMOVDQU XMM0, XMMWORD ptr[R11]	;za³adowanie i-1 wiersza do XMM
			VMOVDQU XMM1, XMMWORD ptr[R13] ;za³adowanie i wierza do XMM
			VMOVDQU XMM2, XMMWORD ptr[R12] ;za³adowanie i+1 wiersza do XMM

			VPMAXUB XMM0, XMM1, XMM0
			VPMAXUB XMM0, XMM0, XMM2 ; w XMM0 jest max w kolumnie

			VPSHUFB XMM3, XMM0, XMM15
			VPSHUFB XMM4, XMM0, XMM14
			VPSHUFB XMM5, XMM0, XMM13 ;ustawienie wartoœci paczkowanych po 3 w wierszu do wspólnej kolumny

			VPMAXUB XMM3, XMM4, XMM3
			VPMAXUB XMM8, XMM3, XMM5 ;w XMM8 trzecie MAXy dla koloru


			VPSLLDQ XMM7, XMM7, 1
			VPSLLDQ XMM8, XMM8, 2 ;bitowe przesuniecie w lewo

			VPOR XMM8, XMM8, XMM7
			VPOR XMM8, XMM8, XMM6 ;w XMM8 paczka do zapisania w pamiêci

			DEC R10
			JL POJEDYNCZE_BAJTY
			INC R10

			ADD R11, 13			  ;przesuniêcie po oryginalnej bitmapie w prawo do wczytywania kolejnych pikseli
			ADD R12, 13
			ADD R13, 13

			VMOVDQU XMMWORD ptr[R14], XMM8 ;za³adowanie do pamiêci 

			ADD R14, 15 ;przesuniêcie sie wskaŸnikiem na wiersz w kopii bitmapy do przodu
			JMP POWROT_Z_PETLI_PACZKA 

			POJEDYNCZE_BAJTY:  ;gdy ju¿ nie ³adujemy 15-bajtowych paczek do pamiêci

			;kod dla wys³ania pojedynczych bajtów do pamiêci
			SUB RSI, 8
			JLE MNIEJ_NIZ_9_PRZYGOTUJ
			MOVQ RAX, XMM8
			MOV QWORD ptr[R14], RAX ;za³adowanie do pamiêci pierwszych 8 bajtów 
			ADD R14, 8
			JMP RESZTA_POWYZEJ_8

			MNIEJ_NIZ_9_PRZYGOTUJ:
			ADD RSI, 8
			MOVQ R8, XMM8
			SUB RSI, 0
			JZ PRZYGOTOWANIE_DO_NOWEGO_WIERSZA

			PETLA_MNIEJ_NIZ_9:
			MOV BYTE ptr[R14], R8B ;1 dodatkowy bajt
			INC R14
			DEC RSI
			JZ PRZYGOTOWANIE_DO_NOWEGO_WIERSZA
			SHR R8, 8
			MOV BYTE ptr[R14], R8B ;2 dodatkowy bajt
			INC R14
			DEC RSI
			JZ PRZYGOTOWANIE_DO_NOWEGO_WIERSZA
			SHR R8, 8
			MOV BYTE ptr[R14], R8B ;3 dodatkowy bajt
			INC R14
			DEC RSI
			JZ PRZYGOTOWANIE_DO_NOWEGO_WIERSZA
			SHR R8, 8
			MOV BYTE ptr[R14], R8B ;4 dodatkowy bajt
			INC R14
			DEC RSI
			JZ PRZYGOTOWANIE_DO_NOWEGO_WIERSZA
			SHR R8, 8
			MOV BYTE ptr[R14], R8B ;5 dodatkowy bajt
			INC R14
			DEC RSI
			JZ PRZYGOTOWANIE_DO_NOWEGO_WIERSZA
			SHR R8, 8
			MOV BYTE ptr[R14], R8B ;6 dodatkowy bajt
			INC R14
			DEC RSI
			JZ PRZYGOTOWANIE_DO_NOWEGO_WIERSZA
			SHR R8, 8
			MOV BYTE ptr[R14], R8B ;7 dodatkowy bajt
			INC R14
			DEC RSI
			JZ PRZYGOTOWANIE_DO_NOWEGO_WIERSZA
			SHR R8, 8
			MOV BYTE ptr[R14], R8B ;8 dodatkowy bajt
			INC R14
			DEC RSI
			JMP PRZYGOTOWANIE_DO_NOWEGO_WIERSZA

			RESZTA_POWYZEJ_8:
			MOVHLPS XMM8, XMM8
			MOVQ R8, XMM8
			JMP PETLA_MNIEJ_NIZ_9

		;czynnoœci przed przejœciem do nowego wiersza
		PRZYGOTOWANIE_DO_NOWEGO_WIERSZA:
		MOV R11, [RCX] ;R11 <- wiersz o indeksie i-1
		ADD RCX, 10h   ;przesuniêcie o dwa wiersze ni¿ej
		MOV R12, [RCX] ;R12 <- wiersz o indeksie i+1
		SUB RCX, 8h	   
		MOV R13, [RCX] ;R13 <- wiersz o indeksie i
		ADD RDX, 8h	   ;RDX <- adres wiersza do zapisywania dla kopii bitmpapy

		MOV RAX, R9 ;w RAX wskaŸnik na ostatni wiersz bitmapy
		SUB RAX, RCX

		JG	PETLA_WIERSZ

	DEC RDI
	JG PETLA_KOLOR

	SUB QWORD ptr [RSP + 32], 24

;za³adowanie ze stosu wartoœci rejestrów sprzed wywo³ania funkcji
POP RBX
POP RDI
POP RSI
POP R14
POP R9
POP R8
POP RDX
POP RCX

RET 

MaximalFilter endp


;-------------------------------------------------------------------------

end