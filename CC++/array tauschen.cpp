#include<conio.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>

using namespace std;

void tauschen(int &,int &);

int main ()
{
	int zahl1,zahl2;
		cout <<"Zahl 1 :";
		cin >>zahl1;
		cout <<"Zahl 2 :";
		cin >>zahl2;
		tauschen(zahl1,zahl2);
		cout <<"Zahl 1 :"<<zahl1<<endl;
		cout <<"Zahl 2 :"<<zahl2<<endl;
		getch();
		return 0;
}



void tauschen(int & z1,int & z2)
{
	int temp;
	temp=z1;
	z1=z2;
	z2=temp;
}