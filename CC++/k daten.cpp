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
void Eingabe (struct K_Daten *);
void Ausgabe (struct K_Daten);

int main()
{
	struct K_Daten Kunde;

	system("cls");
	Eingabe(&Kunde);
	Ausgabe(Kunde);
	system ("Pause");
	return 0;

}

	void Eingabe (struct K_Daten * E_Kunde)
	{	
		printf("Eingabe Kundendaten : \n\n");
		printf("Name :");
		scanf("%s",E_Kunde->Name);
		printf("Kontonr :");
		scanf("%d",&E_Kunde->Kontonummer);
		printf("Kontostand :");
		scanf("%lf",&E_Kunde->Kontostand);

	}

	void Ausgabe (struct K_Daten A_Kunde)
	{
		
		printf("Kundendaten :Name %sKontonummer %d Kontostand%lf\n\n",A_Kunde.Name,A_Kunde.Kontonummer,A_Kunde.Kontostand);
	}
	

	
	



