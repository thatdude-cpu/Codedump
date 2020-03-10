# include <stdlib.h>
# include <stdio.h>
# include <windows.h>
# include <math.h>
# include <time.h>
# include <conio.h>
int main (void)
{
	int Zaeler,Zufallszahl;
	Zaeler=0;
	int Zahl;

	printf("Zahlenspiel");

	srand ((unsigned)time (NULL) );
	
	do
	{
	Zufallszahl = (rand()%15+1);
	printf("Erraten sie die Zahl von 1 bis 15\n");
	scanf("%i",Zahl);
	Zaeler++;
	if(Zahl == Zufallszahl);
	{
	 printf("Richtig");
     printf("%i",Zufallszahl);
	printf("%i",Zaeler);
	}
	else
	{
		if
	}

	
		
		while(Zahl != 16);

	 

 system("Pause");
 return 0;
}