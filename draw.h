#ifndef DRAW_H
#define DRAW_H

typedef int guess;
#define BAD 0
#define BAD_POSITION 1
#define GOOD 2

void drawGameNet(void);
void drawCircleLine(int line);
void drawCircleCursor(int currentPosition, int newPosition);
void drawNumber(int oldNumber, int newNumber, int position);
void drawNumberLine(int numbers[5], int line);
void drawResultDots(int line);
void drawResultFilledDots(int line, guess values[5]);

#endif