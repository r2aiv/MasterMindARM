#include "draw.h"
#include "lcd.h"

void drawGameNet(void){
	LCDSetLine(104, 0, 104, 130, YELLOW);
	LCDSetLine(26, 105, 130, 105, YELLOW);
	LCDSetLine(26, 0, 26, 130, YELLOW);
	LCDSetLine(78, 0, 78, 130, YELLOW);
	LCDSetLine(52, 0, 52, 130, YELLOW);
}

void drawCircleLine(int line){
	int deltaLine;//, i;
	
	if((line >= 0) && (line < 4))
		deltaLine = 26 * line;
	else
		return;
	
	LCDSetCircle(117 - deltaLine, 13, 10, RED);	
	LCDSetCircle(117 - deltaLine, 33, 10, RED);	
	LCDSetCircle(117 - deltaLine, 53, 10, RED);	
	LCDSetCircle(117 - deltaLine, 73, 10, RED);	
	LCDSetCircle(117 - deltaLine, 93, 10, RED);	
	//eksperyment z wypelnieniem
	//for (i = 10; i > 0 ; i--)
	//	LCDSetCircle(117 - deltaLine, 93, i, RED);	
}

void drawCircleCursor(int currentPosition, int newPosition){
	int deltaYcurrent, deltaYnew;
	
	if((currentPosition >= 0) && (currentPosition <= 4) && (newPosition >= 0) && (newPosition <= 4)){
		deltaYcurrent = 20 * currentPosition;
		deltaYnew = 20 * newPosition;
	}	
	else
		return;
	
	LCDSetCircle(117, 13 + deltaYcurrent, 10, RED);
	LCDSetCircle(117, 13 + deltaYnew, 10, YELLOW);
}

void drawNumber(int oldNumber, int newNumber, int position){
	int deltaY;
	if((position >= 0) && (position <= 4)){
		deltaY = 20 * position;

	}	
	else
		return;
	
	LCDPutChar(oldNumber + '0', 110, 9 + deltaY, LARGE, BLACK, BLACK);
	LCDPutChar(newNumber + '0', 110, 9 + deltaY, LARGE, RED, BLACK);
}

void drawResultDots(int line){
	int deltaLine;
	
	if((line >= 0) && (line < 4))
		deltaLine = 26 * line;
	else
		return;
	
	LCDSetCircle(124 - deltaLine, 112, 3, RED);
	LCDSetCircle(124 - deltaLine, 124, 3, RED);
	LCDSetCircle(118 - deltaLine, 118, 3, RED);
	LCDSetCircle(112 - deltaLine, 112, 3, RED);
	LCDSetCircle(112 - deltaLine, 124, 3, RED);
}
