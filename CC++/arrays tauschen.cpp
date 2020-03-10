#include <stdlib.h>
#include <stdio.h>
#include <math.h>
// Marius Weinhardt 24.2.15
int main (void)
{
	int tausch =0,pos,epos=9,hilf;
	int zahlen[10];


	scanf("%s",zahlen);

	do
	{


	for(pos=0;pos<epos;pos++)
	{

		if(zahlen[pos]>zahlen[pos+1])
		{
			hilf=zahlen[pos];
			zahlen[pos]=zahlen[pos+1];
			zahlen[pos+1]=hilf;
			tausch=1;
		}
	}

	epos--;


			} while ((tausch=0) || (epos>=9));

	printf("%s" ,zahlen);

	system("pause");

	return 0;

}
