/*
 * Copyright 2016-2018 NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    CodeMerge_V1.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "GFX_Lib_K64F.h"
#include "ecg.h"
#include "fever.h"
#include "i2c_config.h"
#include "o2.h"
#include "algorithm.h"
/* TODO: insert other definitions and declarations here. */
void ecg_callback(void);


uint16_t i,j,l;
uint16_t bpm[160];
uint16_t oxy[160];
/*
 * ECG data
 */
uint16_t data[ECG_VALUES];
/*
 * Oximeter data
 */
uint8_t a = 0;

uint32_t aun_ir_buffer[FS*ST]; //infrared LED sensor data
uint32_t aun_red_buffer[FS*ST];  //red LED sensor data
float n_spo2,prev_spo2,ratio,correl;  //SPO2 value
int8_t ch_spo2_valid;  //indicator to show if the SPO2 calculation is valid
int32_t n_heart_rate,prev_heart_rate; //heart rate value
int8_t  ch_hr_valid;  //indicator to show if the heart rate calculation is valid
/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    PRINTF("Hello World\n");

    /*
     * LCD Init
     */
    lcdGFX_init();
    // Test graphics vectors (generated manually)

    for(uint16_t i=0;i<160;i++)
    {
    	oxy[i] = 20*sin(2*PI*i/30)+40;
    }

    for(uint16_t i=0;i<160;i++)
    {
    	bpm[i] = 40;
    }
    for(uint16_t i=0;i<10;i++)
    {
    	bpm[i+40] = 40+(i*40/10);
    	bpm[i+40+80] = 40+(i*40/10);
    }
    for(uint16_t i=0;i<15;i++)
    {
    	bpm[i+50] = 80-(i*60/15);
    	bpm[i+50+80] = 80-(i*60/15);
    }
    for(uint16_t i=0;i<5;i++)
    {
    	bpm[i+65] = 20+(i*60/15);
    	bpm[i+65+80] = 20+(i*60/15);
    }

    prev_spo2 = 00.00;
    prev_heart_rate = 00;
    /*
     * Init ECG
     */
    ecg_init(ecg_callback);

    /*
     * Temp init
     */
    i2cInit();

    /*
     * Oximeter init
     */
    uint8_t uch_dummy;

    maxim_max30102_reset(); //resets the MAX30102
    maxim_max30102_read_reg_blocking(REG_INTR_STATUS_1, &uch_dummy);  //Reads/clears the interrupt status register
    maxim_max30102_init();  //initialize the MAX30102

//    gpioMode(PIN_SPO2, INPUT);
//    gpioIRQ(PIN_SPO2, GPIO_IRQ_MODE_FALLING_EDGE, callback_pin);
//    NVIC_EnableIRQ(PORTB_IRQn);
    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++ ;
    }
    return 0 ;
}

void PIT0_IRQHandler(void)
{
    /* Clear interrupt flag.*/
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
	pit_callback();
}

void PIT2_IRQHandler(void)
{

	PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);
	i++;
	j++;
	if(!(j%160))
	{// Cada 4s
		PIT_StopTimer(PIT_1_PERIPHERAL, kPIT_Chnl_2);
		fever_temp_meassurement();
		float temp;
		temp = fever_get_temperature();
		if(ch_spo2_valid)
		{
			prev_spo2 = n_spo2;
			prev_heart_rate = n_heart_rate;
		}
 		lcdGFX_updateDATA(prev_heart_rate, prev_spo2, temp);
 		PIT_StartTimer(PIT_1_PERIPHERAL, kPIT_Chnl_2);
	}

}

void PORTB_IRQHandler(void)
{
    /* Clear external interrupt flag. */
    GPIO_PortClearInterruptFlags(BOARD_PIN_SPO2_GPIO, 1U << BOARD_PIN_SPO2_PIN);

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
	bool meassure = maxim_max30102_read_fifo(( aun_ir_buffer  + a), (aun_red_buffer + a));

	if (meassure == true)
	{
		a++;

	}

	if(a == FS*ST)
	{
		rf_heart_rate_and_oxygen_saturation(aun_ir_buffer, FS*ST, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid, &ratio, &correl);

		if (ch_spo2_valid ){
			//printf("spo2 %d heart rate %d \n",(int)n_spo2, n_heart_rate);
		}
		else
		{
			//printf("no valido \n");
		}

		a = 0;
	}

#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void ecg_callback(void)
{
	ecg_get_samples(data);
	for(int k=0;k<ECG_VALUES-1;k++)
	{
		uint32_t uni,dec;
		uni = (aun_red_buffer[l]/100)%10;
		dec = (aun_red_buffer[l]/1000)%10;

		lcdGFX_updateGFX((data[k]+data[k+1])/82, ((dec*10) + uni));
		l++;
		if(l == 100)
		{
			l = 0;
		}
	}
}
