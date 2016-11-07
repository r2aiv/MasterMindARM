#include <AT91SAM7X256.H>
#include <inttypes.h>
#include "lcd.h"
#include "adc.h"

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

void helloScreen(void);
void menuScreen(void);
void gameScreen(void);
void optionScreen(void);
void highScoreScreen(void);

//TODO: add initialisation of all needed components (lcd, speaker, buttoms)
int main(void){
	helloScreen();
}

//TODO: add visual efects. Some kind of gradient
//TODO: add continuing by pressing button
void helloScreen(void){
	LCDPutStr("MASTER_MIND", 120, 20, MEDIUM, BLACK, RED);
	delay_ms(2000);
	menuScreen();
	
}


void menuScreen(void){
	int currentOption = 0;
	
	int keyPressed; //future: replace by handling of joystick
	while(1){
		LCDPutStr("MENU", 120, 20, SMALL, BLACK, RED);
		LCDPutStr("1.NOWA GRA", 120, 20, SMALL, BLACK, RED);
		LCDPutStr("2.NAJLEPSZE WYNIKI", 120, 20, SMALL, BLACK, RED);
		LCDPutStr("3.OPCJE", 120, 20, SMALL, BLACK, RED);
		
		switch(currentOption){
			case 0:
				//TODO: starting new game
				//TODO: drawing rectangle on the new game option
				break;
			case 1:
				//TODO: opening highscores
				//TODO: drawing rectangle on the highscore option
				break;
			case 2:
				//TODO: opening options
				//TODO: drawing rectangle on the options option ;)
				break;
		}
		
		if(keyPressed == 0){
			switch(currentOption){
			case 0:
				//TODO: starting new game
				gameScreen();
				break;
			case 1:
				//TODO: opening highscores
				highScoreScreen();
				break;
			case 2:
				//TODO: opening options
				optionScreen();
				break;
			}
		}
		
	}
	
}
