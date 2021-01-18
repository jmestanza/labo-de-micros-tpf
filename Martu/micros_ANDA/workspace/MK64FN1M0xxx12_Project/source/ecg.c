/*
 * ecg.c
 *
 *  Created on: Jan 18, 2021
 *      Author: martinamaspero
 */
#include "ecg.h"
#include "adc.h"
#include "pit.h"
#include "gpio.h"

void pit_func (void);

void ecg_init()
{

    gpioMode(LO_MAS, INPUT);
    gpioMode(LO_MENOS, INPUT);
	//ADC_init(0, updateData);
	ADC_enableModule(0);

	init_pit(1, 10000, pit_func);


}


void pit_func (void)
{
	if(!(gpioRead(LO_MAS) || gpioRead(LO_MENOS)))
	{
		ADC_startConvertion();
	}
	else
	{
		printf("!!!!!!!");
	}
}


