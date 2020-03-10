# include<stdlib.h>
# include <stdio.h>
# include<math.h>
# include<Windows.h>
# include<time.h>

void gotoxy (short x, short y)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X=x-1;
	pos.Y=y-1;
	SetConsoleCursorPosition(hcon , pos);
}

int main (void)
{
	int Z,S,V;
	{
	system("cls");
	printf("Geben sie bitte die Geeschwindigkeit ein (1-1000): ");
	scanf("%i",&V);
	system ("cls");
	Z=1;
	while(Z<=16)
	{
		for(S=1;S<=30;S++)
		{
			if(S%2)
			{
				system("cls");
				gotoxy(S,Z    ); printf("         %c%c%c<  ",219,219,219);
				gotoxy(S,Z+1  ); printf("          %c   %c",219,220);
				gotoxy(S,Z+2  ); printf("       %c%c%c%c%c%c%c  ",219,223,219,219,219,223,223);
				gotoxy(S,Z+3  ); printf("         %c%c%c",219,219,219);
				gotoxy(S,Z+4  ); printf("         %c %c",219,219);
				gotoxy(S,Z+5  ); printf("         %c%c %c%c",219,220,219,220);
			
			}
			else 
			{ 
				system("cls");
				gotoxy(S,Z    ); printf("       %c%c%c-  ",219,219,219);
				gotoxy(S,Z+1  ); printf("        %c       ",219);
				gotoxy(S,Z+2  ); printf("       %c%c%c  ",219,219,219);
				gotoxy(S,Z+3  ); printf("       %c%c%c",219,219,219);
				gotoxy(S,Z+4  ); printf("        %c",219,219);
				gotoxy(S,Z+5  ); printf("        %c%c",219,220);
			
			}
			if (S==30)
			{
				system("cls");
				gotoxy(S,Z+1  ); printf("  %c%c%c",219,219,219);
				gotoxy(S,Z+2  ); printf("   %c",219);
				gotoxy(S,Z+3  ); printf("  %c%c%c",219,219,219);
				gotoxy(S,Z+4  ); printf("  %c%c%c%c%c",219,219,219,220,220);
				gotoxy(S,Z+5  ); printf("   %c  %c%c",178,219,220);
				
			}
			Sleep(1000);
		}
		Sleep(1000-V);
	}
	system("cls");
	Z=Z+8;
}
  printf("Bitte return drücken");

 
 system("Pause");
 return 0;
}