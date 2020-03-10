# include <stdlib.h>
# include <stdio.h>
# include <windows.h>
# include <math.h>
# include <time.h>

int main (void)
{
	int Zaeler,Zufallszahl;
	Zaeler=0;

	printf("Wuerfel\n");

	srand ((unsigned)time (NULL) );
	
	do
	{
	Zufallszahl = (rand()%6+1);
	printf("gewuerfelt: %i\n",Zufallszahl);
	Zaeler++;
	Sleep (500) ;
	}
	 while(Zufallszahl != 6);

	 printf("Anzahl Wuerfe: %i\n",Zaeler);

 system("Pause");
 return 0;
}