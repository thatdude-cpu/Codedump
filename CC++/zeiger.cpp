#pragma hdrstop
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>


void main () 

	 
{
	  char zeichen, *zeiger;
	
	  
	printf("Bitte ein char zeichen eingeben\n");
	fflush(stdin);
	scanf("%c",&zeichen);
	fflush(stdin);
	printf("Die speicherstelle vom typ char enthaelt nun den Buchstaben: %c\n",zeichen);
	getch();
	fflush(stdin);
	printf("Die Adresse von zeichen heisst wie folgt %p\n",&zeichen);
	printf("Die adresse von zeichen wird nun auf den zeiger kopiert\n");
	zeiger=&zeichen;
	getch();
	fflush(stdin);
	printf("Der zeiger enthält nun folgende adresse %p\n",zeiger);
	getch();
	fflush(stdin);
	printf("jetzt wird ueber den zeiger ein char zeichen in die \n");
	printf("Speicherstelle zeichen folgender wert hineingeschrieben \n");
	printf("bitte ein char zeichen eingeben\n");
	scanf("%c",zeiger);
	fflush(stdin);
	printf("Die zeichen speicherstelle enthaelt nun folgenden buchstaben %c \n",zeichen);
	printf("%p",&zeiger);
	getch();
}