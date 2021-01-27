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
/* TODO: insert other definitions and declarations here. */
void ecg_callback(void);


uint16_t i,j;
uint16_t bpm[160];
uint16_t oxy[160];
uint16_t data[ECG_VALUES];
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

    /*
     * Init ECG
     */
    ecg_init(ecg_callback);

    /*
     * Oximeter init
     */
    i2cInit();

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
	if(!(j%40))
	{// Cada 1s
		PIT_StopTimer(PIT_1_PERIPHERAL, kPIT_Chnl_2);
		fever_temp_meassurement();
		float temp;
		temp = fever_get_temperature();
 		lcdGFX_updateDATA(i%99, 82.34, temp);
 		PIT_StartTimer(PIT_1_PERIPHERAL, kPIT_Chnl_2);
	}

}

void ecg_callback(void)
{
	ecg_get_samples(data);
	for(int k=0;k<ECG_VALUES-1;k++)
	{
		lcdGFX_updateGFX((data[k]+data[k+1])/82, oxy[i%160]);
	}
}
