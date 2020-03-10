# include <math.h>
# include <conio.h>
# include <stdio.h>
# include <stdlib.h>
# include <Windows.h>
int main ()



{
	

	unsigned int zaeler=0,erg,mask=0x80000000,Zahl;

	
	printf("Bitte geben sie ihre zahl ein\n");
	scanf("%d",&Zahl);
	
	

	do
	{


	if(mask&Zahl)
	 {
		printf("1");
		 
	 }
	
	  else
	  {
	   printf("0");
	  }
		zaeler++; 	
		mask=mask>>1;
	} while (zaeler<32);

	system("pause");
	return 0;
}