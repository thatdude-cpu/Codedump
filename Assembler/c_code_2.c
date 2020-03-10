 #include "t89c51cc01.h"
 #include "fvs_bib_lcd.h"

void main (void)
{

	unsigned char wert=0;
	int z=0x3F;
  ADCLK=5;
	ADCF=0x80;
	ADCON=0x2F;
  P1=0;
	init_lcd();
	while(1)
   {
		
		
		 if(ADCON&0x10)
		 {
			 home();
			 wert=ADDH/10+'A';
		 	 ADCON=0x2F;
			 zeichen(wert);
		
		 if(P3_4)
		 {
		 z++;
		 pos(z);
		 zeichen(wert);
		sec(1);
		

			}
//			if(z==0xF)
//			{
//			z=0x3F;
//		}		
}
		
		
}
	}