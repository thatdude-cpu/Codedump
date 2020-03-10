#include<conio.h>
#include<stdlib.h>
#include<stdio.h>

using namespace std;


int main (void)
{
	int Anzahl;
	int *Z_vekt;
	int x;


	printf("Wieviel Speicherplatz wird benoetigt:");
	scanf("%d",&Anzahl);

	Z_vekt=new int [Anzahl];

	if(Z_vekt==0)
	{
		printf("kein speicherplatz vorhanden\n");

	}
	else
	{

		for(x=0;x<Anzahl;x++)
		{
			printf("geben sie eine %4d. zahl ein ",x+1);
			scanf("%d",&Z_vekt[x]);
		}
	
	for(x=0;x<Anzahl;x++)
	{

		printf("Ausgabe %4d. zahl: %d\n",x+1,Z_vekt[x]);
	}
	
	delete [] Z_vekt;
	
	}
	getch();
}