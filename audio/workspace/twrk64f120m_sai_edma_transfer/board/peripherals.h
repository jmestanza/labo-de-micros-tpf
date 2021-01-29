/*
 * Copyright 2017-2018 NXP
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
 * @file    peripherals.h
 * @brief   Peripherals initialization header file.
 */
 
/* This is a template for board specific configuration created by MCUXpresso IDE Project Wizard.*/

#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

#include "fsl_pit.h"


#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */


/* Definitions for BOARD_InitPeripherals functional group */
/* BOARD_InitPeripherals defines for PIT */
/* Definition of peripheral ID. */
#define PIT_1_PERIPHERAL PIT
/* Definition of clock source. */
#define PIT_1_CLOCK_SOURCE kCLOCK_BusClk
/* Definition of clock source frequency. */
#define PIT_1_CLK_FREQ CLOCK_GetFreq(PIT_1_CLOCK_SOURCE)
/* Definition of ticks count for channel 0. */
#define PIT_1_0_TICKS USEC_TO_COUNT(1000U, PIT_1_CLK_FREQ) - 1U

#define PIT_1_1_TICKS USEC_TO_COUNT(10000000U, PIT_1_CLK_FREQ) - 1U // 10 segs
// con 10 segundos SIEMPRE se corta a la mitad los audios (literalmente a partir de 1 segundo empieza a reproducir)
// Esto es por el tiempo de setup que creo que puede ser o bien de i2s o bien de la placa UDA1380

//#define PIT_1_1_TICKS USEC_TO_COUNT(3000000U, PIT_1_CLK_FREQ) - 1U // 5 segs

//#define PIT_1_1_TICKS USEC_TO_COUNT(5000000U, PIT_1_CLK_FREQ) - 1U // 5 segs
// En este loop SIEMPRE se corta el de temperatura corporal, le precede oximetro de pulso
// temp corp = 2.50 segs
// spo2 = 2.40 segs
// ecg = 2.52



//#define PIT_1_1_TICKS USEC_TO_COUNT(3000000U, PIT_1_CLK_FREQ) - 1U // 3 segs
// En este loop funciona todos menos el primero porque justamente no tiene algun predecesor que haya reproducido en
// al menos 1 segundos antes

//#define PIT_1_1_TICKS USEC_TO_COUNT(5000000U, PIT_1_CLK_FREQ) - 1U // con 5 segundos a veces se cortan a la mitad los audios
//#define PIT_1_1_TICKS USEC_TO_COUNT(3000000U, PIT_1_CLK_FREQ) - 1U
//#define PIT_1_1_TICKS USEC_TO_COUNT(500U, PIT_1_CLK_FREQ) - 1U
//#define PIT_1_1_TICKS USEC_TO_COUNT(100U, PIT_1_CLK_FREQ) - 1U


#define PIT_1_2_TICKS USEC_TO_COUNT(12000U, PIT_1_CLK_FREQ) - 1U
//Con audacity le puse microfono y lo fui ajustando hasta que dio de longitud parecida

/* PIT_1 interrupt vector ID (number). */
#define PIT_1_0_IRQN PIT0_IRQn
/* PIT_1 interrupt handler identifier. */
#define PIT_1_0_IRQHANDLER PIT0_IRQHandler

/* PIT_1 interrupt vector ID (number). */
#define PIT_1_1_IRQN PIT1_IRQn
/* PIT_1 interrupt handler identifier. */
#define PIT_1_1_IRQHANDLER PIT1_IRQHandler

/* PIT_1 interrupt vector ID (number). */
#define PIT_1_2_IRQN PIT2_IRQn
/* PIT_1 interrupt handler identifier. */
#define PIT_1_2_IRQHANDLER PIT2_IRQHandler


void BOARD_InitPeripherals(void);
/**
 * @brief 	Initialize peripherals specific settings.
 */
void BOARD_InitBootPeripherals(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _PERIPHERALS_H_ */


