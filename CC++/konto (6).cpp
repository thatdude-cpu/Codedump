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
	Konto Kunde[10];
	int auswahl=0,knr;

	cout<<	"1.Kundendaten einlesen" <<endl;	
	cout<<	"2.Kundendaten auslesen" <<endl;	
	cout<<	"3.Bearbeitung ende" <<endl;
	fflush(stdin);

do{

	cin>>auswahl;
	switch(auswahl)
	{
	case 1: 
		{
	cout <<"\nKundenummer eingeben: ";	
	cin>>knr;
	Kunde[knr-1].scan();
		}
		break;
		
	case 2: 
		{
	cout <<"\nKundenummer eingeben: ";	
	cin>>knr;
	Kunde[knr-1].print();
		
	break;	
		}
	}
	}while(auswahl!=3);



	getch();

}