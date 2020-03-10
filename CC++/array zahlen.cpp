# include <stdlib.h>
# include <stdio.h>
# include <windows.h>

//Marius Weinhardt
int main (void)
{
	
	int zähler=0,z=0;
	char Zeichen[30];

		
	
		
		printf (" Zeichen eingeben " );
	    scanf("%s",&Zeichen);
		 
		
		
		while(Zeichen[zähler])
	    {
		
		 zähler++;
		
	    }
	    
		
		printf("Anzahl der Zeichen: %i",zähler);

		
	 system("Pause");
     return 0;
}