#include<stdlib.h>
#include<stdio.h>
#include<windows.h>
#include<conio.h>

int main ()
{
	int wahl=0;
	int modus=0;
	printf("Wilkommen in der Matrix Benutzeroberfl�che bitte Modus ausw�hlen\n");
	printf("1.AR\n");
	printf("2.VR\n");
	wahl=getch();
	
	

	switch (wahl)
	{
	case '1': modus=1; 
	break;
	case '2': modus=2; 
		break;

	}
	
	if(modus==2)
	{
		printf("Fehler VR modus wird nicht unterst�tzt wechsle auf AR");
	    printf("overlay wird erstellt und ausgef�hrt");
	}
	else
	{
		printf("overlay wird erstellt und ausgef�hrt");
	}

	return 0;
	system("pause");
}