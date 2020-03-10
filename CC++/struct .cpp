#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

struct K_Daten
{
	char Name[20];
	int Kontonummer;
	double Kontostand;
    int passwort[12];
	
};
void Eingabe (struct K_Daten []);
void Ausgabe (struct K_Daten []);

int main()
{
	struct K_Daten Kunde[4];
	int i=0;
	for (i=0;i<3;i++)
	{
	system("cls");
	Eingabe(Kunde);
	Ausgabe(Kunde);
	system ("Pause");
	}
	return 0;

}

	void Eingabe (struct K_Daten E_Kunde[])
	{	
		int i=0;
		for (i=0;i<=3;i++)
		{
		printf("Eingabe Kundendaten : \n\n");
		printf("Name :");
		scanf("%s",E_Kunde[i].Name);
		printf("Kontonr :");
		scanf("%d",&E_Kunde[i].Kontonummer);
		printf("Kontostand :");
		scanf("%lf",&E_Kunde[i].Kontostand);
		}
	}

	void Ausgabe (struct K_Daten A_Kunde [])
	{
		int i=0;
		for (i=0;i<=3;i++)
		{
		printf("Kundendaten :Name %sKontonummer %d Kontostand%lf\n\n",A_Kunde[i].Name,A_Kunde[i].Kontonummer,A_Kunde[i].Kontostand);
		}
		}
	

	//Auf der Lichtung auf der Strasse in der Stadt jeder hörte was er zu sagen hat niemand stellte ihn infrage es war egal was er tat er hatte recht ohne frage er wusste was zu tuen war jeder lauschte seine worten jeder tat was er gesagt jeder wollte bei ihm warten ohne ihn je gesehen zu haben. so kam es dass man ihn suchte ohne zu wissen wer er war sie suchten und sie fahnden ihn Auf dem Berg und in der Hölle. Jede tag und jede Nacht so wie er zu ihnen sprach.


	// 
	


