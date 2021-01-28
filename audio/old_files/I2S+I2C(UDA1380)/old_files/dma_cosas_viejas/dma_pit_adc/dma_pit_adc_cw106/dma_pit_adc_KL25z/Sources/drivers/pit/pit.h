/*
 * pit.h
 *
 *  Created on: Nov 25, 2014
 *      Author: Manuel Alejandro
 */

#ifndef PIT_H_
#define PIT_H_

#include "derivative.h"
#include "adc.h"

#define ADC_CHANNEL 8 // Channel 8 (PTB0)
#define LED_GREEN  19 // PTB19

void pit_init(void);
void PIT_IRQHandler(void);
void enable_irq (int irq);


#endif /* PIT_H_ */
