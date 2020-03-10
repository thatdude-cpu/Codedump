#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<conio.h>

using namespace std;

class Temperatur
{
public:
	void   setKelvin(double t);
	double getKelvin();
	double getCelsius(double t);
	double getFahrenheit(double t);


private:
	double kelvin;
	double celsius;
	double fahrenheit;
};

void Temperatur::setKelvin(double t)
{
	if(t>=0) 
	{
		kelvin=t;	
	}
	else
	{
		kelvin=0;
	}
}

double Temperatur::getKelvin()
{
	return kelvin;
}

double Temperatur::getCelsius(double t)
{
	
	

	
	return kelvin+273.15;;

}

double Temperatur::getFahrenheit(double t)
{

	return kelvin+(459.67)*(5/9);

}

int main (void)
{
	Temperatur meinTemp;
	double t=0;
	cout <<"\nWert eingeben:";
	cin >>t;
	meinTemp.setKelvin(t);
	cout <<"\nTemperatur in Kelvin:"<<meinTemp.getKelvin;
	cout <<"\nTemperatur in Celsius:"<<meinTemp.getCelsius;
	cout <<"\nTemperatur in FAhrenheit:"<<meinTemp.getFahrenheit;
	
	
	return 0;

}

