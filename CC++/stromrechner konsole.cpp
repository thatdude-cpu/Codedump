#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <Windows.h>

//Marius Weinhardt	22.9.2014
//--------------------------------------------------
void gotoxy (short x, short y)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X=x-1;
	pos.Y=y-1;
	SetConsoleCursorPosition(hcon , pos);
}
	int main(void)
{
 double U,I,R,P;
 printf("********************************************************************************\n");
 printf("					U,I--> R,P\n");
 printf("********************************************************************************\n\n");

 printf("Spannung :			Volt");		//Eingabe
 gotoxy(35,8);
 scanf ("%lf",&U);
 gotoxy(35,8);
 printf("%6.2lf\n", U);
 printf("Strom:					Ampere");
 gotoxy(35,9);
 scanf("%lf",&I);
 gotoxy(35,9);
 printf("%6.2lf\n", I);
 R=U/I;							
 P=U*I;
 printf("Wiederstand:		Ohm");	
 gotoxy(35,10);
 printf("%6.2lf\n", R);
 printf("Leistung:			Watt");
 gotoxy(35,10);
 printf("%6.2lf\n", I);
 system("Pause");
 return 0;
}