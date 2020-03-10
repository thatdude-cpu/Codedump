/*
**************************************************************************
Ferdinand von Steinbeis-Schule Reutlingen                              
                                     
C - Funktionen - für LCD - Anzeige                             04.10.2007   
(angepasst für Atmel Controller)  (ae)
**************************************************************************
*/
#include "t89c51cc01.h"
#include "fvs_bib_lcd.h"
//--------------------------------------------------------------------------

void init_lcd(void)	
		    {
				init();					//Anzeige initialisieren
				display();
 				dis_on1();
				clear();
			} 
//--------------------------------------------------------------------------

void store(void)            		
			{						//Wertübernahme mit Bit-Enabel 
               wait(); 
                 P2_4=1;
               wait();
                 P2_4=0;
               wait();
             }
//--------------------------------------------------------------------------
void clear(void)             
			{						//Anzeige löschen
              wait();
                P2=0x00;             //DB 4-7 + RS+RW+E auf 0
               store();
                P2=0x01;			   //DB 1 auf 1
              store();
               msec(1); 
             }
//--------------------------------------------------------------------------
void lcdsend(char text[81])           
		{
			char i;					//Textausgabe maximal 80 Zeichen
			i=0;
						
				while ((text[i])!=0)
				{
					zeichen(text[i]);
					i++;
				}
		}

//--------------------------------------------------------------------------
void zeichen(unsigned char z1)
           								// 1 Zeichen auf LCD Modul ausgeben
		   {
                unsigned char temp1,temp2;
				P2=0x00;
				temp1=z1;
				temp2=z1;

				temp1=temp1>>4;			  // High Bits
				temp1=temp1 & 0x0F;
				temp1=temp1 | 0x40;
				P2=temp1;
				store();

				temp2=temp2 & 0x0F;		   // Low Bits
				temp2=temp2 | 0x40;
				P2=temp2;
				store();

            }
//--------------------------------------------------------------------------
void init(void)
             {							//LCD Modul Initialisieren 
               	P2=0x00;
               	sec(1);
				 
               	P2=0x03;
               	store();
               	msec(1);
               	store();
               	msec(1);
               	store();
               	msec(1);

               	P2=0x02;			       //DL auf 0 (4 Bit Mode)
               	store();
			   	store();
			   	store();
               
		    	zeichen(0x0E);
               	zeichen(0x06);
               	zeichen(0x01);
     
			    P2_6=1;
             }
//--------------------------------------------------------------------------

void display(void)
			{		          		  
                P2=0x02;			    
              	store();             
                P2=0x08;			//Mode 08h -- Darstellungsart des Moduls
                store();
             }
//--------------------------------------------------------------------------
 
void rshift(void)
			{		          		  
                P2=0x01;			// Scroll 1 Stelle rechts    
              	store();             
                P2=0x0C;
                store();
             }

//--------------------------------------------------------------------------

void lshift(void)
			{		          		  
                P2=0x01;			// Scroll 1 Stelle links     
              	store();             
                P2=0x08;
                store();
             }

//--------------------------------------------------------------------------

void currshift(void)
			{		          		  
                P2=0x01;			// Cursor 1 Stelle rechts     
              	store();             
                P2=0x04;
                store();
             }
//--------------------------------------------------------------------------

void curlshift(void)
			{		          		  
                P2=0x01;			// Cursor 1 Stelle links     
              	store();             
                P2=0x00;
                store();
             }

//--------------------------------------------------------------------------

void dis_off(void)
			{		          		 // Display off
                P2=0x00;		     
              	store();             
                P2=0x08;
                store();
             }			 
//--------------------------------------------------------------------------

void dis_on1(void)
			{		          		 // Display on 
                P2=0x00;		     
              	store();             
                P2=0x0C;			 // Wert - 00001100b
                store();
             }
//--------------------------------------------------------------------------
 
void dis_on2(void)
			{		          		 // Display on mit Cursor
                P2=0x00;		     
              	store();             
                P2=0x0E;
                store();
             }
//--------------------------------------------------------------------------

void dis_on3(void)
			{		          		 // Display on mit blinkendem Cursor
                P2=0x00;		     
              	store();             
                P2=0x0F;
                store();
             } 
//--------------------------------------------------------------------------

void pos(unsigned char posi)  
											// Cursor Positionieren 
             {
                unsigned char wo;

				P2=0x00;
				wo=posi;
				wo=wo>>4;					// 4 x nach rechts schieben
				wo=wo & 0x07;				// Maske - 00000111b
				wo=wo | 0x08;				// ODER  - 00001000b
				P2=wo;
				store();

			    wo=posi;
				wo=wo & 0x0F;				// Maske - 00001111b
				// wo=wo | 0x00;   			//nicht unbedingt nötig?
				P2=wo;
				store();
			 }
//--------------------------------------------------------------------------

void home(void)  
			{            			  //Cursor Home (auf Stelle 0 setzen)
                P2=0x00;
             	store();
                P2=0x02;
              	store();
              	 //zeit(1);
				 wait();
             } 
//-------------------------------------------------------------------------
void wait(void)             					
				{							 	// Zeitverzögerungen 
					unsigned char tx;		 	// lokale Variable
					for (tx=150;tx!=0;tx--); 	//ca. 300µs
				}
//--------------------------------------------------------------------------
void msec(unsigned char ms)   
			  {						
			  	unsigned char t1; 				//warte n Millisekunden	
				unsigned char t2;				//lokale Variable
				
				for (t1=ms;t1!=0;t1--)			//Anzahl ms * 1 Millisekunde
				{
					for (t2=249;t2!=0;t2--); 	//ca. 500µs
					for (t2=249;t2!=0;t2--); 	//ca. 500µs
				}

              }
//--------------------------------------------------------------------------

void sec(unsigned char s)   
		{						
		  	unsigned char s1; 				//warte n Sekunden	
			unsigned char s2;				//lokale Variable
			unsigned char s3;				//lokale Variable
			unsigned char s4;				//lokale Variable
				   				
				for (s1=s;s1!=0;s1--)			//Anzahl * 1 Sekunde
				{
					for (s2=20;s2!=0;s2--) 		  //ca. 20 x 50000 µs
					{
						for (s3=100;s3!=0;s3--)	 	//ca. 100 x 500µs
						{
							for (s4=250;s4!=0;s4--); 	//ca. 500µs
						}
					}
				}

        }
//--------------------------------------------------------------------------