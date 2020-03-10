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
	void scan(void);
	void print(void);
};

void Konto::scan (void)
{
	cout <<"\nName:";
	cin>>Name;
	cout <<"\nKontonummer";
	cin>>Kontonr;
	cout <<"\nKontostand";
	cin>>Kontostand;
}

void Konto::print(void)
{
	cout <<"\nName: "<<Name;
	cout <<"\nKontonummer: "<<Kontonr;
	cout <<"\nKontostand: "<<Kontostand;
}

void main (void)
{
	system("cls");
	Konto Kunde1, Kunde2;
	Kunde1.scan();
	Kunde2.scan();
	Kunde1.print();
	Kunde2.print();
	system("\npause");

}