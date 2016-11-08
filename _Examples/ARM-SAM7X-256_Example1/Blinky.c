/******************************************************************************/
/* BLINKY.C: LED Flasher                                                      */
/******************************************************************************/
                  
#include <AT91SAM7X256.H>                        /* AT91SAMT7X256 definitions */
#include "delay.h"

//
//  Main Program
// 
int main (void) {
		
  // Enable the Clock of the PIO
  AT91C_BASE_PMC->PMC_PCER  = (1 << AT91C_ID_PIOB);

	
	
	// ARM7X256 Leds Definition

#define LED_PIO		AT91C_BASE_PIOB			//adres kontrolera PIOB

#define LED_MASK	(AT91C_PIO_PB21|AT91C_PIO_PB20) 

	
/*LED_INIT = 	przejecie kontroli przez PIO nad liniami wyjscia
							ustawienie pinow jako wyjscia
							odblokowanie mozliwosc zapisu za pomoca rejestru ODSR
*/
	
#define LED_INIT	{LED_PIO->PIO_PER = LED_MASK; LED_PIO->PIO_OER = LED_MASK; LED_PIO->PIO_OWER = LED_MASK;}


#define LED_OFF 		LED_PIO->PIO_SODR = LED_MASK  //zgaszenie wszystkich diod
#define LED_ON 		LED_PIO->PIO_CODR = LED_MASK      //zapalenie
#define LED_TOG		LED_PIO->PIO_ODSR ^= LED_MASK     //zmiana stanu diod



#define LED_BCK_OFF		LED_PIO->PIO_CODR = AT91C_PIO_PB20
#define LED_BCK_ON		LED_PIO->PIO_SODR = AT91C_PIO_PB20

#define SPEAKER_SET		LED_PIO->PIO_SODR = AT91C_PIO_PB19
#define SPEAKER_CLR		LED_PIO->PIO_CODR = AT91C_PIO_PB19
	
	LED_INIT;
  
	

while(1)
{
	LED_BCK_ON;
	delay(1);
	LED_BCK_OFF;
	delay(3);

};	
	
	
}
