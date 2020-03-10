# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <conio.h>

// Marius Weinhardt
int main (void)
{
	char Zeichen;
	int Zaehler;
	Zaehler=0;

	
	printf("Bitte geben sie ziffern von 0-9 ein wenn sie fertig druecken sie Enter.\n");
	
	
	do

		{
			fflush(stdin);	
	
			Zeichen=(char) getch();
			printf("%c\n",Zeichen);
	
			if((Zeichen>='0')&&(Zeichen<='9'))
	
			{

				Zaehler++;
	
			}

		}
			while  (Zeichen != 13);
		

			printf("%i\n",Zaehler);
		
		
	system("Pause");
	return 0;
}
		