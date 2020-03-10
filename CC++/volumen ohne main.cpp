#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<math.h>

using namespace std;

class Kreis 
{
public:
void	setRadius(double);
double	getFlaeche();

private:
double dRadius;
};

class Zylinder:public Kreis
{
public:
void setAbmessungen(double, double);
double getVolumen();

private:
double	dHoehe;
};

class Rohr
{
public:
void setAbmessungen(double,double,double);
double	getVolumen(void);

private:
Zylinder Innen;
Zylinder Aussen;
};



void Kreis::setRadius(double Radius)
{
	dRadius=Radius;
}
double	Kreis::getFlaeche()
{	
	return(M_PI*(dRadius*dRadius));
}


void Zylinder::setAbmessungen(double Hoehe, double Radius)
{
		dHoehe=Hoehe;
		setRadius(Radius);

}
double Zylinder::getVolumen()
{
	return(getFlaeche()*dHoehe);
}


void Rohr::setAbmessungen(double Hoehe,double Radiusi,double Radiusa )
{
	
	Innen.setAbmessungen(Hoehe,Radiusi);
	Aussen.setAbmessungen(Hoehe,Radiusa);

}
double Rohr::getVolumen(void)
{

	return(Aussen.getVolumen()-Innen.getVolumen());
}

int main (void)
{
	



}