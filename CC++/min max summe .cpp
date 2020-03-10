# include <stdlib.h>
# include <stdio.h>
# include <windows.h>
# include <math.h>


int main (void)
{
int i;
double wert,min=0,max=0,Summe=0.0,durchschnitt=0 ;



	for(i=0;i<=9;i++)
	{

	do
{ 	
printf("Bitte geben sie den %2d wert ein: ",i+1);

scanf ("%lf",&wert);
 
if((wert < 0.0) || (wert > 100))
	
	{
		printf("Neueingabe\n");
    }
}			
while((wert<0.0) || (wert >100));

	  if (i==1)
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

printf("Summe : %8.2lf",Summe); 
durchschnitt=Summe/10;
printf("Durchschnitt %8.2lf",durchschnitt);

system ("pause");
	return 0;
}

