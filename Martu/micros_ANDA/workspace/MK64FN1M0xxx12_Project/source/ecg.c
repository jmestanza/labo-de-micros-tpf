/*
 * ecg.c
 *
 *  Created on: Jan 18, 2021
 *      Author: martinamaspero
 */
#include "ecg.h"
#include "gpio.h"
#include "fsl_adc16.h"
#include "MK64F12.h"
#include "peripherals.h"
#include "fsl_pit.h"



static uint16_t ecgValues[ECG_VALUES];
static uint16_t ecgValues1[ECG_VALUES];
static uint8_t sample_ptr = 0;
void (*samplesReadyCallback)(void);
bool pingpong = false;

void ecg_init(void (*funcallback)(void))
{

	// USA PIT 1 Y ADC0!!!!!
	samplesReadyCallback = funcallback;
	NVIC_EnableIRQ(PIT1_IRQn);
	PIT_StartTimer(PIT_PERIPHERAL,PIT_CHANNEL_1);

}


void ecg_get_samples(uint16_t * data)
{
	if(pingpong)
	{
		for (int i =0 ; i < ECG_VALUES; i++)
		{
			data[i] = ecgValues1[i] ;
		}

	}
	else
	{
		for (int i =0 ; i < ECG_VALUES; i++)
		{
			data[i] = ecgValues[i] ;
		}
	}

}



/* ADC0_IRQn interrupt handler */
void ADC0_IRQHANDLER(void) {
  /*  Place your code here */

	if(sample_ptr == ECG_VALUES)
		{
			sample_ptr = 0;
			pingpong = !pingpong;
			samplesReadyCallback();
		}
		if(pingpong)
		{
			ecgValues[sample_ptr]= ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP);
		}
		else
		{
			ecgValues1[sample_ptr]= ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP);
		}
		sample_ptr++;

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}



