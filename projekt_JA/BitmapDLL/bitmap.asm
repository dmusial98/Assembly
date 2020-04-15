.data

.code
;ARGUMENTS: RCX - height_from, RDX - height_to, R8 - width_, R9 - bitmap_arrays
;
;R8 <- p�niej rejestr do wysy�ania pojedynczych bajt�w do pami�ci
;R9 <- licznik wierszy
;R10 <- ilo�� 15-bajtowych paczek
;R11 <- wska�nik na wiersz i-1 bitmapy
;R12 <- wska�nik na wiersz i+1 bitmapy
;R13 <- wska�nik na i-ty wiersz bitmapy
;R14 <- wska�nik na obecny wiersz kopii bitmapy
;R15 <- reszta z ostatniej paczki 15-bajtowej do pami�ci
;RBX <- ilo�� 15-bajtowych paczek do wys�ania do pami�ci - licznik
;RSI <- rejestr roboczy, p�niej licznik �adowania pojedynczych bit�w do pami�ci
;RDI <- licznik koloru
;RCX <- wska�nik na wiersz oryginalnej bitmapy
;RDX <- wska�nik na wiersz kopii bitmapy
;XMM0, XMM1, XMM2 <- zawarto�ci bitmapy z wierszy kolejno i-1, i, i+ 1
;XMM3, XMM4, XMM5 <- warto�ci maksymalne z kolumn j-1, j, j+1
;XMM6, XMM7, XMM8 <- warto�ci maksymalne dla koloru z masek 3x3
;XMM8 <- pozniej rejestr do zapisywania do pamieci przetworzonych danych
;XMM13, XMM14, XMM15 <- maski do ustawiania do rejestr�w XMM3, XMM4, XMM5 max�w z pojedynczych kolumn


MaximalFilter proc from_height: DWORD, to_height: DWORD, width_: DWORD, bitmap_arrays: PTR

;rzucenie na stos warto�ci rejestr�w sprzed wywo�ania funkcji
PUSH RCX ;RSP+56
PUSH RDX ;RSP+48
PUSH R8  ;RSP+40
PUSH R9	 ;RSP+32
PUSH R14 ;RSP+24
PUSH RSI ;RSP+16
PUSH RDI ;RSP+8
PUSH RBX ;RSP

;�adowanie masek do rejestr�w XMM
MOV RAX, 8006808003808000h ;mniej znacz�ca po�owa maski
MOVQ XMM15, RAX

MOV RAX, 8080800C80800980h ;bardziej znacz�ca po�owa maski
MOVQ XMM0, RAX
MOVLHPS XMM15, XMM0		   ;w XMM15 pierwsza maska

MOV RAX, 8007808004808001h ;mniej znacz�ca po�owa maski
MOVQ XMM14, RAX
MOV RAX, 8080800D80800A80h ;bardziej znacz�ca po�owa maski
MOVQ XMM0, RAX
MOVLHPS XMM14, XMM0		   ;w XMM14 druga maska

MOV RAX, 8008808005808002h ;mniej znacz�ca po�owa maski
MOVQ XMM13, RAX
MOV RAX, 8080800E80800B80h ;bardziej znacz�ca po�owa maski
MOVQ XMM0, RAX
MOVLHPS XMM13, XMM0        ;w XMM13 trzecia maska

MOV RAX, [RSP + 40] ;RAX <- width_
MOV RSI, 15
CDQ					;wyr�wnanie RDX do 32 bitowego rejestru
DIV ESI
INC RAX
MOV R10, RAX		;R10 <- ilo�� 15-bajtowych paczek
MOV R15, RDX		;R15 <- reszta z ostatniej paczki 15-bajtowej do pami�ci

MOV RDI, 3 ;licznik kolor�w

	PETLA_KOLOR:
	MOV R13, [RSP + 32] ;R13 <- bitmap_arrays
	MOV RCX, [R13]      ;RCX <- wska�nik na wiersz oryginalnej bitampy
	ADD R13, 24			;przesuni�cie na bitmap_copy
	MOV RDX, [R13]		;RDX <- bitmap_copy

	SUB R13, 16
	MOV QWORD ptr [RSP + 32], R13 ;zapis bitmap_arrays do pami�ci

	MOV R8, [RSP + 56]	;R8 <- height_from 
	MOV R9, [RSP + 48]	;R9 <- height_to
	SHL R8, 3			;height_from * 8 -> ilosc bajt�w do przeskoczenia do okre�lonego wiersza na pocz�tku
	SHL R9, 3			;height_to * 8 -> ilo�� bajt�w do przeskoczenia dla wyliczenia adresu ostatniego wiersza

	ADD R9, RCX		;w R9 wska�nik na ostatni wiersz bitmapy do przej�cia
	ADD RCX, R8		;w RCX wska�nik na (height_from) wiersz bitmapy
	ADD RDX, R8		;w RDX wska�nik na (height_from) wiersz kopii bitmapy

	SUB RCX, 8h
	MOV R11, [RCX]	;R11 <- wska�nik na i-1 wiersz bitmapy
	ADD RCX, 10h	
	MOV R12, [RCX]	;R12 <- wska�nik na i+1 wiersz bitmapy
	SUB RCX, 8h	

	MOV R13, [RCX]	;do R13 wska�nik na i-ty wiersz bitmapy

		PETLA_WIERSZ:
		MOV RSI, R15
		MOV RBX, R10			   ;ilo�� paczek 15-bajtowych do wys�ania do pami�ci -> RBX
		MOV R14, [RDX]			   ;do R14 wska�nik na obecny wiersz kopii bitmapy
		ADD R14, 1h				   ;do zapisania piksela o indeksie 1 

			POWROT_Z_PETLI_PACZKA:			
			DEC RBX						;dekrementacja licznika 15-bajtowych paczek
			JL POJEDYNCZE_BAJTY

			VMOVDQU XMM0, XMMWORD ptr[R11]	;za�adowanie i-1 wiersza do XMM
			VMOVDQU XMM1, XMMWORD ptr[R13] ;za�adowanie i wierza do XMM
			VMOVDQU XMM2, XMMWORD ptr[R12] ;za�adowanie i+1 wiersza do XMM

			VPMAXUB XMM0, XMM1, XMM0
			VPMAXUB XMM0, XMM0, XMM2 ; w XMM0 jest max w kolumnie

			VPSHUFB XMM3, XMM0, XMM15
			VPSHUFB XMM4, XMM0, XMM14
			VPSHUFB XMM5, XMM0, XMM13 ;ustawienie warto�ci paczkowanych po 3 w wierszu do wsp�lnej kolumny

			VPMAXUB XMM3, XMM4, XMM3
			VPMAXUB XMM6, XMM3, XMM5 ;w XMM6 pierwsze MAXy dla koloru

			INC R11				 ;przesuni�cie po bitmapie w prawo
			INC R12
			INC R13

			VMOVDQU XMM0, XMMWORD ptr[R11]	;za�adowanie i-1 wiersza do XMM
			VMOVDQU XMM1, XMMWORD ptr[R13] ;za�adowanie i wierza do XMM
			VMOVDQU XMM2, XMMWORD ptr[R12] ;za�adowanie i+1 wiersza do XMM

			VPMAXUB XMM0, XMM1, XMM0
			VPMAXUB XMM0, XMM0, XMM2 ; w XMM0 jest max w kolumnie

			VPSHUFB XMM3, XMM0, XMM15
			VPSHUFB XMM4, XMM0, XMM14
			VPSHUFB XMM5, XMM0, XMM13 ;ustawienie warto�ci paczkowanych po 3 w wierszu do wsp�lnej kolumny

			VPMAXUB XMM3, XMM4, XMM3
			VPMAXUB XMM7, XMM3, XMM5 ;w XMM7 drugie MAXy dla koloru

			INC R11				 ;przesuni�cie po bitmapie w prawo
			INC R12
			INC R13

			VMOVDQU XMM0, XMMWORD ptr[R11]	;za�adowanie i-1 wiersza do XMM
			VMOVDQU XMM1, XMMWORD ptr[R13] ;za�adowanie i wierza do XMM
			VMOVDQU XMM2, XMMWORD ptr[R12] ;za�adowanie i+1 wiersza do XMM

			VPMAXUB XMM0, XMM1, XMM0
			VPMAXUB XMM0, XMM0, XMM2 ; w XMM0 jest max w kolumnie

			VPSHUFB XMM3, XMM0, XMM15
			VPSHUFB XMM4, XMM0, XMM14
			VPSHUFB XMM5, XMM0, XMM13 ;ustawienie warto�ci paczkowanych po 3 w wierszu do wsp�lnej kolumny

			VPMAXUB XMM3, XMM4, XMM3
			VPMAXUB XMM8, XMM3, XMM5 ;w XMM8 trzecie MAXy dla koloru


			VPSLLDQ XMM7, XMM7, 1
			VPSLLDQ XMM8, XMM8, 2 ;bitowe przesuniecie w lewo

			VPOR XMM8, XMM8, XMM7
			VPOR XMM8, XMM8, XMM6 ;w XMM8 paczka do zapisania w pami�ci

			DEC R10
			JL POJEDYNCZE_BAJTY
			INC R10

			ADD R11, 13			  ;przesuni�cie po oryginalnej bitmapie w prawo do wczytywania kolejnych pikseli
			ADD R12, 13
			ADD R13, 13

			VMOVDQU XMMWORD ptr[R14], XMM8 ;za�adowanie do pami�ci 

			ADD R14, 15 ;przesuni�cie sie wska�nikiem na wiersz w kopii bitmapy do przodu
			JMP POWROT_Z_PETLI_PACZKA 

			POJEDYNCZE_BAJTY:  ;gdy ju� nie �adujemy 15-bajtowych paczek do pami�ci

			;kod dla wys�ania pojedynczych bajt�w do pami�ci
			SUB RSI, 8
			JLE MNIEJ_NIZ_9_PRZYGOTUJ
			MOVQ RAX, XMM8
			MOV QWORD ptr[R14], RAX ;za�adowanie do pami�ci pierwszych 8 bajt�w 
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

		;czynno�ci przed przej�ciem do nowego wiersza
		PRZYGOTOWANIE_DO_NOWEGO_WIERSZA:
		MOV R11, [RCX] ;R11 <- wiersz o indeksie i-1
		ADD RCX, 10h   ;przesuni�cie o dwa wiersze ni�ej
		MOV R12, [RCX] ;R12 <- wiersz o indeksie i+1
		SUB RCX, 8h	   
		MOV R13, [RCX] ;R13 <- wiersz o indeksie i
		ADD RDX, 8h	   ;RDX <- adres wiersza do zapisywania dla kopii bitmpapy

		MOV RAX, R9 ;w RAX wska�nik na ostatni wiersz bitmapy
		SUB RAX, RCX

		JG	PETLA_WIERSZ

	DEC RDI
	JG PETLA_KOLOR

	SUB QWORD ptr [RSP + 32], 24

;za�adowanie ze stosu warto�ci rejestr�w sprzed wywo�ania funkcji
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