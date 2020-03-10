#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <Windows.h>

//Marius Weinhardt	9.10.2014
//--------------------------------------------------
void gotoxy (short x, short y)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X=x-1;
	pos.Y=y-1;
	SetConsoleCursorPosition(hcon , pos);
}
	int main(void)

{
 	int neu,alt,ver;
	double veur,zus,mwst,end;
	
	printf("neuer zaehlerstand:			    KWh"); 
	gotoxy(37,1);
	scanf("%6i",&neu);
	gotoxy(37,1);
	printf("%6i\n",neu);
	printf("alter zaelerstand:			    KWh");
	gotoxy(37,2);
	scanf("%6i",&alt);
	gotoxy(37,2);
	printf("%6i\n",alt);
	if(neu<alt)
	 {
	 neu+=100000;
	 gotoxy(37,3);
	 printf("   !!! Achtung Zaehlerueberlauf !!!\n");
     }
	ver=neu-alt;
	printf("			-------------------\n");
	printf("Verbrauch:                          %6d\n",ver);
	printf("*********************************************\n");
	printf("Rechnung\n");
	printf("preis fuer 1 KWh:		 0.13 EUR\n");
	printf("Anschlussgebuehr:				       27.30 EUR\n");
	veur=ver*0.13;
	printf("Verbrauch:		  %i * 0.13 EUR =        + %7.2lf EUR\n",ver,veur);
	printf("					              ----------\n");
	zus=veur+27.30;
	printf("					             %7.2lf EUR\n",zus);
	mwst=zus/100*19;
	printf("MWST (19%%)			   	           + %7.2lf EUR\n",mwst);
	printf("					              ----------\n");
	end=zus+mwst;
	printf("Endbetrag:			                     %7.2lf EUR\n",end);
 

 system("Pause");
 return 0;
}