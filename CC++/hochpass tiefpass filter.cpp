#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include<math.h>

class Filter
{
public:
	Filter();
	Filter(float fg,float mw,float bw);
	~Filter();
	void set_grenzfrequenz(float fg);
	float get_grenzfrequenz();
protected:
	float grenzfrequenz;
	Kennlinie F;
};


class Kennlinie
{
public:
	Kennlinie();
	void set_Parameter(float st,float ab);
	float get_ywert(float xwert);
	~Kennlinie();

private:
	float ywert,m,b;
};


class Hochpass:public Filter
{
public:
	Hochpass(float Uein,float f);
	Hochpass(float Uein,float f, float m);
	void set_Eingangsspannung(float Uein);
	float get_Ausgangspannung(float f);
	~Hochpass();

private:
float Ua,Ue;

};


class Tiefpass : public Filter
{
public:
	Tiefpass(float Uein,float f, float m);
	~Tiefpass();
	void set_Eingangsspannung(float Uein);
	float get_Ausgangspannung(float f);
private:
	float Ua,Ue;
};




Filter::Filter()
{
grenzfrequenz=0;
F.set_Parameter(0,0);

}

Filter::~Filter()
{
}

Filter::Filter(float fg,float mw,float bw)
{

	grenzfrequenz=fg;
	
	F.set_Parameter(mw,bw);
}


void Filter:: set_grenzfrequenz(float fg)
{
	grenzfrequenz=fg;
}
	
float Filter::get_grenzfrequenz()
{
	return(grenzfrequenz);
}



Kennlinie::Kennlinie()
{
	m=0;
	b=0;
	ywert=0;
}

Kennlinie::~Kennlinie()
{
}


void Kennlinie::set_Parameter(float st,float ab)
{
	m=st;
	b=ab;
}
float Kennlinie::get_ywert(float xwert)
{
	ywert=m*xwert+b;
	return(ywert);
}


Hochpass::Hochpass(float Uein,float f)
{
	Ua=0;
	Ue=Uein;
}

Hochpass::Hochpass(float Uein,float f, float m)
{ 
	Ue=Uein;
	Ua=0;
}

Hochpass::~Hochpass()
{
}

void Hochpass::set_Eingangsspannung(float Uein)
{
	Ue=Uein;
}
float Hochpass::get_Ausgangspannung(float f)
{
	if(f>=grenzfrequenz)
	{
		Ua=Ue;
	}
	Ua=Ue*F.get_ywert;
	return(Ua);
}

Tiefpass::Tiefpass(float Uein,float f, float m)
{
	Ua=0;
}

Tiefpass::~Tiefpass()
{

}

void Tiefpass:: set_Eingangsspannung(float Uein)
{
	Uein=Ue;
}
	float Tiefpass:: get_Ausgangspannung(float f)
{
	
	
	if(f<=grenzfrequenz)
	{
		Ua=Ue;
	}
	Ua=Ue*F.get_ywert;
	return(Ua);
}