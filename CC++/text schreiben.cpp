#include<stdlib.h>
#include<stdio.h>
#include<windows.h>
#include<conio.h>

int main()
{
	FILE *Fp;
	char Zeichen;
	system("cls");
	Fp=fopen("a.txt","r");
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
	printf("Faggot");
	getch();
	}
	system("pause");
	return 0;
}