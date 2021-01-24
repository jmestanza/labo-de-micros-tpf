/*
 * pit.h
 *
 *  Created on: Nov 25, 2014
 *      Author: Manuel Alejandro
 */

#ifndef PIT_H_
#define PIT_H_

#include "derivative.h"

#define ADC_CHANNEL 20 // Channel 20 (Potentiometer)
#define LED_BLUE  10 // PTA10

void pit_init(void);
void PIT0_IRQHandler(void);
void enable_irq (int irq);


#endif /* PIT_H_ */
