# include <stdlib.h>
# include <stdio.h>
# include <windows.h>

//Marius Weinhardt
int main (void)
{
	
	int zähler=0,z=0;
	double Zeichen[5];

		for(z=0;z<=4;z++)
		 {
	
		
		printf (" Zeichen eingeben " );
	    scanf("%s",&Zeichen[z]);
		 
		}
		
		while(Zeichen[z])
	    {
		
		 zähler++;
		
	    }
	    
		
		printf("Anzahl der Zeichen: %i",zähler);

		
	 system("Pause");
     return 0;
}