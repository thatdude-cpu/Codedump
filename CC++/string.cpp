# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <conio.h>
# include <windows.h>
int main (void)
{
	
	int z�hler=0;
	char string1[80];
	
	gets(string1);


	while(string1[z�hler] != '\0')
	{
		if(string1[z�hler]== 'r')
		{
		  string1[z�hler]='l' ;
		}
		
		if(string1[z�hler]=='R')
		{
		  string1[z�hler]='L' ;
		}
		z�hler++;
	}
printf("string1 %s\n",string1);

system("pause");
return 0;
}