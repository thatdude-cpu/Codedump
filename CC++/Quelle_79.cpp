#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>



using namespace std;

class application
{
public:
	application()
	{
	 cout <<"Loading...\n\n" <<endl;
	}
	~application()
	{
	 cout <<"Shut down...\n\n"<<endl;
	}
	void TextOut(void)
	{
	 cout <<"Running...\n\n"<<endl;
	}
	private:

};

int main (int argc, char* argv[])
{
	application *test =  new application;
	test->TextOut();
	delete test;
	getch();
	return 0;
}