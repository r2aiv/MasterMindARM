/******************************************************************************/
/* Example0  */
/******************************************************************************/
/******************************************************************************/
                  
#include "include\AT91SAM7X256.H"                        /* AT91SAMT7X256 definitions */
#include <inttypes.h>	 //typy jednolite danych
	
volatile uint8_t zmienna_1, zmienna_2, zmienna_3; //definicje zmiennych globalnych
	

int main (void) 
	{
	
		
	while(1)
	{
		
		zmienna_1++;
		zmienna_2+=2;
		zmienna_3 ^=0x10;
	};	


}

