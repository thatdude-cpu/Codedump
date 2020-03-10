#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include<windows.h>
//Marius Weinhardt 18.9.15
int main (void)
{
FILE *fp;
char file[100],zeichen;
printf("Bitte geben sie den namen der Textdatei ein zum lesen");
scanf("%s",&file);
fp=fopen(file,"r");
if(fp==0)
{
	printf("datei kann nicht geoeffnet werden");

}
else{

while(!feof)
{
zeichen=fgetc(fp);
printf("%c",zeichen);
}
}
fclose(fp);


//schreiben
printf("Bitte geben sie den namen der Textdatei ein zum schreiben");
scanf("%s",&file);
fp=fopen(file,"w");
if(fp==0)
{
	printf("datei kann nicht geoeffnet werden");

}
else{
printf("zeichen eingeben ende mit esc");
fflush(stdin);
zeichen=getch();
fputc(zeichen,fp);
printf("%c",zeichen);
while(zeichen!=27)
{
zeichen=getch();
if(zeichen==13)
{
	fputc(10,fp);
}
else{

fputc(zeichen,fp);
printf("%c",zeichen);
}

}
}
return 0;
}