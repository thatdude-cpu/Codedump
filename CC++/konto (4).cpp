#include<conio.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>

using namespace std;

class Konto
{
public:
	Konto();
	~Konto();
	Konto(double);
	double getkontostand(void);
	void einzahlen(double);
	int abheben(double);

private:
	double Kontostand;
};


int main (void)
{
	double kontostand;

Konto Eva_Miller;
cout<<"Kontostand ="	<<Eva_Miller.getkontostand();
cout<<"\nEinzahlen  :300";
Eva_Miller.einzahlen(300);
cout<<"\nAbheben    :400";
int err=Eva_Miller.abheben(400);
if(err)
{
	cout<<" Konto kann nicht ueberzogen werden, keine Geldausgabe!!!"<<endl;
}
cout <<"
	Kontostand ="	<<Eva_Miller.getkontostand();
cout<<"\nEinzahlen  :300";
Eva_Miller.einzahlen(300);
cout<<"\nKontostand ="	<<Eva_Miller.getkontostand();
getch();
}

Konto::Konto()
{
Kontostand=0.0;
}

Konto::Konto(double L_kontostand)
{
	Kontostand=L_kontostand;
}
Konto::~Konto()
{
}



double Konto::getkontostand(void)
{
	return Kontostand= this-> Kontostand;;
}
void  Konto::einzahlen(double e_wert)
{
  
   Kontostand=Kontostand+e_wert;
}
int  Konto::abheben(double a_wert)
{

	if(a_wert<Kontostand)
	{
	Kontostand=Kontostand-a_wert;
	return(0);
	}
	else
	{
		return(1);
	}

}

