; Progammierer...:	  Seriell COM 8      USB COM 9
; Datum..........: Tannenbaum	
		sjmp start	;Sprung zum Prgrammstart
		org 0x2b
		ljmp tim2_int
start:	setb et2	;**** Programm ab hier**********	
		setb tR0
		call init_lcd
		mov dptr,#text
		call lcdsend
		setb ea
		mov R6,#0
 wdh:	mov dptr,#noten
		mov a,R6
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
		mov r0,#0x60
		mov a,#128
		mov 0x60,A
		mov 0x64,A
		mov 0x68,A
		mov a,#216
		mov 0x61,A
		mov 0x63,A
		mov 0x62,#253
		mov a,#40
		mov 0x65,A
		mov 0x67,A
		mov 0x66,#3
		mov 0x69,#125
		mov 0x6a,#0
t_aus: 	mov dptr,#Zeiten
		mov a,R6
		movc a,@a+dptr
		mov r3,a
		mov 0x6b,A
m_zeit2: 	mov r5,#12
m_zeit:	jnb tf0,$
		clr tf0
		djnz r5,m_zeit		
		djnz r3,m_zeit2
		clr tr2
		mov r7,#120
		call msec
		inc R6
		sjmp wdh		
		sjmp $
tim2_int:	clr tf2	;rechnen  DAU
		mov p1,@r0
		inc r0
		cjne r0,#0x68, m_reti
		mov r0,#0x60
		mov a,0x6b	; Länge der Note  
		jnb p0.7,mm2
		mov a,#1
mm2:	mov b,p0
		mul ab
		mov b,a


		mov a,0x69	 ; Amplitude
		add a,0x6a	; Rest
		div ab
		mov 0x6a,b
		mov r2,a
		mov a,0x69
		subb a,r2
		mov 0x69,a
		add a,acc
		mov b,#7
		div ab
		mov r2,a
		mov a,#128
		add a,0x69
		mov 0x62,a
		mov a,#128
		add a,r2
		mov 0x61,a
		mov 0x63,a
		mov a,#128
		subb a,0x69
		mov 0x66,a
		mov a,#128
		subb a,r2
		mov 0x65,a		
		mov 0x67,a

		//	cpl p1.7
m_reti:			reti
Noten: db 2,5,5,5,6,7,7,7,7,6,7,8,4,6,5,0,9,9,7,10,9,9,8,8,8,8,6,9,8,8,7,7,2,5,5,5,6,7,7,7,7,6,7,8,4,6,5,99
Zeiten:db 4,3,1,6,2,3,1,6,2,2,2,4,4,4,4,2,2,2,2,6, 2,2,2,6,2,2,2,6,2,2,2,4,4,3,1,6,2,3,1,6,2,2,2,4,4,4,4
GDUR:  db 0,0,0x0FE,0x022,0x0FE,0x056,0x0FE,0x085,0x0FE,0x0AE,0x0FE,0x0C1,0x0FE,0x0E4,0x0FF,0x003,0x0FF,0x011,0x0FF,0x02B,0x0FF,0x042,0x0FF,0x057,0x0FF,0x061,0x0FF,0x072
db 0x0F9,0x059,0x0FA,0x013,0x0FA,0x0B9,0x0FB,0x004,0x0FB,0x090,0x0FC,0x00C,0x0FC,0x044,0x0FC,0x0AD,0x0FD,0x00A
text: db"Oh Tannenbaum",0
 $INCLUDE   (lcd_ups_2.INC)
 $INCLUDE   (zeit_ups_2.INC)
$INCLUDE   (t89c51cc01.INC) 
		END
