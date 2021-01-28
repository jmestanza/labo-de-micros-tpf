/*
 * pit.h
 *
 *  Created on: Nov 25, 2014
 *      Author: Manuel Alejandro
 */

#ifndef PIT_H_
#define PIT_H_

#include "derivative.h"

#define ADC_CHANNEL 12 // Channel 12 (PTB2)
#define LED_BLUE  21 // PTB21

void pit_init(void);
void PIT_IRQHandler(void);
void enable_irq (int irq);


#endif /* PIT_H_ */
