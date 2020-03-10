 #include "t89c51cc01.h"
 #include "fvs_bib_lcd.h"
   #define Blau 0x10;
	 #define Gruen 0x22;
 	 #define Gelb 0x45;


void main (void)
{

   P1=0;
   while(1)
   {
		 
	   if(!P3_2)
		 {
		 P1=Gelb;
		 }
		 else{ 
		 if (!P3_3)
		 {
		 P1=Gruen;
     }			 
	 
		 else{
		 if(P3_4) 
		 {
			 P1=Blau;
		 }
	 }  
 }	 
   }
}