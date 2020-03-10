#include "t89c51cc01.h"
#include "fvs_bib_lcd.h"
void warten(unsigned int msec);

void main (void)
{
	unsigned int z=0;
  unsigned int msec =200; 
	char t[8];
	
   while(1)
   {
	  P1=0x5F;
		 
warten(msec);
	

P1=0;
	
warten(msec);



init_lcd();
		 pos(0x00);
		lcdsend(t[8]); 

	}




}
	

	void warten(unsigned int msec)
	{
		unsigned char zeit=0;
			msec=msec*2;
		for(;msec>0;msec--)
		

	for(zeit=239;zeit>0;zeit--)
		;
	
		
	}
