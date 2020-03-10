#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<conio.h>

using namespace std;



class Schiff
{
public:
	Schiff(char * name,char* land,int maxkap,Hafen* h);
	~Schiff();

private:
	char name[50];
	char land[50];
	char * getName();
	char* getLand();
	int getMaxKAp();
	Hafen* getHafen();
	bool anlegen(Hafen* h);
	bool ablegen();
	bool setKaptiaen(Kapitaen *k);
    bool removeKapitaen();
	void=0 printStatus();

protected:
	int maxkap;
	Kapitaen*k;
	Hafen*h;
};

Schiff::Schiff(char * name,char* land,int maxkap,Hafen* h)
{
	this->name=name;
	this->land=land;
	this->maxkap=maxkap;
	this->h=h;
	this->k=NULL;

	h.anlegen();
}

Schiff::~Schiff()
{
}


class Passagierschiff : public Schiff
{
public:
	Passagierschiff(char* name,char* land,int maxkap, Hafen * h);
	~Passagierschiff();
	bool einsteigen(int passagiere);
	bool austeigen(int passsagiere);
	void printStatus();
private:
	int passagiere;
};

Passagierschiff::Passagierschiff(char* name,char* land,int maxkap, Hafen * h)
{
}

Passagierschiff::~Passagierschiff()
{
}



class Frachtschiff :public Schiff

{
public:
	Frachtschiff
(char* name,char* land,int maxkap, Hafen * h);
	~Frachtschiff
();
		bool beladen(int tonnen);
		bool entladen (int tonnen);
		void printStatus();
private:
	int ladung;
};

Frachtschiff::Frachtschiff
(char* name,char* land,int maxkap, Hafen * h)
{
}

Frachtschiff::~Frachtschiff
()
{
}




class Kapitaen

{
public:
	Kapitaen
(char* name);
	~Kapitaen
();
char* getName();
void bekommtSchiff(Schiff *s);
bool verlaesstSchiff();

private:
char name[50];
Schiff *s;
};

Kapitaen
::Kapitaen
(char* name)
{
	this->name=name;
	this->s=NULL;

}

Kapitaen
::~Kapitaen
()
{
}

char* Kapitaen::getName()
{
	return name;
}

void Kapitaen::bekommtSchiff(Schiff *s)
{
	if(this->s==NULL)
	{
		if(setKapitaen(Kapitaen k))
		
		this->s=s;
	}
}





class Hafen
{
public:
	Hafen(char* name, int anlegeplaetze);
	char* getName();
	bool anlegen(Schiff*s);
	bool ablegen (Schiff*s);
	void printSchiffsliste();
	~Hafen();

private:
	{<2,<=10}int anlegeplaetze;
	char name[50];
};

Hafen::Hafen(char* name, int anlegeplaetze)
{
}

Hafen::~Hafen()
{
}