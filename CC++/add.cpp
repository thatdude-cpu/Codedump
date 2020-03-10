#include<stdio.h>
#include<conio.h>
#include<stdlib.h>

int add (int,int);

int main ()
{
	int zahl1=0,zahl2=0,erg=0;
	
	printf("erste zahl\n");
	scanf("%d",&zahl1);
	printf("zahl2\n");
    scanf("%d",&zahl2);
    erg = add(zahl1,zahl2);
	system("pause");
	return 0;
}

int add (int zahl_1,int zahl_2)
{
	
	int erg;
	erg=zahl_1+zahl_2;
    return erg;
}


