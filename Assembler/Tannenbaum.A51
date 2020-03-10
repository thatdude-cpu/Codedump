; Progammierer...:	  Seriell COM 8      USB COM 9
; Datum..........: Tannenbaum	

		sjmp start	;Sprung zum Prgrammstart
		org 0x2b
		ljmp tim2_int
start:	setb et2	;	  ****** Programm ab hier*************	
		setb tr0
		setb ea
		mov r0,#0
 wdh:	mov dptr,#noten
		mov a,r0
		movc a,@a+dptr
		mov r1,a
		cjne r1,#99,weiter
		sjmp $
 weiter:mov dptr,#GDUR
		mov a,r1
		add a,r1
		movc a,@a+dptr
		mov rcap2h,a
  		mov a,r1
		add a,r1
		inc a
		movc a,@a+dptr
		mov rcap2l,a
		mov a,r1
		jnz t_an
		clr tr2
		sjmp t_aus
t_an:	setb tr2
t_aus: 	mov dptr,#Zeiten
		mov a,r0
		movc a,@a+dptr
		mov r3,a
m_zeit2: mov a,p0
		anl a,#0xf
		inc a
		mov r5,a
m_zeit:	jnb tf0,$
		clr tf0
		djnz r5,m_zeit		
		djnz r3,m_zeit2
		clr tr2
		mov r7,p0
		call msec
		inc r0
		sjmp wdh		
		sjmp $
tim2_int:	clr tf2
			cpl p1.7
			reti
Noten: db 2,5,5,5,6,7,7,7,7,6,7,8,4,6,5,0,9,9,7,10,9,9,8,8,8,8,6,9,8,8,7,7,2,5,5,5,6,7,7,7,7,6,7,8,4,6,5,2,5,5,5,6,7,7,7,7,6,7,8,4,6,5,0,9,9,7,10,9,9,8,8,8,8,6,9,8,8,7,7,2,5,5,5,6,7,7,7,7,6,7,8,4,6,5,99
Zeiten:db 4,3,1,6,2,3,1,6,2,2,2,4,4,4,4,2,2,2,2,6, 2,2,2,6,2,2,2,6,2,2,2,4,4,3,1,6,2,3,1,6,2,2,2,4,4,4,4,4,3,1,6,2,3,1,6,2,2,2,4,4,4,4,2,2,2,2,6, 2,2,2,6,2,2,2,6,2,2,2,4,4,3,1,6,2,3,1,6,2,2,2,4,4,4,4
GDUR:  db 0,0,0x0F8,0x089,0x0F9,0x059,0x0FA,0x013,0x0FA,0x0B9,0x0FB,0x004,0x0FB,0x090,0x0FC,0x00C,0x0FC,0x044,0x0FC,0x0AD,0x0FD,0x00A
 //$INCLUDE   (lcd_ups_2.INC)
 $INCLUDE   (zeit_ups_2.INC)
$INCLUDE   (t89c51cc01.INC) 
		END