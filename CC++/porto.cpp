# include <stdlib.h>
# include <stdio.h>
# include <math.h>

int main (void)
{
	char auswahl;
	int zahl21,zahl11;
	double zahl1=0,zahl2=0,erg,gew ;
	printf("wie schwer ist ihr paket");
	scanf("%d",gew);
	
	if(gew>=4)
	{
		printf("5,62 EUR");
	}
	else if( 4<gew>=8)
	{
		printf("6,39 EUR");
	}
	else if (8<gew>=12)
	{
		printf("7,16 EUR");
	}
	else if( 12<gew>=20)
	{
		printf("8,69 EUR");
	}


	
	printf("Bitte wählen sie das land aus");
	printf("0 im inland\n1 Zone 1 EU und nachbarländer\n2 Zone 2 Rest Europa\n3 Zone 3 Nordamerika, Nordafrika, Naher Osten\n4 Zone 4 Rest Welt\n5Ende");
    scanf("%c",&auswahl);


	switch(auswahl)
	{
	    case '0':   	if(gew>=4)
	{
		printf("5,62 EUR");
	}
	else if( 4<gew>=8)
	{
		printf("6,39 EUR");
	}
	else if (8<gew>=12)
	{
		printf("7,16 EUR");
	}
	else if( 12<gew>=20)
	{
		printf("8,69 EUR");
	}
	 break;
		
		case '1':   if(gew>=4)
	{
		printf("14,57 EUR");
	}
	else if( 4<gew>=8)
	{
		printf("18,66 EUR");
	}
	else if (8<gew>=12)
	{
		printf("22,75 EUR");
	}
	else if( 12<gew>=20)
	{
		printf("30,93 EUR");
	}
	 break;

		case '2':  if(gew>=4)
	{
		printf("14,75 EUR");
	}
	else if( 4<gew>=8)
	{
		printf("18,66 EUR");
	}
	else if (8<gew>=12)
	{
		printf("22,75 EUR");
	}
	else if( 12<gew>=20)
	{
		printf("30,93 EUR");
	}
	 break;
		case '3':   if(gew>=4)
	{
		printf("14,75 EUR");
	}
	else if( 4<gew>=8)
	{
		printf("18,66 EUR");
	}
	else if (8<gew>=12)
	{
		printf("22,75 EUR");
	}
	else if( 12<gew>=20)
	{
		printf("30,93 EUR");
	}
	 break;
		case '4':  if(gew>=4)
	{
		printf("14,75 EUR");
	}
	else if( 4<gew>=8)
	{
		printf("18,66 EUR");
	}
	else if (8<gew>=12)
	{
		printf("22,75 EUR");
	}
	else if( 12<gew>=20)
	{
		printf("30,93 EUR");
	}
	 break;
		case '5':  if(gew>=4)
	{
		printf("14,75 EUR");
	}
	else if( 4<gew>=8)
	{
		printf("18,66 EUR");
	}
	else if (8<gew>=12)
	{
		printf("22,75 EUR");
	}
	else if( 12<gew>=20)
	{
		printf("30,93 EUR");
	}
	 break;
	}

	printf("ergebnis %lf",erg);
	
system("pause");
	return 0;
}
