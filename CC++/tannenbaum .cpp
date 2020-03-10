#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<conio.h>
#include<windows.h>
//Marius Weinhardt
int main (void)
{
	 int s,i,j,k,n,durchlauf;
	 
	
	
	 printf("Anzahl der Zeilen:");
     scanf("%i",&n);

	 for (durchlauf=0;durchlauf< n;durchlauf++);
	 {
	

	 for(i=0 ; i < n ;i++)
	 {
		 printf("\n");
	 
	 for(k=0; k < n+1-i;k++)
	 {	
		 printf(" ");
	 }
	 for(j=0;j < 2*i+1;j++)
	 {	
		 printf("*");
	 }
	 }
	 }
	 
	 system("Pause");
	 return 0;
	 
}
