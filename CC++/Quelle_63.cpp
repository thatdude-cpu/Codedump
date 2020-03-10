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
	application *objektliste[5];
	cout <<"Erstelle 5 Instanzen"<<endl;

	for(int i=0;i<5;i++)
	{
		objektliste [i] = new application(i);
	}

	cout <<"Rufe ausgabe funktion auf"<<endl;
	for(int i=0;i<5;i++)
	{		 
		objektliste[i]->TextOut();
	}
	for(int i=0;i<5;i++)	
	{
		delete objektliste[i];
	}


	getch();
	return 0;
}