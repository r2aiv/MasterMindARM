/******************************************************************************/
// Program utworzony do potrzeb laboratorium IISI Systemy Wbudowane
// do wykorzystania w celach dydaktycznych podczas zajec laboratoryjnych
/******************************************************************************/


#include "include/AT91SAM7X256.H"                        /* AT91SAMT7X256 definitions */
#include "include/lib_AT91SAM7X256.h"

#include <stdio.H>                        //standard I/O
#include <math.H>                        //math library

static volatile unsigned int status_IRQ;

	
static volatile int zmienna_1;

__irq void tim0_isr (void);




//
//  Main Program
// 


int main(void) {


 	
  // Enable the Clock of the PIO
  AT91C_BASE_PMC->PMC_PCER  = (1 << AT91C_ID_PIOB);

	
	// definicja maski uzywanych wyjsc
#define OUTPUTS_MASK	(AT91C_PIO_PB20)  //uzywamy wyjscia PB20

	
/* ustawienie linii PB19 i PB 20 jako wyjscia w trybie GPIO
		przejecie kontroli przez PIO nad liniami wyjscia
		ustawienie pinow jako wyjscia
		odblokowanie mozliwosc zapisu za pomoca rejestru ODSR	
*/
	AT91C_BASE_PIOB->PIO_PER = OUTPUTS_MASK; 
	AT91C_BASE_PIOB->PIO_OER = OUTPUTS_MASK; 
	AT91C_BASE_PIOB->PIO_OWER = OUTPUTS_MASK;


#define LED_BCK_OFF		AT91C_BASE_PIOB->PIO_CODR = AT91C_PIO_PB20
#define LED_BCK_ON		AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB20

			
	
	//zabronienie obslugi wszystkich zrodel przerwan
	AT91C_BASE_AIC->AIC_IDCR = 0xffffffff;
	
	//************************************************************************************************************
	//********* Konfiguracja timera TC0 (1) oraz konfiguracja przerwan (2) zglaszanych przy doliczeniu do wartosci TC_RC  *************
	//************************************************************************************************************

	//**************************************************************
	//***************** Konfiguracja Timera ************************
	//**************************************************************
  
  AT91F_PMC_EnablePeriphClock(AT91C_BASE_PMC, 1 << AT91C_ID_TC0);  // Enable Clock for TIM0
  AT91C_BASE_TC0->TC_CCR    = AT91C_TC_CLKEN | AT91C_TC_SWTRG; //clk enable oraz Software trigger (softwareowy restart licznika)
  AT91C_BASE_TC0->TC_CMR    = 2 | AT91C_TC_CPCTRG; //2 - sterowany przez Timer_CLOCK3 = Master Clock MCK/32 ; 
	//czyli czestotliwosc zegara napedzajacego timer = 1.4975MHz (wynika to z wartosci 18.432MHz (czestotliwosc kwarcu) / 32

		//AT91C_TC_CPCTRG = compare trigger enable
  AT91C_BASE_TC0->TC_RC     = 14975;   // period is 10ms - 14975 cykli po 0.6677 us kazdy = 10000us
	
	
	AT91C_BASE_TC0->TC_IER    = AT91C_TC_CPCS; //zglaszanie przerwan Timera w reakcji na wykrycie stanu COMPARE 
	//czyli w reakcji na zrownanie sie wartosci licznika z rejestrem TC_RC

	
	//**************************************************************
	//przerwanie zostanie "podlaczone" do kontolera AIC za pomoca drugiego sposobu, tj. BEZ wykorzystania procedur biblioteki "lib_AT91SAM7X256"
	
	// Konfiguracja przerwan zglaszanych przez timer: 1. tryb pracy, 2. ustawienie wektora przerwania oraz 3. zezwolenie na przerwanie
  // TIM0 Interrupt: Mode and Vector with Lowest Priority and Enable 
	//wszystko to ustawia sie w AIC (Advanced Interrupt Controller)
  
	AT91C_BASE_AIC->AIC_SMR[AT91C_ID_TC0] = AT91C_AIC_SRCTYPE_INT_POSITIVE_EDGE | AT91C_AIC_PRIOR_HIGHEST;
  //tryb przerwania - wewnetrzne, wyzwalane zboczem (od pojawienia sie stanu przepelnienia), poziom priorytetu

	AT91C_BASE_AIC->AIC_SVR[AT91C_ID_TC0] = (unsigned long) tim0_isr; 
	//wektor przerwania TC0 wskazuje teraz na NASZA procedure obslugi 'tim0_isr'
  
	AT91C_BASE_AIC->AIC_IECR = (1 << AT91C_ID_TC0); 
	//zezwolenie na przerwanie od Timera TC0 (przerwanie zglaszane jest gdy timer doliczy do wartosci compare)

	//************************************************************************************************************
	//************************************************************************************************************
	//************************************************************************************************************
	
	
	zmienna_1=0;

	
	while(1) 
	{
		

		if ( (zmienna_1)& (1<<4) )
		{ 
			LED_BCK_ON; 
		} 
		else
		{
			LED_BCK_OFF;
		}
	};

}



// Timer0 ISR
__irq void tim0_isr (void)  {

  volatile int dummy = AT91C_BASE_TC0->TC_SR;      //  Interrupt Ack - odczytanie rejestru statusu kasuje flage zgloszenia komparacji timera
  AT91C_BASE_AIC->AIC_ICCR  = (1 << AT91C_ID_TC0);        //  Interrupt Clear Command Register - skasowanie flagi zgloszenia przerwania w AIC
	
//..................................
//tu mozemy wstawic swoje instrukcje	



	zmienna_1++;

	
//..................................

	
//tak zakonczamy przerwanie
  *AT91C_AIC_EOICR = 0;                                   // End of Interrupt
}
