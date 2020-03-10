# include <stdio.h>
# include <stdlib.h>
# include <windows.h>
# include <math.h>
# include <conio.h>

int main (void)

{
	double Endsumme,topf, Zinssatz,Zinsen;
	int	Zeit=0;
	printf("Zinsberechnung\n");
    printf("geben sie ihre Anfangssume an\n");
	scanf("%4lf",&topf);
	printf("Summe = %4lf",topf);
    
	do
	{
	printf("Geben sie ihren Zinssatz an\n");
	scanf("%4lf", &Zinssatz);
	printf("Zinssatz = %4lf",Zinssatz);
	if((Zinssatz>'0') && (Zinssatz<'100'))
	printf(" Fehler Geben sie ihren Zinssatz an\n");
	scanf("%4lf", &Zinssatz);
	printf("Zinssatz = %4lf",Zinssatz);
	
	}
	
	while(!((Zinssatz>'0') && (Zinssatz<'100')));

	printf("Geben sie ihre Endsumme an\n");
	scanf("%4lf", &Endsumme);
	printf("Zinssatz = %4lf\n",Endsumme);
	//Rechnung
	
	
	do
	 {
		Zinsen=topf*Zinssatz/100;
        topf=topf+Zinsen;
		Zeit++;
	 printf("Neues Guthaben %lf \n",topf);
	 } while (!(topf > Endsumme));

	 printf("Das geld muss fuer %i\n Jahre angelegt werden\n",Zeit);
	
	
system("Pause");
	 return 0;
}

