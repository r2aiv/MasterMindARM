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

/**** DEFINITION OF DIFFICULT ENUM ****/
enum difficult {EASY, NORMAL, HARD};

/**** DEFINITION OF GUESSING TYPE ****/
typedef int guess;
#define BAD 0
#define BAD_POSITION 1
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
bool checkValues(void);
void clearAfterGame(void);
void clearChooseHistoryArray(void);
void saveChoosenValues(void);
void clearResultHistoryArray(void);
void saveResultValues(void);
/**** END OF FUNCTIONS DECLARATION ****/

/**** GLOBAL VARIABLE DECLARATION ****/
bool flagArray[10];
int targetArray[5];
int currentArray[5] = {1, 1, 1, 1, 1};
guess ifWinArray[5];
bool randomSeedGenerated = false;
char ciag[100];
int gameTime;
volatile int globalTime;
static volatile unsigned int status_IRQ;
bool fromReturn = false;
int chooseHistoryArray[3][5];
guess resultHistoryArray[3][5];
int atempts = 5;
int difficult;
bool devMode = false;
int score = 0;

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

// Timer0 ISR
__irq void tim0_isr (void)  {

  volatile int dummy = AT91C_BASE_TC0->TC_SR;      //  Interrupt Ack - odczytanie rejestru statusu kasuje flage zgloszenia komparacji timera
  AT91C_BASE_AIC->AIC_ICCR  = (1 << AT91C_ID_TC0);        //  Interrupt Clear Command Register - skasowanie flagi zgloszenia przerwania w AIC
	
	globalTime++;
			if(globalTime % 100 == 0){
			gameTime = gameTime + 1;

		}

  *AT91C_AIC_EOICR = 0;                                   // End of Interrupt
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
	
	gameTime=0;
	globalTime=0;
	
	//Enable clock of the PIO
	AT91C_BASE_PMC -> PMC_PCER = (1 << AT91C_ID_PIOA);
	AT91C_BASE_PMC -> PMC_PCER = (1 << AT91C_ID_PIOB);
	
	InitSpi();
	InitLcd();
	LCDClearScreen();
	clearChooseHistoryArray();
	clearResultHistoryArray();

	helloScreen();
}

void helloScreen(void){
	LCDClearScreen();
	LED_BCK_ON;	

	while(1){
		if(fromReturn == true){
			fromReturn = false;
			LCDClearScreen();
		}
		LCDPutStr("MASTERMIND", 35, 25, LARGE, BLACK, RED);
		if(LEFT_KEY_DOWN){
			delay_ms(100);
			//changeAnimation2();
			menuScreen();
		}
	}
}


void menuScreen(void){
	volatile int currentOption = 0;
	volatile int runOption = 0;
	globalTime = 0;
	
	LCDClearScreen();
		
	while(1){	
		if(fromReturn == true){
			fromReturn = false;
			LCDClearScreen();

		}

		LCDPutStr("MENU", 20, 50, SMALL, BLACK, RED);
		LCDPutStr("1.NOWA GRA", 50, 10, SMALL, BLACK, RED);
//	LCDPutStr("2.NAJLEPSZE WYNIKI", 70, 10, SMALL, BLACK, RED);
		LCDPutStr("2.AUTOR", 70, 10, SMALL, BLACK, RED);
		LCDPutStr("3.OPCJE", 90, 10, SMALL, BLACK, RED);

		LCDPutStr("WYBIERZ", 120, 0, SMALL, BLACK, RED);
		
		switch(currentOption){
			case 0:
				if(runOption){
					//changeAnimation();
					if(randomSeedGenerated == false){
						randomSeedGenerated = true;
						srand(globalTime);
					}
					runOption = 0;
					gameScreen();
				}
				LCDSetCircle(53, 5,3, YELLOW);
				LCDSetCircle(73, 5,3, BLACK);
				LCDSetCircle(93, 5,3, BLACK);
				break;
				
			case 1:
				if(runOption){
					runOption = 0;
					authorScreen();
				}
				LCDSetCircle(53, 5,3, BLACK);
				LCDSetCircle(73, 5,3, RED);
				LCDSetCircle(93, 5,3, BLACK);
				break;
				
			/*	
			case 1:
				if(runOption){
					runOption = 0;
					highScoreScreen();
				}
				LCDSetCircle(53, 5,3, BLACK);
				LCDSetCircle(73, 5,3, RED);
				LCDSetCircle(93, 5,3, BLACK);
				break;
				*/
			case 2:
				if(runOption){
					runOption = 0;
					optionScreen();
				}
				LCDSetCircle(53, 5,3, BLACK);
				LCDSetCircle(73, 5,3, BLACK);
				LCDSetCircle(93, 5,3, RED);
				break;
				
			/*Author screen

				*/
		}
		
		if(JOY_PUSH_DOWN || JOY_PUSH_RIGHT){
			if(currentOption < 2)
				currentOption++;
				delay_ms(100);
		}
		
		if(JOY_PUSH_UP || JOY_PUSH_LEFT){
			if(currentOption > 0)
				currentOption--;
				delay_ms(100);
		}
		
		if(RIGHT_KEY_DOWN){
			fromReturn = true;
			delay_ms(100);
			return;
		}
		
		if(JOY_PUSHED || LEFT_KEY_DOWN){
			runOption = 1;
			delay_ms(100);
		}	
	}
}

void gameScreen (void){
	int currentOption = 0;
	bool isWin = false;
	int i;
	
	LCDClearScreen();
	
	//INITIALIZATION
	generateNumbers();
	switch(difficult){
		case EASY:
			atempts = 999;
			break;
		
		case NORMAL:
			atempts = 20;
			break;
		
		case HARD:
			atempts = 8;
			break;
	}
	
	drawGameNet();
	drawCircleLine(0);
	drawCircleLine(1);
	drawCircleLine(2);
	drawCircleLine(3);

	drawNumberLine(currentArray, 0);

	drawResultDots(1);
	drawResultDots(2);
	drawResultDots(3);
	
	LCDSetCircle(124, 112, 3, RED);
	LCDSetCircle(124, 124, 3, RED);
	LCDSetCircle(118, 118, 3, RED);
	LCDSetCircle(112, 112, 3, RED);
	LCDSetCircle(112, 124, 3, RED);
	
	if(devMode == true){
		LCDPutChar(targetArray[0] + '0', 5, 102, SMALL, RED, BLACK);
		LCDPutChar(targetArray[1] + '0', 5, 107, SMALL, RED, BLACK);
		LCDPutChar(targetArray[2] + '0', 5, 112, SMALL, RED, BLACK);
		LCDPutChar(targetArray[3] + '0', 5, 117, SMALL, RED, BLACK);
		LCDPutChar(targetArray[4] + '0', 5, 122, SMALL, RED, BLACK);
	}
	
	/////////////////	
	globalTime = 0;
	gameTime = 0;
	
	drawCircleCursor(1, currentOption);
	
	while(1){

		sprintf(ciag, "CZAS:%d  ", gameTime);
		LCDPutStr(ciag, 6, 5, SMALL, BLACK, RED);
		sprintf(ciag, "ILOSC PROB:%d  ", atempts);
		LCDPutStr(ciag, 16, 5, SMALL, BLACK, RED);
		
		
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
			delay_ms(100);
			isWin = checkValues();
			//drawResultFilledDots(0, ifWinArray);
			saveChoosenValues();
			saveResultValues();
			atempts--;
			for(i = 0; i < 3; i++){
				drawNumberLine(chooseHistoryArray[i], i + 1);
				drawResultFilledDots(i + 1, resultHistoryArray[i]);
			}
			if(isWin == true){
				score = ((int)(atempts * 100/gameTime))*100;
				switch(difficult){
					case EASY:
						score = (int)score * 0.001;
						break;
					case MEDIUM:
						break;
					case HARD:
						score = score * 2;
						break;
				}
				winScreen();
				return;
			}
		}
		
		if(RIGHT_KEY_DOWN){
			globalTime = 0;
			fromReturn = true;
			clearChooseHistoryArray();
			clearResultHistoryArray();
			clearAfterGame();
			delay_ms(100);
			return;
		}
		
		if(atempts == 0){
			delay_ms(100);
			LCDClearScreen();
			return;
		}
	}
	
}

void clearAfterGame(void){
	int i;
	
	for(i = 0; i < 5; i++){
		targetArray[i] = 1;
		currentArray[i] = 1;
		ifWinArray[i] = 1;
	}
}

void winScreen(void){
	LCDClearScreen();
	
	LCDPutStr("!!WYGRALES!!", 20, 17, LARGE, BLACK, RED);	
	LCDPutStr("TWOJ CZAS TO:", 50, 15, MEDIUM, BLACK, RED);	
	sprintf(ciag, "%d", gameTime);
	LCDPutStr(ciag, 60, 15, SMALL, BLACK, RED);	
	LCDPutStr("TWOJ WYNIK TO:", 80, 12, MEDIUM, BLACK, RED);
	sprintf(ciag, "%d", score);
	LCDPutStr(ciag, 90, 15, SMALL, BLACK, RED);		
	
	clearChooseHistoryArray();
	clearResultHistoryArray();
	clearAfterGame();
	
	while(1){

		if(RIGHT_KEY_DOWN){
			fromReturn = true;
			delay_ms(100);
			return;
		}
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

void drawColouredDot(int color, int x, int y){
	LCDSetCircle(x, y, 10, color);	
}

bool checkValues(void){
	int i, j;
	int ifWinArrayIndex = 0;
	
	//checking color and place of dots
	for(i = 0; i < 5; i++){
		//checking good position
		if(currentArray[i] == targetArray[i]){
			ifWinArray[ifWinArrayIndex] = GOOD;
			ifWinArrayIndex++;
		}
		else{
			for(j = 0; j < 5; j++){
				//checking good number
				if(j != i){
					if(currentArray[i] == targetArray[j]){
						ifWinArray[ifWinArrayIndex] = BAD_POSITION;
						ifWinArrayIndex++;
						j = 100;
					}
				}
			}
			if(j == 5){
				ifWinArray[ifWinArrayIndex] = BAD;
				ifWinArrayIndex++;	
			}
		}
	}
	
	//checking targetArray
	for(i = 0; i < 5; i++){
		if(ifWinArray[i] != GOOD){
			return false;
		}
	}
	return true;
}

void highScoreScreen(void){
	LCDClearScreen();
	LCDPutStr("NAJLEPSZE WYNIKI", 20, 15, SMALL, BLACK, RED);
	
	while(1){
		if(RIGHT_KEY_DOWN){
			fromReturn = true;
			delay_ms(100);
			return;
		}
	}
}

void optionScreen(void){
	int currentOption = 0;
	bool leftPushed = false;
	bool rightPushed = false;
	LCDClearScreen();

	while(1){
		LCDPutStr("OPCJE", 20, 45, LARGE, BLACK, RED);
		LCDPutStr("POZIOM TRUDNOSCI", 50, 2, MEDIUM, BLACK, RED);
		LCDPutStr("DEV MODE", 70, 2, MEDIUM, BLACK, RED);
		
		if(devMode == true)
			LCDPutStr("TAK", 80, 10, SMALL, BLACK, RED);
		if(devMode == false)
			LCDPutStr("NIE", 80, 10, SMALL, BLACK, RED);
		
		switch(difficult){
			case EASY:
				LCDPutStr("LATWY", 60, 10, SMALL, BLACK, RED);
				break;
			
			case NORMAL:
				LCDPutStr("NORMALNY", 60, 10, SMALL, BLACK, RED);
				break;
			
			case HARD:
				LCDPutStr("TRUDNY", 60, 10, SMALL, BLACK, RED);
				break;
		}
		
		switch(currentOption){
			case 0:
				LCDSetCircle(63, 5,3, YELLOW);
				LCDSetCircle(83, 5,3, BLACK);
				if(leftPushed == true){
					if(difficult > 0){
						difficult--;
					}
					leftPushed = false;
				}
				
				if(rightPushed == true){
					if(difficult < 2){
						difficult++;
					}
					rightPushed = false;
				}
				
				break;
				
			case 1:
				LCDSetCircle(63, 5,3, BLACK);
				LCDSetCircle(83, 5,3, YELLOW);
				if(leftPushed == true){
					if(devMode == true){
						devMode = false;
					}						
					leftPushed = false;
				}
				
				if(rightPushed == true){
					if(devMode == false){
						devMode = true;
					}
					rightPushed = false;
				}				
				break;
		}
		
		if(JOY_PUSH_UP){
				if(currentOption > 0){
					currentOption--;
				}
				delay_ms(100);
		}
		
		if(JOY_PUSH_DOWN){
				if(currentOption < 1){
					currentOption++;
				}
				delay_ms(100);
		}
			
		if(JOY_PUSH_LEFT){
			leftPushed = true;
			delay_ms(100);
		}
		
		
		if(JOY_PUSH_RIGHT){
			rightPushed = true;
			delay_ms(100);
		}
			
		if(LEFT_KEY_DOWN){
			
			delay_ms(100);
		}
		
		if(RIGHT_KEY_DOWN){
			fromReturn = true;
			delay_ms(100);
			return;
		}
	}
}

void authorScreen(void){
	LCDClearScreen();
	while(1){
		LCDPutStr("AUTOR", 20, 45, SMALL, BLACK, RED);
		LCDPutStr("KRZYSZTOF PLUCINSKI", 40, 5, SMALL, BLACK, RED);
		
		if(RIGHT_KEY_DOWN){
			fromReturn = true;
			delay_ms(100);
			return;
		}
	}
}

void clearChooseHistoryArray(void){
	int i, j;
	for(i = 0; i < 3; i++){
		for(j = 0; j < 5; j++){
			chooseHistoryArray[i][j] = 0;
		}	
	}
}

void saveChoosenValues(void){
	int i, j;
	for(i = 2; i > 0; i--){
		for(j = 0; j < 5; j++){
			chooseHistoryArray[i][j] = chooseHistoryArray[i - 1][j];
		}
	}
	for(j = 0; j < 5; j++){
		chooseHistoryArray[0][j] = currentArray[j];
	}
}

void clearResultHistoryArray(void){
		int i, j;
	for(i = 0; i < 3; i++){
		for(j = 0; j < 5; j++){
			resultHistoryArray[i][j] = BAD;
		}	
	}
}
void saveResultValues(void){
	int i, j;
	for(i = 2; i > 0; i--){
		for(j = 0; j < 5; j++){
			resultHistoryArray[i][j] = resultHistoryArray[i - 1][j];
		}
	}
	for(j = 0; j < 5; j++){
		resultHistoryArray[0][j] = ifWinArray[j];
	}
}

