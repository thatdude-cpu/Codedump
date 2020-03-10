#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include<iostream>

using namespace std;

class Kreis
{
public:
	Kreis();
	Kreis(double radius);
	int getAnzObjekte();
	void setRadius(double radius);
	double getRadius();
	double getUmfang();
	double getFlaeche();
	~Kreis();

private:
	int anzObjekte;
		double radius;
		double flaeche;
	double umfang;
};

Kreis::Kreis()
{
	radius=0;
	getUmfang();
	getFlaeche();
		
	anzObjekte++;
}
Kreis::Kreis(double radius)
{
	this->radius=radius;
	getUmfang();
	getFlaeche();

}
Kreis::~Kreis()
{
}

int Kreis::getAnzObjekte()
{
}
	void Kreis::setRadius(double radius)
		{
			if(this->radius!=radius)
			{
			this->radius=radius;
			getUmfang();
			getFlaeche();
			}
			
	}
	double Kreis::getRadius()
		{
			
			return(radius);
			this->radius=radius;
		}
	double Kreis::getUmfang()
		{
			umfang=2*radius*3.14;
			return(umfang);	
	}
	double Kreis::getFlaeche()
		{
			flaeche=radius*radius*3.14;
			return(flaeche);
	}



class Kegel:public Kreis
{
public:
	Kegel();
	Kegel(double radius, double hoehe);
	void setAbmessungen(double radius, double hoehe);
	double getVolumen();
	~Kegel();

protected:
	double hoehe;
	double volumen;

};

Kegel::Kegel()
{
	setRadius(0);
}

Kegel::Kegel(double radius, double hoehe)
{
	setRadius(radius);
	this->hoehe=hoehe;
	getUmfang();
	getFlaeche();
}
Kegel::~Kegel()
{
}
void Kegel::setAbmessungen(double radius, double hoehe)
{
	setRadius(radius);
	this->hoehe=hoehe;
	getUmfang();
	getFlaeche();
}

double Kegel::getVolumen()
{
	return(0.33*getFlaeche()*hoehe);
}

class Zylinder
{
public:
	Zylinder();
	Zylinder(double radius,double hoehe);
	void setAbmessungen(double radius,double hoehe);
	double getVolumen();
	~Zylinder();

protected:
	double hoehe;
	double volumen;
};

Zylinder::Zylinder()
{
}

Zylinder::~Zylinder()
{
}

int main (void)
{
	Kreis obj1;
	cout <<obj1.getAnzObjekte();
	obj1.setRadius(2);
	cout << obj1.getFlaeche();
	cout << obj1.getUmfang();

	Zylinder *z1 = new Zylinder;
	Zylinder *z2 = new Zylinder;
	Zylinder *z3 = new Zylinder;
	Kegel *k1 = new Kegel;
	Kegel *k2 = new Kegel;
	cout << k1->getAnzObjekte();
	cout << z1->getVolumen();
	cout << z2->getVolumen();
	cout << z3->getVolumen();
	cout << k1->getVolumen();
	cout << k2->getVolumen();
	delete k1;
	delete k2;
	delete z1;
	delete z2;
	delete z3;
	cout << k1->getAnzObjekte();


}