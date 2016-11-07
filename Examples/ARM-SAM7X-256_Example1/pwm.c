#include "pwm.h"
#include "bits.h"
//#include "ioat91sam7x256.h"
#include <AT91SAM7X256.H>                        /* AT91SAMT7X256 definitions */

AT91PS_PWMC     w_pPWM      = AT91C_BASE_PWMC;
AT91PS_PMC      w_pPMC      = AT91C_BASE_PMC;
AT91PS_PWMC_CH  w_pPWM_CH3  = AT91C_BASE_PWMC_CH3;
AT91PS_PIO      w_pPioA     = AT91C_BASE_PIOA;
AT91PS_PIO      w_pPioB     = AT91C_BASE_PIOB;

void InitPWM(void) {

  // Enable clock for interface
  w_pPMC->PMC_PCER = 1 << AT91C_ID_PWMC;

  // Set second functionality of pin PB30 - LED BLUE = PWM3 (B peripherial - str. 33 data sheet)
  w_pPioB->PIO_PDR = BIT30;
  w_pPioB->PIO_ASR = 0;
  w_pPioB->PIO_BSR = BIT30;

  // Set second functionality of pin PB19 - AUDIO_OUT = PWM0 (A peripherial - str. 33 data sheet)
  w_pPioB->PIO_PDR = BIT19;
  w_pPioB->PIO_ASR = BIT19;
  w_pPioB->PIO_BSR = 0;

	
	
	

  // Enable clock for PWM
  w_pPWM->PWMC_MR = 0x00010001;

  // Frequency for PWM

  // Period for PWM 0
  w_pPWM_CH3->PWMC_CPRDR = 1023;

	// Period for PWM 3
  w_pPWM_CH3->PWMC_CPRDR = 1023;

  // Duty for PWM
  w_pPWM_CH3->PWMC_CDTYR = 512;

	// Duty for PWM
  w_pPWM_CH3->PWMC_CDTYR = 512;

  // Modify the period at the next period start event.
  w_pPWM_CH3->PWMC_CMR = 0x0400;

  // Enable PWM chanel
  w_pPWM->PWMC_ENA = 1 <<3; //PWM3

}

void SetPWMValue(unsigned int val) {

  // set new value
  w_pPWM_CH3->PWMC_CDTYR = val;
}



