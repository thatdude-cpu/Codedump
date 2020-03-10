; Progammierer...:
; Datum..........:
; Programmnanme..:
; Beschreibung...:                                                                                       
;------------- Programmbereich ----------------------------------89C51CC01-	
		
	
wdh:

	mov P1,#0x80
	mov R7,#3
	call sec  
	mov P1,#0x40
	mov R7,#1
	call sec 
	mov P1,#0x20
	mov R7,#3
	call sec 
	mov P1,#0x40
	mov R7,#1
	call sec 

sjmp wdh  
;*******Ende Programm ****************************************************

;------------ Interruptroutinen ------------------------------------------

;---------------- Textspeicherbereich --------------------------------------

;-------------------------------------------------------------------------- 

$INCLUDE   (lcd_ups_2.INC)
$INCLUDE   (zeit_ups_2.INC)
$INCLUDE   (t89c51cc01.INC) 

;--------------------------------------------------------------------------
		END
