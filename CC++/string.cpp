# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <conio.h>
# include <windows.h>
int main (void)
{
	
	int zähler=0;
	char string1[80];
	
	gets(string1);


	while(string1[zähler] != '\0')
	{
		if(string1[zähler]== 'r')
		{
		  string1[zähler]='l' ;
		}
		
		if(string1[zähler]=='R')
		{
		  string1[zähler]='L' ;
		}
		zähler++;
	}
printf("string1 %s\n",string1);

system("pause");
return 0;
}