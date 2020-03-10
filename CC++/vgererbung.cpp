#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<iostream>
#include<time.h>
using namespace std;



class Lebewesen
{
public:
	Lebewesen(char * nam);
	char *getName(void);
private:
	char name[40];
};

Lebewesen::Lebewesen(char * nam)
{
	strcpy(name,nam);
}

char* Lebewesen::getName(void)
{
	return name;
}

int zufall;
srand((unsigned) time(NULL));
zufall=(rand()%4);


class Mensch
{
public:
	Mensch(char * nam);
	void aktion(void);
	int getPunkte(void);
	~Mensch();

private:
	int punkte;
	HausTier *meinTier;
};

Mensch::Mensch(char * nam)
{
	punkte=0;
	//Haustier zufall
	cout<<"Haustier Namen eingeben:"<<endl;
	cin>>nam;
}

void Mensch::aktion(void)
{
	cout<<"Fuettern oder pflegen"<<endl;
	cin>>
	if()

	else
	if


}
int Mensch::getPunkte(void)
{

}


Mensch::~Mensch()
{
	delete meinTier;
	meinTier=NULL;
}

class HausTier
{
public:
	HausTier(char * nam);
	void fressen(double kg);
	int getGlueck(void);
	virtual void fuettern (int art,double kg) =0;
	virtual void pflegen(int art) =0;
	

protected:
	double gewicht;
	double optGewicht;
	int glueck;

};

HausTier::HausTier(char * nam)
{
	glueck=0;
}

void HausTier::fressen(double kg)
{
	gewicht+=kg;
	if(gewicht<=optGewicht)
	{
	 glueck+=2;
	}
	else
	{
		glueck--;
	}
}

int HausTier::getGlueck(void)
{
	return glueck;
}


class Wellensittich :public HausTier
{
public:
	Wellensittich(char *nam);
	virtual void fuettern(int art,double kg);
	virtual void pflegen (int art);
};

Wellensittich::Wellensittich(char *nam) :HausTier(nam)
{
	gewicht=0.02;
	optGewicht=0.2;
}

void Wellensittich:: fuettern(int art,double kg)
{
	if(art==0)
	{
	fressen(kg);
	}
	else
	{
		glueck-=4;
	}

}

void Wellensittich::pflegen (int art)
{
switch(art)	//alternativ pkt = {2,1,-3,-3} glueck+=pkt[art]
	{
	case 0: glueck+=2;
		break;
	case 1: glueck+=1;
		break;
	case 2: glueck-=3;
		break;
	case 3: glueck-=3;
		break;
	}
	gewicht-=0.01;
}

class Hamster:public HausTier
{
public:
	Hamster(char *nam);
	virtual void fuettern(int art,double kg);
	virtual void pflegen (int art);
};

Hamster::Hamster(char *nam): HausTier(nam)
{
	gewicht=0.05;
	optGewicht=0.3;
}

void Hamster::fuettern(int art,double kg)
{
	if(art==0)
	{
	fressen(kg);
	}
	else
	{
		glueck=glueck-4;
	}
}

void Hamster::pflegen (int art)
{
	switch(art)
	{
	case 0: glueck+=2;
		break;
	case 1: glueck+=1;
		break;
	case 2: glueck-=2;
		break;
	case 3: glueck+=0;
		break;
	}

	gewicht-=0.05;
}


class Katze:public HausTier
{
public:
	Katze(char *nam);
	virtual void fuettern(int art,double kg);
	virtual void pflegen (int art);
};

Katze::Katze(char *nam):HausTier(nam)
{
	gewicht=0.1;
	optGewicht=3;
}

void Katze::fuettern(int art,double kg)
{
	if(art==1)
	{
	fressen(kg);
	}
	else
	{
		glueck=glueck-4;
	}
}
void Katze::pflegen (int art)
{
	switch(art)
	{
	case 0: glueck+=3;
		break;
	case 1: glueck+=2;
		break;
	case 2: glueck-=1;
		break;
	case 3: glueck+=0;
		break;
	}

	gewicht-=0.1;
}

class Hund:public HausTier
{
public:
	Hund(char *nam);
	virtual void fuettern(int art,double kg);
	virtual void pflegen (int art);
};

Hund::Hund(char *nam):HausTier(nam)
{
	gewicht=0.2;
	optGewicht=10;
}


void Hund::fuettern(int art,double kg)
{
if(art==1)
	{
	fressen(kg);
	}
	else
	{
		glueck=glueck-4;
	}
}

void Hund::pflegen (int art)
{
	switch(art)
	{
	case 0: glueck++;
		break;
	case 1: glueck+=3;
		break;
	case 2: glueck+=3;
		break;
	case 3: glueck+=3;
		break;
	}
	gewicht-=0.5;
}


void main(void)
{
int i;

Mensch ich("meinName");
for(i=0;i<4;i++)
	ich.aktion();

printf("\nSie haben %i Punkte erreicht",ich.getPunkte());
getch();

}