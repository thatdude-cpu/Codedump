#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include<math.h>
#include<iostream>

using namespace std;

class GeoObjekt
{
public:
	GeoObjekt(int,int);
	~GeoObjekt();
	void virtual Draw(void);
protected:
int posx;
int posy;

};

GeoObjekt::GeoObjekt(int posx,int posy)
{
	this->posx=posx;
	this->posy=posy;

}

GeoObjekt::~GeoObjekt()
{
}


class Circle:public GeoObjekt
{
public:
	Circle(int,int,int);
	~Circle();
	void Draw();
protected:
	int Durchmesser;


};

Circle::Circle(int posx,int posy,int Durchmesser):GeoObjekt(posx,posy)
{
		this->posx=posx;
		this->posy=posy;
		this->Durchmesser=Durchmesser;
}

Circle::~Circle()
{
}


void Circle::Draw()
{

}

class RectAngle:public GeoObjekt
{
public:
	RectAngle(int,int,int,int);
	~RectAngle();
	void Draw();

protected:
	int Breite;
	int Hoehe;

};

RectAngle::RectAngle(int posy,int posx,int Breite,int Hoehe):GeoObjekt(posx,posy)
{
		this->posy=posy;
		this->posx=posx;
		this->Breite=Breite;
		this->Hoehe=Hoehe;
}

RectAngle::~RectAngle()
{
}

void RectAngle::Draw()
{

}

void main ()
{



}