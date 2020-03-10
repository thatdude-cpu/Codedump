#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

class Kreis
{
public:
	Kreis();
	Kreis(int);
	~Kreis();
	void scan(void);
	void print(void);
	void set_Radius(void);
	void get_Umfang(void);
	void get_Flaeche(void);

private:
	double r,u,f;
};

int main (void)
{
/* c
Kreis *test02;
test02=new Kreis;
test02->scan();
test02->print();
*/

/*b
Kreis test01;
test01.scan();
test01.print();
*/


/*d
Kreis *test03;
test03=new Kreis;
cin >> r;
test03->set_Radius();
test03->get_Umfang();
cout <<"Umfang %d"<<u;
test03->get_Flaeche();
cout<< "Flaeche %d<<f;

Kreis test04;
cin >> r;
test04.set_Radius();
test04.get_Umfang();
cout <<"Umfang %d"<<u;
test04.get_Flaeche();
cout<< "Flaeche %d<<f;



*/

/*e
Kreis *test05;

test05(5)=new Kreis;

#test05->get_Umfang();
test05->get_Flaeche();

Kreis test06 (5);
test06.get_Umfang();
test06.get_Flaeche();




*/




}

Kreis::Kreis(int l_radius)
{
	r=l_radius;

}

Kreis::Kreis()
{
}

Kreis::~Kreis()
{
}

void Kreis::scan(void)
{	

	cout << "Radius eingeben" <<endl;
	cin>>r;

}

void Kreis::print(void)
{

u=r*2*M_PI;
f=r*r*M_PI;

 cout<< "Umfang &d" <<u;
 
 
 cout<< "Fläche %d" <<f;


}

void Kreis::set_Radius(void)
{
	
	r=r;
}

void Kreis::get_Umfang(void)
{
	u=r*r*M_PI;

}
void Kreis::get_Flaeche(void)
{
	f=r*2*M_PI;
}