#include <AT91SAM7X256.H>
#include <inttypes.h>
#include "lcd.h"
#include "adc.h"

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
extern void Delay (unsigned long a);
extern void delay_ms(unsigned int ms);
extern void LCDWrite130x130bmp(void);

void helloScreen(void);
void menuScreen(void);
void gameScreen(void);
void optionScreen(void);
void highScoreScreen(void);
/**** END OF FUNCTIONS DECLARATION ****/

int main(void){
	//Enable clock of the PIO
	AT91C_BASE_PMC -> PMC_PCER = (1 << AT91C_ID_PIOA);
	AT91C_BASE_PMC -> PMC_PCER = (1 << AT91C_ID_PIOB);
	
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
	
	#define LEFT_KEY_DOWN (((AT91C_BASE_PIOB -> PIO_PDSR) & AT91C_PIO_PB24) == 0)
	#define RIGHT_KEY_DOWN (((AT91C_BASE_PIOB -> PIO_PDSR) & AT91C_PIO_PB25) == 0)
	#define JOY_PUSH_LEFT (((AT91C_BASE_PIOA -> PIO_PDSR) & AT91C_PIO_PA7) == 1)
	#define JOY_PUSH_RIGHT (((AT91C_BASE_PIOA -> PIO_PDSR) & AT91C_PIO_PA15) == 1)
	#define JOY_PUSH_UP (((AT91C_BASE_PIOA -> PIO_PDSR) & AT91C_PIO_PA14) == 1)
	#define JOY_PUSH_DOWN (((AT91C_BASE_PIOA -> PIO_PDSR) & AT91C_PIO_PA9) == 1)
	#define JOY_PUSHED (((AT91C_BASE_PIOA -> PIO_PDSR) & AT91C_PIO_PA8) == 1)
	
	helloScreen();
}



//TODO: add visual efects. Some kind of gradient
//TODO: add continuing by pressing button
void helloScreen(void){
	LCDClearScreen();
	LED_BCK_ON;	
	LCDWrite130x130bmp();
	LCDPutStr("START", 10, 0, SMALL, BLACK, RED);
	
	while(1)
	if(LEFT_KEY_DOWN){
		menuScreen();
	}
}


void menuScreen(void){
	int currentOption = 0;
	int runOption = 0;
	
	while(1){
		LCDClearScreen();
		
		LCDPutStr("MENU", 120, 70, SMALL, BLACK, RED);
		LCDPutStr("1.NOWA GRA", 100, 60, SMALL, BLACK, RED);
		LCDPutStr("2.NAJLEPSZE WYNIKI", 80, 60, SMALL, BLACK, RED);
		LCDPutStr("3.OPCJE", 60, 60, SMALL, BLACK, RED);
		LCDPutStr("WYBIERZ", 10, 0, SMALL, BLACK, RED);
		
		switch(currentOption){
			case 0:
				//TODO: starting new game
				//TODO: drawing rectangle on the new game option
				if(runOption)
					gameScreen();
				LCDSetRect(100, 60, 80, 40, NOFILL, RED);
				break;
			case 1:
				//TODO: opening highscores
				//TODO: drawing rectangle on the highscore option
				if(runOption)
					highScoreScreen();
				LCDSetRect(80, 60, 60, 40, NOFILL, RED);
				break;
			case 2:
				//TODO: opening options
				//TODO: drawing rectangle on the options option ;)
				if(runOption)
					optionScreen();
				LCDSetRect(60, 60, 40, 40, NOFILL, RED);
				break;
		}
		
		if(JOY_PUSH_UP){
			if(currentOption < 3)
				currentOption++;
		}
		else if(JOY_PUSH_DOWN){
			if(currentOption > 0)
				currentOption--;
		}
		else if(JOY_PUSHED || LEFT_KEY_DOWN){
			runOption = 1;
		}	
	}
	
}

void gameScreen (void){
	while(1){
		LCDPutStr("GRA", 120, 70, SMALL, BLACK, RED);
		
		if(RIGHT_KEY_DOWN)
			menuScreen();
	}
}

void highScoreScreen(void){
	while(1){
		LCDPutStr("NAJLEPSZE WYNIKI", 120, 70, SMALL, BLACK, RED);
		
		if(RIGHT_KEY_DOWN)
			menuScreen();
	}
}

void optionScreen(void){
	while(1){
		LCDPutStr("OPCJE", 120, 70, SMALL, BLACK, RED);
		
		if(RIGHT_KEY_DOWN)
			menuScreen();
	}
}
