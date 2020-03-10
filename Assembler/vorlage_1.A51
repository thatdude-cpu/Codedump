; Progammierer...:
; Datum..........:
; Programmnanme..:
; Beschreibung...:                                                                                       
;------------- Programmbereich ----------------------------------89C51CC01-	
		ljmp start	;Sprung zum Prgrammstart
;--------------Interrupt Sprungtabelle ------------------------------------		
		org 0003h
		ljmp ext_int0
		
		org 0013h
		ljmp ext_int1

		org 000bh
		ljmp tim0_int
;*********************Anfang Programm**************************************		
         ORG 0x50

start:		

		mov P1,#11
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
