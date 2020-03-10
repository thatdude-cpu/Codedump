
#include<stdlib.h>
#include<stdio.h>
#include<windows.h>
#include<conio.h>

int main()
{

	//schreiben
	FILE *Fp;
	char Filename[100],Zeichen_E;
	system("cls");
	scanf("%s",Filename);
	system("cls");
	Fp=fopen(Filename,"r");
    if(Fp==0)
	{
	printf("Fehler");
	getch();
	}
	else{
	printf("zeichen eingeben bei esc ende");
	fflush(stdin);
	Zeichen_E=getch();
	printf("%c",Zeichen_E);
	while(Zeichen_E != 27)
	{
	fputc(Zeichen_E,Fp);
	
	if(Zeichen_E==13)
	{
	
	
	fputc(10,Fp);
	printf("%c",10);
	}
	fflush(stdin);
	Zeichen_E=getch();
	printf("%c",Zeichen_E);
	}
	}

	fclose(Fp);
	//lesen
//	FILE *Fp;
//	char Filename[100],Zeichen;
	int Zeichen;
	system("cls");
	scanf("%s",Filename);
	system("cls");
	Fp=fopen(Filename,"r");
    if(Fp==0)
	{
	printf("Fehler");
	getch();
	}
	else{
		
		while(!feof(Fp))
		{
			Zeichen=(char)fgetc(Fp);
			printf("%c",Zeichen);
		}
	
	fclose(Fp);

	getch();
	}
	system("pause");
	return 0;
}