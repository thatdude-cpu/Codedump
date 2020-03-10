# include <stdlib.h>
# include <stdio.h>
# include <math.h>

int main (void)
{
	char oper;
	int zahl21,zahl11;
	double zahl1=0,zahl2=0,erg;
	printf("Bitte wählen sie die Rechenoperation aus");
	
    scanf("%c",&oper);

	switch(oper)
	{
	    case '+':    

	scanf("%lf",&zahl1);			
			erg= zahl1+zahl2; break;
		case '-':  	scanf("%lf",&zahl1);	 erg= zahl1-zahl2; break;
		case '*':   	scanf("%lf",&zahl1);	 erg=zahl1*zahl2; break;
		case '/':  	scanf("%lf",&zahl1);	  erg=zahl1/zahl2; break;
		case '%':   	scanf("%lf",&zahl1);	 erg=zahl11%zahl21; break;
		default: printf("Fehlerhafte Rechenoperation");
	}

	printf("ergebnis %lf",erg);
	
system("pause");
	return 0;
}
