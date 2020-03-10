#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Marius Weinhardt
int main(void)
{
	double a,b,c,d,x1,x2,x;
	
	printf("Algorithmus Quadratische Gleichung\n");
	printf("Geben sie a ein\n");
	printf("a=");
	scanf("%lf",&a);
	printf("Geben sie b ein\n");
	printf("b=");
	scanf("%lf",&b);
	printf("Geben sie c ein\n");
	printf("c=");
	scanf("%lf",&c);

	d=(b*b)-(4*a*c);
	if(a!=0)
	 {
	   printf("Diskriminante: %lf\n",d);	
	 }  
	 else 
	 {  
	  printf("Keine Quadratische Gleichung");

	 }
		if(d<0.0)
		 {
			printf(" Keine reele Loesung");
		 }	
		else{
		       
			    if (d!=0)
				{			 
				x1=(-b+sqrt(d))/(2*a);
		        x2=(-b-sqrt(d))/(2*a);
			    printf("x1=%lf\n",x1);
			    printf("x2=%lf\n",x2);
			
			    }	
			    else
			    {
				 x=-b/(2*a);
				printf("x=%lf\n",x);
			    }

		 
		}
		   system("Pause");
			return 0;
}