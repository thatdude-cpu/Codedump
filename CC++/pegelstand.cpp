# include <stdlib.h>
# include <stdio.h>
# include <windows.h>
# include <math.h>

void gotoxy (short x, short y)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X=x-1;
	pos.Y=y-1;
	SetConsoleCursorPosition(hcon , pos);
}
int main (void)
{
int d;
double wert,min=0,max=0,Summe=0.0,durchschnitt=0,schw=0 ;



	for(d=0;d<=9;d++)
	{

	do
{ 	
printf("Bitte Pegelstand fuer %02d:00 eingeben[m]:",d+3);

scanf ("%lf",&wert);
 
if(wert > 1)
	
	{
		printf("Neueingabe\n");
    }
}			
while((wert<0.0) || (wert > 1));

	  if (d==1)
 {
	 min=max=wert;
 }
  else if(wert<min)
  {
	  wert=min;
 }
  else if(wert>max)
  {
	  max=wert;
  }
	 Summe=Summe+wert;
 


}

printf("kleinster Pegelstand %lf\n",min); 
durchschnitt=Summe/10;
printf("groeﬂter Pegelstand %lf\n",max);
schw=max-min;
printf("groeﬂte Pegelschwankung %lf\n",schw);
printf("Durchschnitt %8.2lf\n",durchschnitt);
if(wert<0.25)
{
printf("Achtung Pegelunterschreitung\n");
}


system ("pause");
	return 0;
}

