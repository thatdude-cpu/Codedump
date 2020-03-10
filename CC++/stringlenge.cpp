# include <stdlib.h>
# include <stdio.h>
# include <string.h>


int main (void)
{
	int len1,len2;
	int zähler=0;
	char string1[80]= "",string2 [80]="";
	printf("string1 %s\nstring 2 %s\n",string1,string2);
	gets(string1);


	do
	{
    string2 [zähler] = string1 [zähler];
	zähler++;
	}
	while(string1[zähler] != '\0');


printf("string1 %s\nstring2 %s\n",string1,string2 );

system("pause");
return 0;
}