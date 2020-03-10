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
struct K_Daten Eingabe(void);
void Ausgabe (struct K_Daten);

int main()
{
	struct K_Daten Kunde;

	system("cls");
	Kunde=Eingabe();
	Ausgabe(Kunde);
	system ("Pause");
	return 0;

}

	struct K_Daten Eingabe(void)
	{
		struct K_Daten E_Kunde;
		
		printf("Eingabe Kundendaten : \n\n");
		printf("Name :");
		scanf("%s",E_Kunde.Name);
		printf("Kontonr :");
		scanf("%d",&E_Kunde.Kontonummer);
		printf("Kontostand :");
		scanf("%lf",&E_Kunde.Kontostand);

		return E_Kunde;
	}

	void Ausgabe (struct K_Daten A_Kunde)
	{
		
		printf("Kundendaten :Name %sKontonummer %d Kontostand%lf\n\n",A_Kunde.Name,A_Kunde.Kontonummer,A_Kunde.Kontostand);
	}
	
	/* passwort abfrage
	int passwort [20];
	printf("passwort eingeben");
	scanf("%s",&Kunde.passwort);

	printf("passwort wiederholen");
	scanf("%s",&Kunde.passwort);
	
	if(passwort == Kunde.passwort)
	{
		printf("Verifiziert");
	}
	*/
	
	



