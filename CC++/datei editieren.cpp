#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
void menu();
void zahlspeichern();
void anzeigen();
void sortieren();
void dateipos();
void zahlsuchen();
void groeﬂeausgeben();
int ende();
//Marius Weinhardt 25.9.15
int main (void)
{
	menu();
}
void menu()
{
int	wahl = 0;
printf("Dateiname: text.txt\n");
printf("1. Zahlspeichern\n");
printf("2. Zahlenkette anzeigen\n");
printf("3. groeﬂe ausgeben\n");
printf("4. Zahlenkette sortieren \n");
printf("5. Zahl in Zahlenkette suchen\n");
printf("6. Ende\n");
wahl=getch();
switch(wahl)
{
		case '1' : zahlspeichern(); break;
		case '2' : anzeigen(); break;
		case '3' : groeﬂeausgeben(); break;
		case '4' : sortieren(); break;
		case '5' :dateipos(); break;
		case '6' : zahlsuchen(); break;
		case '7' : ende(); break;
}

}

void zahlspeichern()
{
	FILE *fp;
	int zeichen;
	
	
	//Bin‰r lessen
	fp = fopen("text.dat","a+b");
	if(fp==0)
	{
	printf("Computer sagt nein");
	}
	else
	{
	printf("zeichen eingeben ende esc\n");
	fflush(stdin);
	zeichen=getch();
	fwrite(&zeichen,sizeof(int),SEEK_CUR,fp);
	printf("%c\n",zeichen);
	
	}
	fclose(fp);
	/*  Text lesen
	fp = fopen("text.txt","w");
	if(fp==0)
	{
	printf("Computer sagt nein");
	}
	else
	{
	printf("zeichen eingeben ende esc");
	zeichen=getch(),
	fputc(zeichen,fp);
	printf("%c",zeichen);
	}
	while(zeichen != 27)
	{	
	zeichen=getch();
	if(zeichen == 13)
	{
	   fputc(13,fp);
		fputc(10,fp);
	}	
	else{
	fputc(zeichen,fp);
	printf("%c",zeichen);
	}
	
	}
	*/
	
	getch();
	menu();

}

void anzeigen()
{
	FILE *fp;
	int zeichen;
	int i=0;
	fp = fopen("text.dat","a+b");
	if(fp==0)
	{
	printf("Computer sagt nein");
	}
	
	//mit bin‰r
	int endpos=0;
	fseek(fp,0,2);
		endpos=ftell(fp)/sizeof(int);
	fseek(fp,0,0);
	for(i=0,i<endpos;i++;)
	{
	fread(&zeichen,i+sizeof(int),SEEK_CUR,fp);
	getch();
	printf("%c\n",zeichen);
	fflush(stdin);
	getch();
	}
	/*mit text dateien
	while(!feof)
	{
	zeichen = fgetc(fp);
	printf("%c",zeichen);
	}
	*/
	fclose(fp);
	getch;
	menu();

}

void groeﬂeausgeben()
{
	int groese;
	FILE *fp;
	fp = fopen("text.txt","rb");
	fseek(fp,sizeof(int),SEEK_END);
	groese=ftell(fp);
	printf("%i",groese/4);
	getch();
	menu();
 
}

void sortieren()
{
	menu();
}

void dateipos()
{
		int zahl=0;
	int dat;
	FILE *fp;
	printf("ZAhl eingeben");
	fflush(stdin);
	zahl=getch();
	printf("%i\n",zahl);
	fp = fopen("text.txt","rb");
	//suche nach zahl
	
	fseek(fp,0*sizeof(int),SEEK_SET);
	
}


void zahlsuchen()
{
	int zahl=0;
	int dat;
	FILE *fp;
	printf("ZAhl eingeben");
	fflush(stdin);
	zahl=getch();
	printf("%i\n",zahl);
	fp = fopen("text.txt","rb");
	//suche nach zahl
	
	fseek(fp,0*sizeof(int),SEEK_SET);
	while(!feof(fp))
	{
	 dat=ftell(fp);

		if(zahl)
	{
		printf("dateiposition%i\n",dat);
	 getch();
	}
	else{
			
		fseek(fp,0*sizeof(int),SEEK_CUR);
	
	}
			printf("Nicht gefunden\n");
	}

	fclose(fp);

	menu();

}


int ende()
{

	return 0;

}