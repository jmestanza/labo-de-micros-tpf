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

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Peripherals v1.0
* BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/**
 * @file    peripherals.c
 * @brief   Peripherals initialization file.
 */
 
/* This is a template for board specific configuration created by MCUXpresso IDE Project Wizard.*/

#include "peripherals.h"

const pit_config_t PIT_1_config = {
  .enableRunInDebug = false
};

void PIT_1_init(void) {
  /* Initialize the PIT. */
  PIT_Init(PIT_1_PERIPHERAL, &PIT_1_config);
  /* Set channel 0 period. */
  PIT_SetTimerPeriod(PIT_1_PERIPHERAL, kPIT_Chnl_0, PIT_1_0_TICKS);

  PIT_SetTimerPeriod(PIT_1_PERIPHERAL, kPIT_Chnl_1, PIT_1_1_TICKS);
  /* Set channel 2 period */
  PIT_SetTimerPeriod(PIT_1_PERIPHERAL, kPIT_Chnl_2, PIT_1_2_TICKS);

  /* Enable interrupts from channel 0. */
  PIT_EnableInterrupts(PIT_1_PERIPHERAL, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

  PIT_EnableInterrupts(PIT_1_PERIPHERAL, kPIT_Chnl_1, kPIT_TimerInterruptEnable);

  PIT_EnableInterrupts(PIT_1_PERIPHERAL, kPIT_Chnl_2, kPIT_TimerInterruptEnable);

  /* Enable interrupt PIT_1_0_IRQN request in the NVIC */
  EnableIRQ(PIT_1_0_IRQN);
  /* Enable interrupt PIT_1_2_IRQN request in the NVIC */
  EnableIRQ(PIT_1_1_IRQN);
  /* Enable interrupt PIT_1_2_IRQN request in the NVIC */
  EnableIRQ(PIT_1_2_IRQN);

}


/**
 * @brief Set up and initialize all required blocks and functions related to the peripherals hardware.
 */

void BOARD_InitPeripherals(void)
{
  /* Initialize components */
  PIT_1_init();
}

void BOARD_InitBootPeripherals(void) {
	/* The user initialization should be placed here */
	  BOARD_InitPeripherals();
}
