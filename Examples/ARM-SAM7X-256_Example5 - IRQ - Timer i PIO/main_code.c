/******************************************************************************/
// Program utworzony do potrzeb laboratorium IISI Systemy Wbudowane
// do wykorzystania w celach dydaktycznych podczas zajec laboratoryjnych
/******************************************************************************/


#include "include/AT91SAM7X256.H"                        /* AT91SAMT7X256 definitions */
#include "include/lib_AT91SAM7X256.h"

#include <stdio.H>                        //standard I/O
#include <math.H>                        //math library
#include "lcd.h"
#include "adc.h"
#include "delay.h"

extern void InitSpi(void);
extern void InitLcd(void);
extern void LCDClearScreen(void);

extern void WriteSpiCommand(unsigned int data);
extern void WriteSpiData(unsigned int data);

extern void LCDClearScreen(void);
extern void LCDSetXY(int x, int y);
extern void LCDSetPixel(int  x, int  y, int  color);
extern void LCDSetLine(int x1, int y1, int x2, int y2, int color);
extern void LCDSetRect(int x0, int y0, int x1, int y1, unsigned char fill, int color);
extern void LCDSetCircle(int x0, int y0, int radius, int color);
extern void LCDPutChar(char c, int  x, int  y, int size, int fcolor, int bcolor);
extern void LCDPutString (char *lcd_string, const char *font_style, unsigned char x, unsigned char y,
                   unsigned char fcolor, unsigned char bcolor);
extern void LCDPutStr(char *pString, int  x, int  y, int Size, int fColor, int bColor);
extern void Delay (unsigned long a);


char ciag[100];

static volatile unsigned int status_IRQ;

	
	// ARM7X256 Leds Definition


//maska dla pinow do ktorych podlaczone sa diody, którymi bedziemy sterowac
//#define LEDS_MASK	(AT91C_PIO_PB30 | AT91C_PIO_PB20 | AT91C_PIO_PB21)
	

//konfiguracja wybranych linii PIO
												  //linie pracuja jako GPIO sterowane przez uzutkownika (a nie przez peryferia) = PIO_PER
												  //ustawienie pinow jako wyjscia = PIO_OER
												  //odblokowanie mozliwosc zapisu za pomoca rejestru ODSR = PIO_OWER
//#define LED_INIT	{AT91C_BASE_PIOB->PIO_PER = LEDS_MASK; AT91C_BASE_PIOB->PIO_OER = LEDS_MASK; AT91C_BASE_PIOB->PIO_OWER = LEDS_MASK;}


	
 



//Inputs

#define LEFT_STICK (!((AT91C_BASE_PIOA->PIO_PDSR) & AT91C_PIO_PA7))
#define RIGHT_STICK (!((AT91C_BASE_PIOA->PIO_PDSR) & AT91C_PIO_PA14))


static volatile int zmienna_1, zmienna_2;





// Timer0 ISR
__irq void tim0_isr (void)  {

  volatile int dummy = AT91C_BASE_TC0->TC_SR;      //  Interrupt Ack - odczytanie rejestru statusu kasuje flage zgloszenia komparacji timera
  AT91C_BASE_AIC->AIC_ICCR  = (1 << AT91C_ID_TC0);        //  Interrupt Clear Command Register - skasowanie flagi zgloszenia przerwania w AIC
	
//..................................
//tu mozemy wstawic swoje instrukcje	
	zmienna_2++;
//..................................

//tak zakonczamy przerwanie
  *AT91C_AIC_EOICR = 0;                                   // End of Interrupt
}



 



__irq void pioIsr(void) 
{
		//kopiujemy zawartosc rejestru PIO_ISR do zmiennej status
		status_IRQ = AT91C_BASE_PIOA->PIO_ISR;
		//teraz flagi w PIO_ISR sa juz skasowane
		
	//..................................
	//kod uzytkownika

		
		//przechodzimy do analizy zawartosci zmiennej status
		//przykladowo:

	if (status_IRQ & (1<<7)) //jesli PA7 wywolalo przerwanie... (dowolna zmiana stanu na linii PA7 = wcisniecie lub puszczenie klawisza)
		{
			//kod dla zmiany PA7
			if (LEFT_STICK) //dodatkowo sprawdzamy, czy aktualny stan to wcisniety klawisz? (czyli zdarzenie, które wystapilo to wcisniecie)
			{
				zmienna_1--;

			}
		}
		if (status_IRQ & (1<<14)) //jesli PA14 wywolalo przerwanie... (dowolna zmiana stanu na linii PA8 = wcisniecie lub puszczenie klawisza)
		{
			//kod dla PA8...
			zmienna_1++;
		}
	//..................................


 //tak zakonczamy przerwanie
	AT91F_AIC_AcknowledgeIt(AT91C_BASE_AIC);
}





//
//  Main Program
// 


int main(void) {


const unsigned char PIO_IRQ_PRIORITY = 5;

	
  // Enable the Clock of the PIO
  AT91C_BASE_PMC->PMC_PCER  = (1 << AT91C_ID_PIOB) | (1 << AT91C_ID_PIOA);
	
	//zabronienie obslugi wszystkich zrodel przerwan
	AT91C_BASE_AIC->AIC_IDCR = 0xffffffff;
	
//	LED_INIT;

	//AT91C_BASE_PIOB->PIO_ODR = (AT91C_PIO_PB24|AT91C_PIO_PB25); //keys SW1 i SW2 - jako wejscia

	
	
  
  // Initialize SPI interface to LCD
   InitSpi();

  // Init LCD
   InitLcd();


	// clear the screen
   LCDClearScreen();





	
	
	

	

  // draw a filled box
  LCDSetRect(10, 10, 40, 40, FILL, RED);
  LCDSetRect(10, 50, 40, 80, FILL, GREEN);
  LCDSetRect(10, 90, 40, 120, FILL, BLUE);


  // draw a string
  LCDPutStr("Interrupts", 15, 15, LARGE, PINK, BLACK);




			WriteSpiCommand(VOLCTR);
			WriteSpiData(40);    // P1 = 44  volume value (experiment with this value to get the best contrast)
			WriteSpiData(3);   // P2 = 3   resistance ratio (only value that works)

	
	
	

	
	

	//************************************************************************************************************
	//************* konfiguracja przerwan zglaszanych od jednostki PIOA ******************************************
	//************************************************************************************************************
	
	//przerwanie od PIOA - bedzie zglaszane przy zmianie stanu wybranych wejsc (wychylenie joysticka w lewo lub w prawo) - czyli zmianie zawartosci rejestru kontrolera AT91C_BASE_PIOA->PIO_PDSR
	AT91C_BASE_PIOA->PIO_IDR = 0xffffffff; //na wszelki wypadek wylaczenie zezwolen przerwan od wszyskich linii PIOA (gdyby przypadkowo byly wczesniej zezwolone)

	//przerwanie zostanie "podlaczone" do kontolera AIC za pomoca pierwszego sposobu, tj. z wykorzystaniem procedur biblioteki "lib_AT91SAM7X256"
	//funkcja biblioteki "lib_AT91SAM7X256" podlaczajaca procedure obslugi przerwania do systemu przerwan AIC
	AT91F_AIC_ConfigureIt ( AT91C_BASE_AIC , AT91C_ID_PIOA ,	PIO_IRQ_PRIORITY , 
		AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL , (void*) pioIsr);



	AT91C_BASE_PIOA->PIO_IER = (1<<14) | (1<<7); //przerwania beda generowane w reakcji na zmiane stanu linii PA7 i PA14
	//PA7 = left, PA8 = down, PA9 = UP, PA14 = PRAWO, AP15 = CLICK

	//************************************************************************************************************



	status_IRQ = AT91C_BASE_PIOA->PIO_ISR; //odczyt statusu PIOA - spowoduje to skasowanie ew. zgloszonych wczesniej przerwan
	//AT91F_AIC_AcknowledgeIt(AT91C_BASE_AIC); //potwierdzenie ew. wczesniej zgloszonych przerwan w AIC
	
	AT91F_AIC_EnableIt (AT91C_BASE_AIC, AT91C_ID_PIOA); //zezwolenie na generacje przerwan dla calego PIOA

	//************************************************************************************************************
	//************************************************************************************************************
	//************************************************************************************************************







	//************************************************************************************************************
	//********* Konfiguracja timera TC0 oraz przerwan zglaszanych przy doliczeniu do wartosci TC_RC  *************
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
	zmienna_2=0;

	
	while(1) 
	{
		sprintf(ciag, "zm_1=%d  ", zmienna_1);
		LCDPutStr(ciag, 60, 10, LARGE, BLACK, YELLOW);

		sprintf(ciag, "zm_2=%d  ", zmienna_2);
		LCDPutStr(ciag, 80, 10, LARGE, BLACK, YELLOW);

	};

}


