#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>



using namespace std;

class application
{
public:
	application(int objektNr)
	{
	 this->objektNr=objektNr;
		cout <<"Loading..."<<objektNr<<"\n\n"<<endl;
	}
	~application()
	{
	 cout <<"Shut down..."<<objektNr<<"\n\n"<<endl;
	}
	void TextOut(void)
	{
	 cout <<"Running..."<<objektNr<<"\n\n"<<endl;
	}
	private:
		int objektNr;
};

int main (int argc, char* argv[])
{
	application *test;
	for(int i=0;i<5;i++)
		{
		 test = new application(i);
		 test->TextOut();
		 delete test;
		}
	getch();
	return 0;
}