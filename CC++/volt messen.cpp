# include <stdlib.h>
# include <stdio.h>
# include <windows.h>

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
	
	int z,v,h,w;
	double Messwert[10];

	for(z=0;z<=9;z++)
	{
		//do {
		
		printf (" %2d Meswert [Volt]: ",z+1 );
	    scanf("%lf",&Messwert[z]);
		if((Messwert[z] < 0.0) || (Messwert[z] > 10.0));
		{
		printf("Wert noch einmal eingeben");
		}
		 while ((!(Messwert[z] < 0.0) && (Messwert[z] > 10.0)));
		}
	    
	  system("cls");
	  printf("Messwerte:\n" );

	  for(z=0;z<=9;z++)
	  {
		  printf("%7.2lf\n",Messwert[z]);
	  }

	  system("pause");
	  system("cls");
	 

	 printf("   |\n");
	 printf("10 |\n");
	 printf("   |\n");
	 printf("   |\n");
	 printf("   |\n");
	 printf("   |\n");
	 printf(" 5 |\n");
	 printf("   |\n");
	 printf("   |\n");
	 printf("   |\n");
	 printf("   |\n");
	 printf(" 0 ---------------------------------");
	 w=0;
	
	 	 for(i=0;i<9;i++)
	 {
		 for(k=i+1;k<10;k++)
		 {
			 if(Messwert[i]>Messwert[k])
			 {
				 hilf = Messwert[i];
				 Messwert[i] = Messwert [k];
				 Messwert[k] = hilf;
		
			 }}}

	 
	 for(v=1;v<=9;v++)
	 {
	
	 for(h=1;h<=(int)Messwert[w];h++)
	 {
		 gotoxy(3+(v*2),13-h);
		 printf("%c",223);
	 }
	 w++;
	 }
	    system("pause");
		return 0;

}


