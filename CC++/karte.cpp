#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include<iostream>
#include<math.h>
#include<windows.h>
#include<time.h>
//Marius Weinhardt
using namespace std;

class Karte
{
public:
	Karte(char b, char f);
	void karteAnzeigen();
	char getFarbe();
	char getBild();
private:
	char bild; 
	char farbe;
};

Karte::Karte(char b, char f)
{
	bild=b;
	farbe=f;
}

void Karte::karteAnzeigen()
{

if(farbe==0)
{
	cout<<"Kreuz"<<endl;
}
else
{
if(farbe==1)	
{
	cout<<"Pik"<<endl;
}
else
{
if(farbe==2)
{
	cout<<"Herz"<<endl;
}
else
{


if(farbe==3)
{
	cout<<"Karo"<<endl;
}

}
}
}

if(bild==0)
{
	cout<<"7-er"<<endl;
}
else
{
if(bild==1)
{
	cout<<"8-er"<<endl;
}
else
{
if(bild==2)
{
	cout<<"9-er"<<endl;
}
else
{
if(bild==3)
{
	cout<<"10-er"<<endl;
}
else
{
if(bild==4)
{
	cout<<"Bube"<<endl;
}
else
{
if(bild==5)
{
	cout<<"Dame"<<endl;
}
else
{
if(bild==6)
{
	cout<<"Koenig"<<endl;
}
else
{
if(bild==7)
{
	cout<<"Ass"<<endl;
}
}
}
}
}
}
}
}

}
char Karte::getBild()
{
	return(bild);
}

char Karte::getFarbe()
{
	return (farbe);
}


class KartenStapel
{
public:
	KartenStapel();
	Karte * obersteKarte();
	Karte * obersteKarteWeg();
	void karteDrauflegen(Karte *k);
	void mischen();

protected:
	int anzKarten;
	Karte * karten[32];

};

KartenStapel::KartenStapel()
{
	anzKarten=0;
	
	for(int i=0;i!=32;i++)
	{
		karten[i]=NULL;
	}
}


Karte *KartenStapel::obersteKarte()
{
	return karten[anzKarten-1];
}


Karte *KartenStapel::obersteKarteWeg()
{
	
	return karten[anzKarten-1];
	karten[anzKarten-1]=NULL;
	anzKarten--;
}

void KartenStapel::karteDrauflegen(Karte *k)
{
	karten[anzKarten]=k;
	anzKarten++;
}

void KartenStapel::mischen()
{
	srand ((unsigned)time (NULL) );
	int r1;
	int r2;
	Karte *buffer;
	for(int i=0;i!=64;i++)
	{
		
		r1=(rand()%32);
		r2=(rand()%32);
		
		buffer=karten[r1];
		karten[r1]=karten[r2];
		karten[r2]=buffer;


	}


}


class Spieler:public KartenStapel
{
public:
	Spieler(char *nam,KartenStapel * off,KartenStapel* stapel);
	void karteWegGeben(int nr,KartenStapel*wohin);
	bool spielZug();
	void namenSagen();
private:
	char name [40];
	KartenStapel* offen;
	KartenStapel* stapel;
};

Spieler::Spieler(char *nam,KartenStapel * off,KartenStapel* stapel)
{
	offen=off;
	this->stapel=stapel;
	strcpy(name,nam);
}

void Spieler::karteWegGeben(int nr,KartenStapel*wohin)
{
		
		wohin->karteDrauflegen(karten[nr]);

		for(int i=0;i<anzKarten-1;i++)
		{
			karten[nr]=karten[nr+1];
			anzKarten--;
			karten[anzKarten]=NULL;
		}

}

bool Spieler::spielZug()
{
	char gesuchtefarbe,gesuchstesbild;
	int i =0;
	bool passendekarte =false,gewonnen=false;

	gesuchtefarbe= offen->obersteKarte()->getFarbe();
	gesuchtesbild =;



}

void Spieler::namenSagen()
{
	cout<<"Spieler: "<<name<<endl;
}


int main(void)
{

	Spiel mauMau(4);
	mauMau.kartenVerteilen();
	mauMau.spielDurchfuehren();
	getch();
	return 0;
}




































