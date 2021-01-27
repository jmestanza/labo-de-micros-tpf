/*
 * ecg.h
 *
 *  Created on: Jan 18, 2021
 *      Author: martinamaspero
 */

#ifndef ECG_H_
#define ECG_H_

#include <stdio.h>
#include <stdint.h>

#define ADC_CH 0
#define PIT_CH 1
#define ECG_VALUES	12

void ecg_init(void (*funcallback)(void));
void ecg_get_samples(uint16_t * data);




#endif /* ECG_H_ */
