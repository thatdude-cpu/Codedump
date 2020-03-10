#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>

using namespace std;

class application
{
public:
	application(int objektNr)
	{
	 this->objektNr=objektNr;
		cout <<"Loading..."<<objektNr<<"\n\n"<<endl;
	}
	~application()
	{
	 cout <<"Shut down..."<<objektNr<<"\n\n"<<endl;
	}
	void TextOut(void)
	{
	 cout <<"Running..."<<objektNr<<"\n\n"<<endl;
	}
	private:
		int objektNr;
};

int main (int argc, char* argv[])
{
	application **objektliste;
	int Anzahl;
	cout <<"Anzahl der Objekte"<<endl;
	cin >>Anzahl;

	objektliste = new application *[Anzahl];

	for(int i=0;i<Anzahl;i++)
	{
		objektliste [i] = new application(i+1);
	}

	cout <<"Rufe ausgabe funktion auf"<<endl;
	for(int i=0;i<Anzahl;i++)
	{		 
		objektliste[i]->TextOut();
	}
	for(int i=0;i<Anzahl;i++)	
	{
		delete objektliste[i];
	}
	delete [] objektliste;

	getch();
	return 0;
}