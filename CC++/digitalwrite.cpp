#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<conio.h>
int main (void)
{
	int auswahl,bitnr,i,bitnr2,f;

	unsigned char in;
	unsigned char out,out2;

	const unsigned char sbit_0=1;
	const unsigned char sbit_1=2;
	const unsigned char sbit_2=4;
	const unsigned char sbit_3=8;
	const unsigned char sbit_4=16;
	const unsigned char sbit_5=132;
	const unsigned char sbit_6=164;
	const unsigned char sbit_7=128;

	const unsigned char lbit_0=254;
	const unsigned char lbit_1=253;
	const unsigned char lbit_2=251;
	const unsigned char lbit_3=247;
	const unsigned char lbit_4=239;
	const unsigned char lbit_5=223;
	const unsigned char lbit_6=191;
	const unsigned char lbit_7=127;

	const unsigned char tbit_0=1;
	const unsigned char tbit_1=2;
	const unsigned char tbit_2=4;
	const unsigned char tbit_3=8;
	const unsigned char tbit_4=16;




	if(OpenDevice(0)==-1)
	{
		printf("Fehler beim oeffnen des geräts\n");
		system("pause");
	}

	do
	{
		system("cls");
		printf("****Auswahlmenue\n");
		printf("1 Alle Bits setzen\n");
		printf("2 Alle bits loeschen\n");
		printf("3 einzelnes bit abfragen\n");
		printf("4 einzelnes bit seztzen\n");
		printf("5 einzelnes bit loeschen\n");
		printf("6 Lauflicht starten\n");
		printf("7 Lauflicht 2 starten\n");
		printf("8 Programm beenden\n");
		printf("Ihre aktuelle auswahl:\n");
		fflush(stdin);
		scanf("%d",&auswahl);

		switch(auswahl)
		{
		case 1: system("cls");
		        out=255;
				WriteAllDigital(out);
				printf("Alle Bits gesetzt");
				getch(); break;
		case 2: system("cls");
			    out=0;
				WriteAllDigital(out);
				printf("Alle Bits gelöscht");
				getch(); break;
		case 3: system("cls");
				in=(unsigned char ) ReadAllDigital();
				
				if(in&tbit_0)
				{
				printf("Schalter 1 an\n");
		        }
				else
				{
					printf ("Schalter 1 aus\n");
				}

				if(in&tbit_1)
				{
				printf("Schalter 2 an\n");
		        }
				else
				{
					printf ("Schalter 2 aus\n");
				}


				if(in&tbit_2)
				{
				printf("Schalter 3 an\n");
		        }
				else
				{
					printf ("Schalter 3 aus\n");
				}

				if(in&tbit_3)
				{
				printf("Schalter 4 an\n");
		        }
				else
				{
					printf ("Schalter 4 aus\n");
				}

				if(in&tbit_4)
				{
				printf("Schalter 5 an\n");
		        }
				else
				{
					printf ("Schalter 5 aus\n");
				}


				getch(); break;
		case 4:system("cls");
			printf ("Welches bit möchten sie setzen0-7\n");	
			scanf("%d",bitnr);
			
			
			switch(bitnr)
			{
			 case 1:out2=sbit_0;
				    out=out|sbit_0
			 case 2:out2=sbit_0;
				    out=out|sbit_0
			 case 3:out2=sbit_0;
				    out=out|sbit_0
			 case 4:out2=sbit_0;
				    out=out|sbit_0
			 case 5:out2=sbit_0;
				    out=out|sbit_0
			 case 6:out2=sbit_0;
				    out=out|sbit_0
			 case 7:out2=sbit_0;
				    out=out|sbit_0
			}

			getch(); break;
		case 5:system("cls");
				printf ("Welches bit möchten sie löschen0-7\n");	
			scanf("%d",bitnr);
			
			
			switch(bitnr)
			{
			 case 1: 
			 case 2:
			 case 3:
			 case 4:
			 case 5:
			 case 6:
			 case 7:
			}
			getch(); break;
		case 6:system("cls"); 
				getch(); break;
		case 7:system("cls");
				getch(); break;
		case 8:system("cls");
				getch(); break;
		}

	}while (true);
	