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
	printf("\n1 Dateneingabe");
	printf("2. Datenausgabe mit Auswertung\n");
	printf("3. Schreiben in Datei\n");
	printf("4 Lesen aus Datei\n");
	printf("5. Grafische Auswertung\n");
	printf("6. Ende\n");
	printf("Bitte waehlen [_]\n");
	wahl=getch();
   

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
	
	double min;
	double max;
	double durch=0;
	int i=0;
	double LZ;
	
	
		//Minimum Maximum
			 min=A_Ausgabe[0].Durchschnittstemperatur;
	         max=A_Ausgabe[0].Durchschnittstemperatur;
			 
			 for(i=0;i!=7;i++)
			 {
			 if(A_Ausgabe[i].Durchschnittstemperatur>max)
			 {
				 A_Ausgabe[i].Durchschnittstemperatur=max;

			 }
			 else{
				 if(A_Ausgabe[i].Durchschnittstemperatur<min)
				 {
					 A_Ausgabe[i].Durchschnittstemperatur=min;
				 }
				 }
			 }
			
			 

			 //Wochenbetriebsdauer insgesamt
	LZ=A_Ausgabe[i].Tagesbetriebsdauer+A_Ausgabe[i+1].Tagesbetriebsdauer;
	for(i=0;i!=7;i++)
	{
	LZ=LZ+A_Ausgabe[i].Tagesbetriebsdauer;
	}
	
	//Ausgabe tabelle
    	printf("Daten\n");
		printf("Woche %i\n",Kwoche);
		printf("Wochentag ");
		printf("Durchschnittstemperatur ");
	    printf("Tagesbetriebsdauer ");
		printf("gelieferte waermemenge ");
	
				 for(i=0;i!=7;i++)
	{
		
		//Ausgabe daten
		printf("%lf",A_Ausgabe[i].Durchschnittstemperatur);
		printf("%lf\n",A_Ausgabe[i].Tagesbetriebsdauer);
		printf("%lf\n",A_Ausgabe[i].wärmemenge);
	    printf("Durchschnitt%lf\n",durch);
		printf("Laufzeit%lf",LZ);
	    printf("min %lf max %lf",min,max);
	}



	

	  menue();



}

void Schreiben (struct Daten  Schreiben[], int Kwoche)
{
	printf("Datei wird gespeichert");
    //schreiben
	int I=0;
	char name[13];
	FILE *fp;
	sprintf(name,"KW_%i",Kwoche);
	printf("Dateiname %s",name);
    if((fp=fopen(name,"wb")) == NULL)
	{
		printf("Fehler");
	}
	else{
		for(I=0;I<6;I++)
		{
		
		fwrite(&Schreiben[I],sizeof(Schreiben[0]),1,fp);
		}
		fclose(fp);
	}

		}





void Laden (struct Daten  Laden[], int *Kwoche)
{
	
	//lesen
	FILE *fp;
	printf("Datei namen eingeben");
	scanf("%i",&Kwoche);
	printf("Datei wird geladen");

	int I=0;
	char name[13];
	sprintf(name,"KW_%i",Kwoche);
	printf("Dateiname %s",name);
    if((fp=fopen(name,"rb")) == NULL)
	{
		printf("Fehler");
	}
	else{
	while(!feof(fp))
		{
		
		fwrite(Laden,sizeof(Laden[7]),7,fp);
		}
		fclose(fp);
	}
	

}


void Auswertung (struct Daten  Auswertung[], int Kwoche)
{
	printf("naja");

}

void Ende (struct Daten  Ende[])
{
	printf("ende");
}






