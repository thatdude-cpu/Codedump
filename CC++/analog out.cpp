#include <time.h>
#include <conio.h>
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
//#include "ljackuw.h"
int main (void)
{
 //long A;
 int Spg;

 for (Spg = 0; Spg <=5; Spg++)

  {
	printf ("Spannung %i Volt\n",Spg);

   // EAnalogOut(&A,0,Spg,0)
	Sleep(1000);
  }
 
 for (Spg = 4; Spg >=0; Spg--)

  {
	printf ("Spannung %i Volt\n",Spg);

   // EAnalogOut(&A,0,Spg,0)
	Sleep(1000);
  }


  system("Pause");
  return 0;
 }