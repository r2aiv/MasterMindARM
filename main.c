#include <AT91SAM7X256.H>
#include "include/lib_AT91SAM7X256.h"//for timer
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include "lcd.h"
#include "animation.h"
#include "draw.h"

/**** DEFINITION OF BOLEAN TYPE ****/
typedef int bool;
#define true 1
#define false 0

/**** DEFINITION OF COLORS ENUM ****/
enum colors {WHITE_COLOR, BLACK_COLOR, RED_COLOR, GREEN_COLOR, BLUE_COLOR, CYAN_COLOR, MAGENTA_COLOR, YELLOW_COLOR, BROWN_COLOR, ORANGE_COLOR, PINK_COLOR};

/**** DEFINITION OF GUESSING TYPE ****/
typedef int guess;
#define BAD 0
#define COLOR 1
#define GOOD 2

/**** FUNCTIONS DECLARATIONS ****/


//STATES OF PROGRAM
void helloScreen(void);
void menuScreen(void);
void gameScreen(void);
void optionScreen(void);
void highScoreScreen(void);
void winScreen(void);
void authorScreen(void);

//GAME LOGIC
void generateNumbers(void);
void resetFlagArray(void);
void clearCurentTab(void);
bool checkValues(void);
/**** END OF FUNCTIONS DECLARATION ****/

/**** GLOBAL VARIABLE DECLARATION ****/
bool flagArray[9];
int targetArray[5];
int currentArray[5] = {1, 2, 3, 4, 5};
guess ifWinArray[5];
bool randomSeedGenerated = false;

/**** LED DEFINE INIT ****/
//TODO lines 67-81 -> dead code
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

/**** SPEAKER DEFINE INIT ****/

#define SPEAKER_SET		LED_PIO->PIO_SODR = AT91C_PIO_PB19
#define SPEAKER_CLR		LED_PIO->PIO_CODR = AT91C_PIO_PB19

/**** OUTPUT DEFINE INIT ****/

#define LEFT_KEY_DOWN (((AT91C_BASE_PIOB -> PIO_PDSR) & AT91C_PIO_PB24) == 0)
#define RIGHT_KEY_DOWN (((AT91C_BASE_PIOB -> PIO_PDSR) & AT91C_PIO_PB25) == 0)
#define JOY_PUSH_LEFT (((AT91C_BASE_PIOA -> PIO_PDSR) & AT91C_PIO_PA7) == 0)
#define JOY_PUSH_RIGHT (((AT91C_BASE_PIOA -> PIO_PDSR) & AT91C_PIO_PA14) == 0)
#define JOY_PUSH_UP (((AT91C_BASE_PIOA -> PIO_PDSR) & AT91C_PIO_PA9) == 0)
#define JOY_PUSH_DOWN (((AT91C_BASE_PIOA -> PIO_PDSR) & AT91C_PIO_PA8) == 0)
#define JOY_PUSHED (((AT91C_BASE_PIOA -> PIO_PDSR) & AT91C_PIO_PA15) == 0)


#define LEFT_STICK (!((AT91C_BASE_PIOA->PIO_PDSR) & AT91C_PIO_PA7))
#define RIGHT_STICK (!((AT91C_BASE_PIOA->PIO_PDSR) & AT91C_PIO_PA14))
char ciag[100];
int zmienna_1;
volatile int zmienna_2;
static volatile unsigned int status_IRQ;


// Timer0 ISR
__irq void tim0_isr (void)  {

  volatile int dummy = AT91C_BASE_TC0->TC_SR;      //  Interrupt Ack - odczytanie rejestru statusu kasuje flage zgloszenia komparacji timera
  AT91C_BASE_AIC->AIC_ICCR  = (1 << AT91C_ID_TC0);        //  Interrupt Clear Command Register - skasowanie flagi zgloszenia przerwania w AIC
	
//..................................
//tu mozemy wstawic swoje instrukcje	
	zmienna_2++;
			if(zmienna_2 % 100 == 0){
			zmienna_1 = zmienna_1 + 1;

		}
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


int main(void){
	
	//Tu sie zaczyna dziadostwo do obslugi przerwan dla zegara
	//const unsigned char PIO_IRQ_PRIORITY = 5;

	AT91C_BASE_AIC->AIC_IDCR = 0xffffffff;

  AT91F_PMC_EnablePeriphClock(AT91C_BASE_PMC, 1 << AT91C_ID_TC0);
  AT91C_BASE_TC0->TC_CCR    = AT91C_TC_CLKEN | AT91C_TC_SWTRG;
  AT91C_BASE_TC0->TC_CMR    = 2 | AT91C_TC_CPCTRG;

  AT91C_BASE_TC0->TC_RC     = 14975;   // period is 10ms - 14975 cykli po 0.6677 us kazdy = 10000us
	
	AT91C_BASE_TC0->TC_IER    = AT91C_TC_CPCS;

	AT91C_BASE_AIC->AIC_SMR[AT91C_ID_TC0] = AT91C_AIC_SRCTYPE_INT_POSITIVE_EDGE | AT91C_AIC_PRIOR_HIGHEST;

	AT91C_BASE_AIC->AIC_SVR[AT91C_ID_TC0] = (unsigned long) tim0_isr; 
  
	AT91C_BASE_AIC->AIC_IECR = (1 << AT91C_ID_TC0); 
	//A tu sie konczy
	
	
	
	zmienna_1=0;
	zmienna_2=0;

	
	//Enable clock of the PIO
	AT91C_BASE_PMC -> PMC_PCER = (1 << AT91C_ID_PIOA);
	AT91C_BASE_PMC -> PMC_PCER = (1 << AT91C_ID_PIOB);
	
	InitSpi();
	InitLcd();
	LCDClearScreen();

	helloScreen();
}



//TODO: add visual efects. Some kind of gradient
void helloScreen(void){
	LCDClearScreen();
	LED_BCK_ON;	
	//LCDWrite130x130bmp();
	LCDPutStr("MASTERMIND", 35, 25, LARGE, BLACK, RED);
	//TODO dodac jakie wizualne efekty
	while(1)
	if(LEFT_KEY_DOWN){
		delay_ms(50);
		//changeAnimation2();
		menuScreen();
	}
}


void menuScreen(void){
	volatile int currentOption = 0;
	volatile int runOption = 0;
	zmienna_2 = 0;
	
	LCDClearScreen();
	while(1){
		
		LCDPutStr("MENU", 20, 50, SMALL, BLACK, RED);
		LCDPutStr("1.NOWA GRA", 50, 10, SMALL, BLACK, RED);
		LCDPutStr("2.NAJLEPSZE WYNIKI", 70, 10, SMALL, BLACK, RED);
		LCDPutStr("3.OPCJE", 90, 10, SMALL, BLACK, RED);
// 	LCDPutStr("4.AUTOR", 110, 10, SMALL, BLACK, RED);
		LCDPutStr("WYBIERZ", 120, 0, SMALL, BLACK, RED);
		
		switch(currentOption){
			case 0:
				//TODO: starting new game
				if(runOption){
					//changeAnimation();
					if(randomSeedGenerated == false){
						randomSeedGenerated = true;
						srand(zmienna_2);
					}
					gameScreen();
				}
				LCDSetCircle(53, 5,3, YELLOW);
				break;
			case 1:
				//TODO: opening highscores
				if(runOption)
					highScoreScreen();
				LCDSetCircle(73, 5,3, RED);
				break;
			case 2:
				//TODO: opening options
				if(runOption)
					optionScreen();
				LCDSetCircle(93, 5,3, RED);
				break;
			/*Author screen
				case 3:
				if(runOption)
					authorScreen();
				LCDSetCircle(93, 5,3, RED);
				break;
				*/
		}
		
		if(JOY_PUSH_DOWN || JOY_PUSH_RIGHT){
			if(currentOption < 2)
				currentOption++;
				delay_ms(50);
				LCDClearScreen();
			
		}
		if(JOY_PUSH_UP || JOY_PUSH_LEFT){
			if(currentOption > 0)
				currentOption--;
				delay_ms(50);
				LCDClearScreen();
		}
		if(RIGHT_KEY_DOWN)
			//return;
			helloScreen();
		
		else if(JOY_PUSHED || LEFT_KEY_DOWN){
			runOption = 1;
		}	
	}
	
}

void gameScreen (void){
	int currentOption = 0;
	
	LCDClearScreen();
	
	//INITIALIZATION
	generateNumbers();
	
	drawGameNet();
	drawCircleLine(0);
	drawCircleLine(1);
	drawCircleLine(2);
	drawCircleLine(3);
		
	LCDPutChar(currentArray[0] + '0', 110, 9, LARGE, RED, BLACK); //znak w kole
	LCDPutChar(currentArray[1] + '0', 110, 29, LARGE, RED, BLACK); //znak w kole
	LCDPutChar(currentArray[2] + '0', 110, 49, LARGE, RED, BLACK); //znak w kole
	LCDPutChar(currentArray[3] + '0', 110, 69, LARGE, RED, BLACK); //znak w kole
	LCDPutChar(currentArray[4] + '0', 110, 89, LARGE, RED, BLACK); //znak w kole

	//LCDPutStr("CZAS: xxxx", 6, 5, SMALL, BLACK, RED);
	LCDPutStr("ILOSC PROB: x", 16, 5, SMALL, BLACK, RED);
	
	drawResultDots(1);
	drawResultDots(2);
	drawResultDots(3);
	
	LCDSetCircle(124, 112, 3, RED);
	LCDSetCircle(124, 112, 2, BLUE);
	LCDSetCircle(124, 112, 1, BLUE);
	LCDSetCircle(124, 124, 3, RED);
	LCDSetCircle(118, 118, 3, RED);
	LCDSetCircle(112, 112, 3, RED);
	LCDSetCircle(112, 124, 3, RED);
	
	LCDPutChar(targetArray[0] + '0', 84, 9, LARGE, RED, BLACK);
	LCDPutChar(targetArray[1] + '0', 84, 29, LARGE, RED, BLACK);
	LCDPutChar(targetArray[2] + '0', 84, 49, LARGE, RED, BLACK);
	LCDPutChar(targetArray[3] + '0', 84, 69, LARGE, RED, BLACK);
	LCDPutChar(targetArray[4] + '0', 84, 89, LARGE, RED, BLACK);
	
	/////////////////	
	zmienna_2 = 0;
	zmienna_1 = 0;
	
	drawCircleCursor(1, currentOption);
	
	while(1){

		sprintf(ciag, "CZAS:%d  ", zmienna_1);
		LCDPutStr(ciag, 6, 5, SMALL, BLACK, RED);
		
		if(JOY_PUSH_UP)
			if((currentArray[currentOption] < 9) && (currentArray[currentOption] >= 1)){
				drawNumber(currentArray[currentOption], currentArray[currentOption] + 1, currentOption);
				currentArray[currentOption]++;	
				//LCDPutChar(currentArray[currentOption] + '0', 84, 89, LARGE, RED, BLACK);
				delay_ms(100);
			}
		
		if(JOY_PUSH_DOWN)
			if((currentArray[currentOption] <= 9) && (currentArray[currentOption] > 1)){
				drawNumber(currentArray[currentOption], currentArray[currentOption] - 1, currentOption);
				currentArray[currentOption]--;
				//LCDPutChar(currentArray[currentOption] + '0', 84, 89, LARGE, RED, BLACK);
				delay_ms(100);
			}
			
		if(JOY_PUSH_LEFT)
			if((currentOption <= 4) && (currentOption > 0)){
				drawCircleCursor(currentOption, (currentOption - 1));
				currentOption--;
				delay_ms(100);
			}
		
		
		if(JOY_PUSH_RIGHT)
			if((currentOption < 4) && (currentOption >= 0)){
				drawCircleCursor(currentOption, (currentOption + 1));
				currentOption++;
				delay_ms(100);
			}
			
		if(LEFT_KEY_DOWN){
			LCDPutStr("MENU", 20, 50, SMALL, BLACK, RED);
			if(checkValues() == true){
				delay_ms(100);
				winScreen();
			}
			else{
				//drawing result array
				//drawing last choice
			}
		}
		
		if(RIGHT_KEY_DOWN){
			//return;
			helloScreen();
			zmienna_2 = 0;
		}
	}
	
}

void winScreen(void){
	LCDClearScreen();
	while(1){
		LCDPutStr("WYGRALES", 120, 70, SMALL, BLACK, RED);
		if(RIGHT_KEY_DOWN)
		//	return;
		helloScreen();
	}
}

void generateNumbers(void){	
	int i;
	
	resetFlagArray();

	for(i = 0; i < 5; i++){
		do{
			targetArray[i] = (rand() % 9) + 1;
		}
		while(flagArray[targetArray[i]] == true);
		flagArray[targetArray[i]] = true;
	}
}

void resetFlagArray(void){
	int i;
	
	for(i = 0; i < 9; i++)
		flagArray[i] = false;
}

//TODO - implement
void drawColouredDot(int color, int x, int y){
	LCDSetCircle(x, y, 10, color);	
}

//TODO - implement
bool checkValues(void){
	int i;//, j;
	int ifWinArrayIndex = 0;
	LCDPutStr("COS", 20, 50, SMALL, BLACK, RED);
	//checking color and place of dots
	for(i = 0; i < 5; i++){
		//checking good position
		if(currentArray[i] == targetArray[i]){
			LCDPutStr("FOR", 20, 60, SMALL, BLACK, RED);
			ifWinArray[ifWinArrayIndex] = 1;
			ifWinArrayIndex++;
		}/*
		else{
			for(j = 0; j < 5; j++){
				//checking good color
				if(currentArray[i] == targetArray[j]){
					ifWinArray[ifWinArrayIndex] = COLOR;
					ifWinArrayIndex++;
				}
			}
		}*/
	}
	
	//checking targetArray
	for(i = 0; i < 5; i++){
		if(ifWinArray[i] != 1){
			LCDPutStr("ZLE", 20, 50, SMALL, BLACK, RED);
			return false;
		}
	}
	LCDPutStr("DOBRZE", 20, 50, SMALL, BLACK, RED);
	return true;
}

//TODO - implement
void clearCurentTab(void){
	
}

void highScoreScreen(void){
	LCDClearScreen();
	while(1){
		LCDPutStr("NAJLEPSZE WYNIKI", 20, 15, SMALL, BLACK, RED);
		
		if(RIGHT_KEY_DOWN)
			menuScreen();
			//return;
	}
}

void optionScreen(void){
	LCDClearScreen();
	while(1){
		LCDPutStr("OPCJE", 20, 45, SMALL, BLACK, RED);
		
		if(RIGHT_KEY_DOWN)
			menuScreen();
			//return;
	}
}

void authorScreen(void){
	LCDClearScreen();
	while(1){
		LCDPutStr("AUTOR", 20, 45, SMALL, BLACK, RED);
		LCDPutStr("KRZYSZTOF PLUCINSKI", 40, 45, SMALL, BLACK, RED);
		
		if(RIGHT_KEY_DOWN)
			menuScreen();
			//return;
	}
}

