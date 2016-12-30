#include "animation.h"
#include "lcd.h"

void changeAnimation(void){
	int i;
	for(i = 0; i < 132; i++){
		LCDSetRect(0, 0, i, i, FILL, WHITE);
		delay_ms(5);
	}
	for(i = 132; i >= 0; i--){
		LCDSetRect(132, 132, i, i, FILL, BLACK);
		delay_ms(5);
	}
}

void changeAnimation2(void){
	int i;
	for(i = 0; i < 66; i++){
		LCDSetRect(66 - i, 66 - i, 66 + i, 66 + i, FILL, WHITE);
		delay_ms(5);
	}
	for(i = 66; i >= 0; i--){
		LCDSetRect(132 - i, 132 - i, 0 + i, 0 + i, FILL, BLACK);
		delay_ms(5);
	}
}
