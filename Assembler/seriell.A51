;--------------------------------------------------------------------------
; Progammierer.: ae                          
; Datum........: 4.5.06          
; Progammname..: seriell.a51	 
; Beschreibung.: Testprogramm für die serielle Schnittstelle 
;		 >> Auf  dem PC Hyperterminal starten!!                         
;                                                            
;---------------------------------------------------------------89C51CC01 -
;__________________________________________________________________________

 

;------------- Programmbereich --------------------------------------------
		org  0000h
		ljmp 100h	;Sprung zum Prgrammstart

;--------------Interrupt Sprungtabelle ------------------------------------
		

		org 0003h
		ljmp ext_int0
		
		org 0013h
		ljmp ext_int1

		org 000bh
		ljmp tim0_int

		org 23h
		ljmp seriell_int


;****** Anfang Programm ****************************************************
		

                ORG 100H

start:		mov p1,#1	;Testanzeige an Port 1 
		mov r2,#30h

		setb et1
		setb ea
		setb es
		setb ex0
		setb it0
		clr ti
		

		mov scon,#50h	;Mode 1 und REN setzen
		mov tmod,#20h	;Timer 1 zur Baudratenerzeugung
		mov pcon,#80h	;doppelte Baudrate  >> 4800 Baud
		mov th1, #0f3h	;Reload für seriell timing fosz/12/16/13 > 4800 Baud
		setb tr1

		call init	;LCD Anzeige
		call display
		call clear
		call dis_on1
		
		mov dptr, #text1 
		call lcdsend
		mov r0,#40h
		call pos
		mov dptr,#text2	;gelb entspricht Sendetaste
		call lcdsend
		sjmp $		;Stop


;*******Ende Programm ****************************************************

;------------ Interruptroutinen ------------------------------------------

ext_int0:	
		inc r2		;Zeichen das gesendet wird
		call clear
		mov dptr,#text3	;Anzeige der Übertragungsdaten
		call lcdsend
		mov r0,#40h
		call pos
		mov r0,02h
		call zeichen	;Zeichen das gesendet wird		
		mov p1,r2
		mov sbuf,r2
		clr ea
		jnb ti,$
		clr ti
		setb ea
		reti


ext_int1:	reti


tim0_int:	reti

seriell_int:
 		clr ri
		mov r0,#40h
		lcall pos
		mov r0,sbuf
		call zeichen	;Zeichen das empfangen wurde
		reti
;-------------------------------------------------------------------------- 	


;---------------- Textspeicherbereich --------------------------------------

 text1:    db      "Serielle Datenuebertragung",0
 text2:    db      "Weiter mit gelb",0
 text3:    db      "4800 Baud N 1",0


;----------Breich für Unterprogramme --------------------------------------

$INCLUDE   (lcd_ups_2.INC)
$INCLUDE   (zeit_ups_2.INC)
$INCLUDE   (t89c51cc01.INC) 

;--------------------------------------------------------------------------
		END
