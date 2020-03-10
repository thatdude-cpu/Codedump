; Progammierer...:
; Datum..........:
; Programmnanme..:
; Beschreibung...:                                                                                       
;------------- Programmbereich ----------------------------------89C51CC01-	
		
		ljmp 0x50	;Sprung zum Prgrammstart

;--------------Interrupt Sprungtabelle ------------------------------------
		org 0x03
		ljmp ext_int0
		
		org 0x13
		ljmp ext_int1

		org 0x0b
		ljmp tim0_int
;*********************Anfang Programm**************************************		
ORG 0x50

mov ADCLK,#5
mov ADCF,#0x80
mov R5,#0x3F
call init_lcd

start:		
;abfrage fertig
	
			wdh:
			mov ADCON,#0x2F
			M1:mov A,ADCON
		 jnb ACC.4,M1
	;LEDS setzen
		mov P1,ADDH
		;neustarten
		
		
		mov A,ADDH
		mov B,#10
		DIV AB
		ADD A,#0x41
		mov R6,A
		call zeichen
		call home
		jb P3.2,wdh
		jnb P3.2,verschieben
		call sek
		;jnb P3.3,leer
		;jb P3.3,wdh
		call sek
		verschieben:
		inc R5
		call pos
		
		;leer:
		;inc R5
		;call pos
		
		
		sjmp wdh
		

		
		
		sjmp $

;*******Ende Programm ****************************************************

;------------ Interruptroutinen ------------------------------------------

ext_int0:	reti

ext_int1:	reti

tim0_int:	reti

;---------------- Textspeicherbereich --------------------------------------
; text:    db      "Hallo ",0
;-------------------------------------------------------------------------- 

$INCLUDE   (lcd_ups_2.INC)
$INCLUDE   (zeit_ups_2.INC)
$INCLUDE   (t89c51cc01.INC) 

;--------------------------------------------------------------------------
		END
