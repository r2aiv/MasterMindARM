#include <AT91SAM7X256.H>
#include <stdlib.h>
#include <time.h>
#include "lcd.h"

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
extern void delay_ms(unsigned int ms);
//extern void LCDWrite130x130bmp(void);

//STATES OF PROGRAM
void helloScreen(void);
void menuScreen(void);
void gameScreen(void);
void optionScreen(void);
void highScoreScreen(void);
void winScreen(void);
void authorScreen(void);

//DRAWING FUNCTIONS
void drawColouredDot(int color, int x, int y); //TODO: correct
void drawGameNet(void);

//GAME LOGIC
void generateNumbers(void);
void clearCurentTab(void);
bool checkValues(void);
/**** END OF FUNCTIONS DECLARATION ****/

/**** GLOBAL VARIABLE DECLARATION ****/
bool flagArray[9];
int targetArray[5];
int currentArray[5];
guess ifWinArray[5];

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

int main(void){
	//Enable clock of the PIO
	AT91C_BASE_PMC -> PMC_PCER = (1 << AT91C_ID_PIOA);
	AT91C_BASE_PMC -> PMC_PCER = (1 << AT91C_ID_PIOB);
	InitSpi();

	// Init LCD
	InitLcd();

	LCDClearScreen();

	//srand(time(0));

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
		menuScreen();
	}
}


void menuScreen(void){
	volatile int currentOption = 0;
	volatile int runOption = 0;
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
				if(runOption)
					gameScreen();
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
	//TODO: handling of win
	int currentOption = 0;
	
	LCDClearScreen();
	
	drawGameNet();
	
	//INITIALIZATION
	//generateNumbers();
	//drawColouredDot(RED, 10, 5);
	LCDSetCircle(117, 13, 10, RED);	
	//drawColouredDot(RED, 10, 25);
	LCDSetCircle(117, 33, 10, RED);	
	//drawColouredDot(RED, 10, 45);
	LCDSetCircle(117, 53, 10, RED);	
	//drawColouredDot(RED, 10, 65);
	LCDSetCircle(117, 73, 10, RED);	
	//drawColouredDot(RED, 10, 85);
	LCDSetCircle(117, 93, 10, RED);	
	
	LCDPutChar('1', 110, 9, LARGE, RED, BLACK); //znak w kole

	LCDPutStr("CZAS: xxxx", 6, 5, SMALL, BLACK, RED);
	LCDPutStr("ILOSC PROB: x", 16, 5, SMALL, BLACK, RED);
	
	//prototyp interfejsu
	
	//2 linia
	LCDSetCircle(91, 13, 10, RED);	
	//drawColouredDot(RED, 10, 25);
	LCDSetCircle(91, 33, 10, RED);	
	//drawColouredDot(RED, 10, 45);
	LCDSetCircle(91, 53, 10, RED);	
	//drawColouredDot(RED, 10, 65);
	LCDSetCircle(91, 73, 10, RED);	
	//drawColouredDot(RED, 10, 85);
	LCDSetCircle(91, 93, 10, RED);
	
	//3 linia
	LCDSetCircle(65, 13, 10, RED);	
	//drawColouredDot(RED, 10, 25);
	LCDSetCircle(65, 33, 10, RED);	
	//drawColouredDot(RED, 10, 45);
	LCDSetCircle(65, 53, 10, RED);	
	//drawColouredDot(RED, 10, 65);
	LCDSetCircle(65, 73, 10, RED);	
	//drawColouredDot(RED, 10, 85);
	LCDSetCircle(65, 93, 10, RED);
	
	//4linia
	LCDSetCircle(39, 13, 10, RED);	
	//drawColouredDot(RED, 10, 25);
	LCDSetCircle(39, 33, 10, RED);	
	//drawColouredDot(RED, 10, 45);
	LCDSetCircle(39, 53, 10, RED);	
	//drawColouredDot(RED, 10, 65);
	LCDSetCircle(39, 73, 10, RED);	
	//drawColouredDot(RED, 10, 85);
	LCDSetCircle(39, 93, 10, RED);
	
	//wyniki
	LCDSetCircle(124, 112, 3, RED);
	LCDSetCircle(124, 124, 3, RED);
	LCDSetCircle(118, 118, 3, RED);
	LCDSetCircle(112, 112, 3, RED);
	LCDSetCircle(112, 124, 3, RED);
	
	/////////////////	
	
	while(1){
		//LCDPutStr("GRA", 120, 70, SMALL, BLACK, RED);
		/*
		drawColouredDot(currentArray[0], 10, 5);
		drawColouredDot(currentArray[1], 10, 25);
		drawColouredDot(currentArray[2], 10, 45);
		drawColouredDot(currentArray[3], 10, 65);
		drawColouredDot(currentArray[4], 10, 85);
		*/
		/*if(JOY_PUSH_UP)
			if((currentArray[currentOption] < 5) && (currentArray[currentOption] > 0))
				currentArray[currentOption]++;
		
		if(JOY_PUSH_DOWN)
			if((currentArray[currentOption] < 5) && (currentArray[currentOption] > 0))
				currentArray[currentOption]--;
			
		if(JOY_PUSH_LEFT)
			if((currentOption < 5) && (currentOption > 0))
				currentOption--;
		
		if(JOY_PUSH_RIGHT)
			if((currentOption < 5) && (currentOption > 0))
				currentOption++;
				
		if(LEFT_KEY_DOWN){
			if(checkValues()){
				winScreen();
				return;
			}
			else{
				//drawing result array
				//drawing last choice
			}
		}*/
		
		if(RIGHT_KEY_DOWN)
			//return;
		helloScreen();
	}
	
}

void winScreen(void){
	while(1){
		LCDPutStr("WYGRALES", 120, 70, SMALL, BLACK, RED);
		if(RIGHT_KEY_DOWN)
		//	return;
		helloScreen();
	}
}

void generateNumbers(void){
	int i;
	
	for(i = 0; i < 5; i++){
		do{
			targetArray[i] = 1;//(rand() % 9) + 1;
		}
		while(flagArray[targetArray[i]] == true);
		flagArray[targetArray[i]] = true;
	}
}

//TODO - implement
void drawColouredDot(int color, int x, int y){
	LCDSetCircle(x, y, 10, color);	
}

//TODO - implement
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
				//checking good color
				if(currentArray[i] == targetArray[j]){
					ifWinArray[ifWinArrayIndex] = COLOR;
					ifWinArrayIndex++;
				}
			}
		}
	}
	
	//checking targetArray
	for(i = 0; i < 5; i++){
		if(ifWinArray[i] != GOOD)
			return false;
	}
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

void drawGameNet(void){
	LCDSetLine(104, 0, 104, 130, YELLOW);
	LCDSetLine(26, 105, 130, 105, YELLOW);
	LCDSetLine(26, 0, 26, 130, YELLOW);
	LCDSetLine(78, 0, 78, 130, YELLOW);
	LCDSetLine(52, 0, 52, 130, YELLOW);
}
