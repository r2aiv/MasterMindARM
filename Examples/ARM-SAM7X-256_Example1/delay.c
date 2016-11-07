#include "delay.h"
void delay_us(int val) {
  unsigned int n;
  for (n = 0; n < (10*val); n++);
} 

void delay_1ms(void) {
  unsigned int n;

  for (n = 0; n < 10000; n++);

}
void delay (int val) {
  unsigned int n;

  for (n = 0; n < val; n++)
		delay_1ms();
}
