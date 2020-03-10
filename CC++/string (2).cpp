# include <stdlib.h>
# include <stdio.h>
# include <string.h>


int main (void)
{
	int len,zähler=0;
	char string1[30]= "Donauschiff";
	char string2 [9]="kapitaen";
	
	printf("string1 %s\n",string1);
	
	len=strlen (string1);
    
	printf("%s\n",len);
	
	strcat (string1,string2);
	
	printf("%s\n,string1");




system("pause");
return 0;
}