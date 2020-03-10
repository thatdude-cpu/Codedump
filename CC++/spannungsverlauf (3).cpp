#include <time.h>
#include <conio.h>
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
//#include "ljackuw.h"


void gotoxy (short x, short y)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X=x-1;
	pos.Y=y-1;
	SetConsoleCursorPosition(hcon , pos);
}
 int main (void)
{





 long A;
 int Spg,Zeit=0;


 printf("Spannungsverlauf\n");
 printf(" U\n");
 printf("[V]|\n");
 printf("  5|\n");
 printf("   |\n");
 printf("  4|\n");
 printf("   |\n");
 printf("  3|\n");
 printf("   |\n");
 printf("  2|\n");
 printf("   |\n");
 printf("  1|\n");
 printf("   |\n");
 printf("   |\n");
 printf("   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
 printf("   0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30\n");
 
 for (Spg = 0; Spg <=5; Spg++)

  {

	
	gotoxy(5+Zeit*2,14-Spg);
    	printf ("*");
	
	//EAnalogOut(&A,0,Spg,0);
	Sleep(1000);
   Zeit++; 
 }
 
 for (Spg = 4; Spg >=0; Spg--)

  {
	  gotoxy(5+Zeit*2,14-Spg);
	  printf ("*");

   //EAnalogOut(&A,0,Spg,0);
	Sleep(1000);
    Zeit++ ; 
 } 


  system("Pause");
  return 0;
 }