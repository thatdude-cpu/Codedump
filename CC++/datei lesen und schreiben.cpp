#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<math.h>
#include<windows.h>



int main (void)
{

 FILE *Datei;
 int menue,eingabe,lesen=0,string[1000],laenge,Pos,suchen,fertig,hilf,j,i;
 int ende=1;

 do
 {

 system("cls");

 printf("Zahlenkette verwalten\n\nDateiname: Zahlen.dat\n\n1 - Zahl aufnehmen und in Datei eintragen\n2 - gesamte Zahlenkette anzeigen\n3 - Zahlenkette sortieren\n4 - Zahl aus beliebiger Dateiposition lesen\n5 - Zahl in Zahlenkette suchen (Ergebnis: Dateiposition)\n6 - Anzahl der Dateieintraege bestimmen\n7 - Ende\n\nAuswahl: ");
 scanf("%i",&menue);

 switch(menue)
 {

 case 1:
		 {
		 Datei=fopen("Zahlen.dat","a+b");
		 system("cls");
		 fseek(Datei,0,SEEK_END);
		 printf("Weitere Zahl eingeben: ");
		 fflush(stdin);
		 scanf("%i",&eingabe);
		 fwrite(&eingabe,sizeof(int),1,Datei);             //schreiben
		 fclose(Datei);
		 break;
		}

 case 2:
		{
		 Datei=fopen("Zahlen.dat","a+b");
		 system("cls");
		 fseek(Datei,0,SEEK_END);
		 fertig=ftell(Datei);                      //länge bestimmen
		 fertig=fertig/sizeof(int);
		 rewind(Datei);
		 for(hilf=0;hilf<fertig;hilf++){
			fread(&lesen,sizeof(int),1,Datei);               //lesen
			printf("%i\n",lesen);                       //schreiben
		 }
			getch();
			fclose(Datei);
			break;
		}

 case 3:
		{
		 Datei=fopen("Zahlen.dat","a+b");
		 system("cls");

		 fseek(Datei,0,SEEK_END);
		 laenge=ftell(Datei);                               //Laenge bestimmen
		 rewind(Datei);

		 for (i = 0; i< ((laenge) / sizeof(int)); ++i) 
		 {
			 fread(&string[i],sizeof(int),1,Datei);
		 }

		 for (i = 0; i< ((laenge)/sizeof(int) ); ++i)
		 {
			 hilf = string[i];
			 j = i - 1;

			 while ((hilf < string[j]) && (j >= 0))                       //Sortieren
			 {
				 string[j + 1] = string[j];
				 j--;
			 }
			 string[j + 1] = hilf;
		 }
		 
		 fclose(Datei);
		 Datei = fopen("Zahlen.dat", "r+b");           //Datei in anderm Modus öffnen
		 for (i = 0; i< ((laenge) / sizeof(int) ); ++i) {
			 fwrite(&string[i],sizeof(int),1,Datei);                                //neu schreiben
		 }
		 printf("Sortiert!\n");
		 getch();
		 fclose(Datei);
		 break;
		}

 case 4:
		 {
		 Datei=fopen("Zahlen.dat","a+b");
		 system("cls");

		 printf("Position: ");
		 fflush(stdin);
		 scanf("%i",&Pos);                                 //Position einlesen
		 fseek(Datei,(Pos-1)*sizeof(int),SEEK_SET);        //an Position suchen
		 fread(&lesen,sizeof(int),1,Datei);
		 printf("Zahl aus der Position: %i",lesen);		   //Ausgeben

		 getch();
		 fclose(Datei);
		 break;
		}

 case 5:
		{
		 Datei=fopen("Zahlen.dat","a+b");
		 system("cls");

		 printf("Zahl, nach der gesucht wird: ");
		 fflush(stdin);
		 scanf("%i",&suchen);                               //Zahl einlesen
		 Pos=0;

		 fseek(Datei,0,SEEK_END);
		 fertig=ftell(Datei);                              //Länge bestimmen
		 fertig = fertig / sizeof(int);
		 rewind(Datei);

		 for(hilf=0;hilf<fertig;hilf++)
		 {
			fread(&lesen,sizeof(int),1,Datei);            //Position suchen

			if(suchen==lesen)
			{
				printf("Position: %i\n",Pos+1);           //Position ausgeben
			}
			Pos++;
		 }
			getch();
			fclose(Datei);
			break;
		}

 case 6:
		{
		 Datei=fopen("Zahlen.dat","a+b");
		 system("cls");

		 fseek(Datei,0,SEEK_END);                     //Ans ende springen
		 laenge=ftell(Datei);                        //Position auslesen
		 printf("Laenge: %i",laenge/4);               //Position Ausgeben

		 getch();
		 fclose(Datei);
		 break;
		}

 case 7:
		{
		 ende=false;                          //Abbruchbedingung
		 break;
		}
 }

 }while(ende);
}