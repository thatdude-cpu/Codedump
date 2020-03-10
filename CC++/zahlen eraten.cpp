# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <math.h>
# include <windows.h>

int main (void)
{
	int zaeler,Zufallszahl,Zahl;
	zaeler=0;

	printf("Zahlenspiel\n");
	printf("Erraten sie die Ziffer von 1-15 sie haben 3 Versuche\n");
	srand ((unsigned)time (NULL) );
	Zufallszahl = (rand()%15+1);
	do
	{
	 scanf("%i",&Zahl);
	 zaeler++;
	 if(Zahl == Zufallszahl)
	 {
		 printf("Richtig\n");
		 printf("%i",Zufallszahl);
	     printf("%i",zaeler);
	 }
	 else
	 if(zaeler == 3)
	 {
		 printf("Versuche aufgebraucht sie haben Verloren\n");
		 printf("%i\n",Zufallszahl);
	 }
	 else 
	if(Zufallszahl>Zahl)
		printf("groesser\n");
	else
		printf("kleiner\n");
	}
	while(!(Zahl == Zufallszahl)||(zaeler == 3));
	printf("versuchs nochmal\n");
	
	system("pause");
		 return 0;

}