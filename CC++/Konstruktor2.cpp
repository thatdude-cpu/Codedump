#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>

using namespace std;

class Konto
{

private:
	char Name[20];
	int Kontonr;
	double Kontostand;

public:
	Konto();
	Konto(char[],int,double);
	~Konto();
	void scan(void);
	void print(void);

};

Konto::Konto (void) 
{
	cout << "konstruktor wird ausgeführt\n\n"<< endl;
	strcpy (Name,"Leer");
	Kontonr=0;
	Kontostand=0.0;
}



Konto::Konto (char l_name[],int l_kontonr ,double l_kontost) 
{
	cout << "invidual konstruktor wird ausgeführt\n\n"<< endl;
	strcpy (Name,l_name);
	Kontonr=l_kontonr;
	Kontostand=l_kontost;
}


Konto::~Konto (void)
{
 cout<<	"\n\nDestruktor wird ausgeführt\n\n" <<endl;	
}	
void Konto::scan (void)
{
	cout <<"\nName: ";	
	cin>>Name;
	cout <<"\nKontonummer: ";	
	cin>>Kontonr;
	cout <<"\nKontostand : ";	
	cin>>Kontostand;
}

void Konto::print(void)
{
	cout <<"\nName :"<<Name; 
	cout <<"\nKontonummer :"<<Kontonr;
	cout <<"\nKontostand  :"<<Kontostand;
}



void main (void)
{
	system("cls");
	Konto Kunde("Mustermann",1234,100.99);
	Kunde.print();
	getch();
}