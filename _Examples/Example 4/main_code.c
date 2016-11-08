/******************************************************************************/
// Program utworzony do potrzeb laboratorium IISI Systemy Wbudowane
// do wykorzystania w celach dydaktycznych podczas zajec laboratoryjnych
/******************************************************************************/

#include <AT91SAM7X256.H>                        /* AT91SAMT7X256 definitions */
#include <stdio.H>                        //standard I/O
#include <math.H>                        //math library
#include "lcd.h"
#include "adc.h"
#include <inttypes.h>	 //typy jednolite danych

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


char ciag[40];
	


//
//  Main Program

int main (void) {

volatile int ADC_result;
int hor_pos, dir_hor;
	float speed;
  float voltage;
	
	// Enable the Clock of the PIO
  AT91C_BASE_PMC->PMC_PCER  = (1 << AT91C_ID_PIOB) | (1 << AT91C_ID_PIOA);
	
	InitADC();

  // Initialize SPI interface to LCD
   InitSpi();

  // Init LCD
   InitLcd();

	// clear the screen
   LCDClearScreen();


	

	WriteSpiCommand(VOLCTR);
	WriteSpiData(40);    // P1 = 44  volume value (experiment with this value to get the best contrast)
	WriteSpiData(3);   // P2 = 3   resistance ratio (only value that works)

	hor_pos=0;
	dir_hor=5;
	while(1) 
	{

			//kontrast regulowany potencjometrem
			//ADC_result=GetAdcChanel(ADC_CHN_7);
			ADC_result=GetAdcChanel_wersja2(6);
			sprintf(ciag, "adc_val=%04d [-]", ADC_result);
			LCDPutStr(ciag, 40, 0, LARGE, BLACK, YELLOW);
			speed= ADC_result*0.001;
			
			#define C1 0.0032258064
			voltage = C1 * ADC_result;

			sprintf(ciag, "voltage=%3.2f[V]", voltage);
			LCDPutStr(ciag, 60, 0, LARGE, BLACK, YELLOW);

			
			//prosta animacja przemieszczania sie kwadratu ze zmienna szybkoscia
			delay_ms(100);
			//skasowanie starej pozycji
			LCDSetRect(100, hor_pos, 120, hor_pos+20, FILL, BLACK);

			hor_pos+=dir_hor*speed;
			if (hor_pos>100)
				dir_hor=-10;
			if (hor_pos<=0)
				dir_hor=+10;
			//wyswietlenie nowej pozycji
			LCDSetRect(100, hor_pos, 120, hor_pos+20, FILL, BLUE);
		
	}; //while

		
		
 }; //main
