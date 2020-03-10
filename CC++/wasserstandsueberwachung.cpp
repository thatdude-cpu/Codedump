#include<stdlib.h>
#include<stdio.h>
#include<conio.h>

void eingabe (int[],int*);
void ausgabe (int[]);
void auswertung (int[],int);


int main ()
{
 
	int stand[12],alarm;
	
		system ("cls");
		eingabe(stand,&alarm);
       system ("cls");
		ausgabe(stand);
		system ("cls");
	//   auswertung(stand,alarm);
		getch();

	}


	void eingabe (int stand[12],int*)
	{
		int i,uhr=0,alarm=0;
		while(i=0,i != 5)
		{
		printf("Wasserstandsueberwachung");
		printf("Wasserstand um %i:00 :",uhr);
		scanf("%s",&stand[i]);
		do
		{
		if((stand[i] > 120) || (stand[i] < 0))
		{
			printf("Falscheingabe");
			scanf("%s",&stand[i]);
		}
		}
		while ((stand[i] < 120) || (stand[i] > 0));
		i++;
	    uhr++;
		}
       printf("Alarmschwelle : ");
	   scanf("%i",alarm);

	}


	void ausgabe (int stand [12])
	{
		printf("wasserstandsueberwachung");
		printf("Uhrzeit          Wasserstand");
		printf("02:00 %s",stand[0]);
		printf("04:00 %s",stand[1]);
		printf("06:02 %s",stand[2]);
		printf("08:02 %s",stand[3]);
		printf("10:02 %s",stand[4]);
		printf("12:02 %s",stand[5]);
		printf("14:02 %s",stand[6]);
		printf("16:02 %s",stand[7]);
		printf("17:02 %s",stand[8]);
		printf("19:02 %s",stand[9]);
		printf("21:02 %s",stand[10]);
		printf("24:00 %s",stand[11]);
	}