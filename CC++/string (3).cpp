#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include<windows.h>
void fkt (char str1[],char str2[]);

	void main ()
{
	
    char str1[5],str2[5];
	  
	    str1[0] = 'H';
		str1[1] = 'A';
		str1[2] = 'N';
		str1[3] = 'S';
		str1[4] = '\0';
		
		fkt(str2,str1);

	printf("%s %s",str2,str1);
	getch();

}

void fkt (char str2[],char str1[])
	
{
	int i=0;
	while(str1[i] !='\0')
	{
		str2[i]=str1[i];
		i++;
	}
	str2[i]='\0';
	}