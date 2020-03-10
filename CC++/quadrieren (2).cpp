#include<stdlib.h>
#include<stdio.h>
#include<windows.h>
#include<math.h>
#include<conio.h>
void vertauschen(int*,int*);
void quadrieren (int*,int*);
int main (void)
{
	int a = 3, b = 8;

printf("\nvorher : \ta = %i, \tb = %i",a,b);
vertauschen(&a,&b);
printf("\nnacher : \ta = %i, \tb = %i",a,b);
quadrieren(&a,&b);
printf("\nquadriert : \ta = %i, b = %i",a,b);
getch();
	return 0;
}


void vertauschen (int *f_a,int *f_b)
{
	int tmp;

	tmp=*f_a;
	*f_a=*f_b;
	*f_b=tmp;
}

void quadrieren (int *az,int *bz)
{
	 *az=(*az)*(*az); 
	 *bz=(*bz)*(*bz);
}