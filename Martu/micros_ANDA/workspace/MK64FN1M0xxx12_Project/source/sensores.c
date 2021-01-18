
/*
 * Copyright 2016-2021 NXP
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
 * @file    MK64FN1M0xxx12_Project.c
 * @brief   Application entry point.
 */

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"
#include "gpio.h"
#include "pit.h"
#include "i2c_config.h"
#include "fever.h"
#include "algorithm.h"
#include "o2.h"



/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

#define PIN_SPO2    PORTNUM2PIN(PB,18) // PTB18

void callback_pin (void);
void callback_pit(void);

uint8_t a = 0;

uint32_t aun_ir_buffer[FS*ST]; //infrared LED sensor data
uint32_t aun_red_buffer[FS*ST];  //red LED sensor data
float n_spo2,ratio,correl;  //SPO2 value
int8_t ch_spo2_valid;  //indicator to show if the SPO2 calculation is valid
int32_t n_heart_rate; //heart rate value
int8_t  ch_hr_valid;  //indicator to show if the heart rate calculation is valid


/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    uint8_t uch_dummy;
    i2cInit();

    maxim_max30102_reset(); //resets the MAX30102
    maxim_max30102_read_reg_blocking(REG_INTR_STATUS_1, &uch_dummy);  //Reads/clears the interrupt status register
    maxim_max30102_init();  //initialize the MAX30102

    gpioMode(PIN_SPO2, INPUT);
    gpioIRQ(PIN_SPO2, GPIO_IRQ_MODE_FALLING_EDGE, callback_pin);
    NVIC_EnableIRQ(PORTB_IRQn);


    init_pit(0, 1000000U, callback_pit);

    PRINTF("Hello World\n");

    /* Force the counter to be placed into memory. */
    volatile static int k = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
    	k++;

        /* 'Dummy' NOP to allow source level single stepping of tight while() loop */

        __asm volatile ("nop");
    }
    return 0 ;
}


void callback_pin (void)
{

	bool meassure = maxim_max30102_read_fifo(( aun_ir_buffer  + a), (aun_red_buffer + a));

	if (meassure == true)
	{
		a++;

	}

	if(a == FS*ST)
	{
		rf_heart_rate_and_oxygen_saturation(aun_ir_buffer, FS*ST, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid, &ratio, &correl);

		if (ch_spo2_valid ){
			printf("spo2 %d heart rate %d \n",(int)n_spo2, n_heart_rate);
		}
		else
		{
			printf("no valido \n");
		}

		a = 0;
	}

}

void callback_pit(void)
{
	fever_temp_meassurement();
	printf("temp = %d \n", (int)fever_get_temperature());

}







