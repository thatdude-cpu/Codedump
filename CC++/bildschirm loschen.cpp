#include<stdlib.h>
#include <stdio.h>
#include <Windows.h>
void gotoxy (short x, short y)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X=x-1;
	pos.Y=y-1;
	SetConsoleCursorPosition(hcon , pos);
}

void cls (void);


	int main ()
{
	int li;
	for(li=1;li<=10;li++)
	{
		printf("Text\n");
	}
	system("pause");
	cls();
	printf ("bildschirm leer");
	system("pause");
	return 0;
}

void cls (void)
{
	int  li;
	for(li=0;li<=300;li++)
	{
		printf("\n");
	}
	gotoxy(1,1);
}
