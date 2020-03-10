#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>

int main (void)
{
	
	int summe=0,
		wert,zaeler;

	printf("werte eingeben\n");


		
		for(zaeler=0;zaeler < 5;zaeler++)
		{
		scanf("%i",&wert);
		printf("Wert : %i\n",wert);
		summe=wert+summe;
		}

	printf("Summe :%i",summe);


	system ("pause");
	return 0;
}