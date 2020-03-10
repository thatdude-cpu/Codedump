 #include "t89c51cc01.h"
 #include "fvs_bib_lcd.h"
 sfr p1 = 0x90;
 sfr P0 = 0x80;
 
 
void main (void)
{

   
   while(1)
   {
	  P1=P0;
   }
}