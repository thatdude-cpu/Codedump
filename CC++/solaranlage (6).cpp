#include<conio.h>
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>

struct Daten
{
	
	
	int Woche;
	double Durchschnittstemperatur;
	double Tagesbetriebsdauer;
    double wärmemenge;
	int LZ;
};


void Eingabe (struct Daten [],int*);
void Ausgabe (struct Daten [],int);
void Schreiben (struct Daten [],int);
void Laden (struct Daten [],int*);
void Auswertung (struct Daten [],int);
void Ende (struct Daten []);
void menue();


int main()
{
	menue();
}

	void menue()
	{
	int wahl=0;
	int Kwoche;
    struct Daten Woche[7];
	printf("Betriebsdaten Solaranlage\n");
	printf("1 Dateneingabe");
	printf("2. Datenausgabe mit Auswertung\n");
	printf("3. Schreiben in Datei\n");
	printf("4 Lesen aus Datei\n");
	printf("5. Grafische Auswertung\n");
	printf("6. Ende\n");
	printf("Bitte waehlen [_]\n");
	wahl =getch();
   

  switch(wahl)
	{
	case '1' : Eingabe(Woche,&Kwoche);
		break;
	case '2' : Ausgabe(Woche,Kwoche);
			break;
	case '3' : Schreiben(Woche,Kwoche);
			break;
	case '4' : Laden(Woche,&Kwoche);
			break;
	case '5' : Auswertung(Woche,Kwoche);
			break;
	case '6' : Ende(Woche);
	         break;
	}
	
	

}


void Eingabe (struct Daten E_Eingabe[], int *Kwoche)
{
	
	printf("Woche:");
	scanf("%i",&Kwoche);
	int i=0;
	
	for(i=0;i!=7;i++)
	
	{
	printf("Durchschnittstemperatur");
	scanf("%lf",&E_Eingabe[i].Durchschnittstemperatur);
	printf("Tagesbetriebsdauer");
	scanf("%lf",&E_Eingabe[i].Tagesbetriebsdauer);
	printf("Gelieferte Waermemenge");
	scanf("%lf",&E_Eingabe[i].wärmemenge);
	}

	printf("Ende");
	menue();

}

void Ausgabe (struct Daten A_Ausgabe[], int Kwoche)
{
	double durch=0;
	int i=0;
	double LZ;
	for(i=0;i!=7;i++)
	{
		Ausgabe[i].Tagesbetriebsdauer
	LZ=A_Ausgabe[i].Tagesbetriebsdauer;
	}
		printf("Daten\n");
		printf("Woche %i\n",Kwoche);
		printf("Wochentag ");
		printf("Durchschnittstemperatur");
	    printf("Tagesbetriebsdauer");
		printf("gelieferte waermemenge");
	
	for(i=0;i!=7;i++)
	   
	{
		
		printf("%lf",A_Ausgabe[i].Durchschnittstemperatur);
		printf("%lf\n",A_Ausgabe[i].Tagesbetriebsdauer);
		printf("%lf\n",A_Ausgabe[i].wärmemenge);
	    printf("Durchschnitt%lf\n",durch);
		printf("Laufzeit%lf",LZ);
	}

	  menue();



}

void Schreiben (struct Daten  Schreiben[], int Kwoche)
{
	printf("Datei wird gespeichert");


}

void Laden (struct Daten  Ausgabe[], int *Kwoche)
{
	printf("ist nich nicht da");
}

void Auswertung (struct Daten  Auswertung[], int Kwoche)
{
	printf("naja");
}

void Ende (struct Daten  Ende[])
{
	printf("ende");
}






