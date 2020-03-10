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

start:		

		mov P1,#0x55
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
