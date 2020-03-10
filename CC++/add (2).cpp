#include<stdio.h>
#include<conio.h>
#include<stdlib.h>

void add (int,int);

int main ()
{
	int zahl1=0,zahl2=0;
	
	printf("erste zahl\n");
	scanf("%d",&zahl1);
	printf("zahl2\n");
    scanf("%d",&zahl2);
    add(zahl1,zahl2);
	system("pause");
	return 0;
}

void add (int zahl1,int zahl2)
{
	
	printf("ergebnis %d",zahl1+zahl2);
}

