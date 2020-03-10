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

class Sparkonto:public Konto
{
private:
	double ZinsSatz;
	
public:
	Sparkonto();
	Sparkonto(double,double);
	 ~Sparkonto();
	 void zinsberechnen(int);

};


int main (void)
{
	/* Kontostand 1
	Konto Eva_Miller;
cout <<"Kontostand"<<Eva_Miller.getkontostand;
cout<<"Einzahlen: 300";
Eva_Miller.einzahlen(300);
cout<<"Abheben 400";
Eva_Miller.abheben;
cout <<"Kontostand"<<Eva_Miller.getkontostand;
cout<<"Einzahlen: 300";
Eva_Miller.einzahlen(300);
cout<<"Kontostand"<<Eva_Miller.getkontostand;
*/


Sparkonto Werner_Maus;
cout << "Kontostand ="<<Werner_Maus.getkontostand()<<endl;
cout <<"Einzahlen: 100" <<endl;
Werner_Maus.einzahlen(100);
cout << "Kontostand ="<<Werner_Maus.getkontostand()<<endl;
cout << "Zinsberechnung (Tage): 100";
Werner_Maus.zinsberechnen(100);
cout << "Kontostand ="<<Werner_Maus.getkontostand()<<endl;
cout <<"Einzahlen: 300" <<endl;
Werner_Maus.einzahlen(300);
cout << "Kontostand ="<<Werner_Maus.getkontostand()<<endl;
cout << "Zinsberechnung (Tage): 200";
Werner_Maus.zinsberechnen(200);
cout << "Kontostand ="<<Werner_Maus.getkontostand()<<endl;
cout << "abheben : 500"<<Werner_Maus.abheben(500)<<endl;
cout << "Kontostand ="<<Werner_Maus.getkontostand()<<endl;
cout << "abheben : 400"<<Werner_Maus.abheben(400)<<endl;
cout << "Kontostand ="<<Werner_Maus.getkontostand()<<endl;
getch();
}

Konto::Konto()
{
 Kontostand=0.0;
}

Konto::~Konto()
{
}

Konto::Konto(double L_Kontostand)
{
 Kontostand=L_Kontostand;
}

double Konto::getkontostand(void)
{
	return Kontostand;
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
	return(1);
	}
	else
	{
		return(0);
	}

}



Sparkonto::Sparkonto()
{
 ZinsSatz=0.05;
}


Sparkonto::~Sparkonto()
{

}

Sparkonto::Sparkonto(double Kontostand,double z_zinssatz)
	:Konto(Kontostand)
{
	ZinsSatz=z_zinssatz;
}

void Sparkonto::zinsberechnen(int tage)
	
{
	einzahlen((getkontostand()/100)*ZinsSatz*tage);
}