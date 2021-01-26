/*
 * pit.c
 *
 *  Created on: Jan 15, 2021
 *      Author: martinamaspero
 */

#include "pit.h"
#include "fsl_pit.h"
#include "clock_config.h"



pfunc callback_0;
pfunc callback_1;
pfunc callback_2;

void init_pit (uint8_t ch, uint32_t timeInUSEC)
{
    pit_config_t pitConfig;
    /*
     * pitConfig.enableRunInDebug = false;
     */
    PIT_GetDefaultConfig(&pitConfig);

    /* Init pit module */
    PIT_Init(PIT_BASEADDR, &pitConfig);

    /* Set timer period for channel */
    PIT_SetTimerPeriod(PIT_BASEADDR, ch, USEC_TO_COUNT(timeInUSEC, PIT_SOURCE_CLOCK));

    /* Start channel */
    PIT_StartTimer(PIT_BASEADDR, ch);

}

void pit_enable_int (uint8_t ch, pfunc callback)
{
	PIT_ClearStatusFlags(PIT_BASEADDR, ch, kPIT_TimerFlag);
    /* Enable timer interrupts for channel  */
    PIT_EnableInterrupts(PIT_BASEADDR, ch, kPIT_TimerInterruptEnable);

	switch (ch) {
		case 0:
			NVIC_EnableIRQ(PIT0_IRQn);
			callback_0 = callback;
			break;
		case 1:
			NVIC_EnableIRQ(PIT1_IRQn);
			callback_1 = callback;
			break;
		case 2:
			NVIC_EnableIRQ(PIT2_IRQn);
			callback_2 = callback;
			break;
	}
}

void PIT0_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT_BASEADDR, 0, kPIT_TimerFlag);
    callback_0();

    /* Added for, and affects, all PIT handlers. For CPU clock which is much larger than the IP bus clock,
     * CPU can run out of the interrupt handler before the interrupt flag being cleared, resulting in the
     * CPU's entering the handler again and again. Adding DSB can prevent the issue from happening.
     */
    __DSB();
}

void PIT1_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT_BASEADDR, 1, kPIT_TimerFlag);
    callback_1();

    /* Added for, and affects, all PIT handlers. For CPU clock which is much larger than the IP bus clock,
     * CPU can run out of the interrupt handler before the interrupt flag being cleared, resulting in the
     * CPU's entering the handler again and again. Adding DSB can prevent the issue from happening.
     */
    __DSB();
}


void PIT2_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT_BASEADDR, 2, kPIT_TimerFlag);
    callback_2();

    /* Added for, and affects, all PIT handlers. For CPU clock which is much larger than the IP bus clock,
     * CPU can run out of the interrupt handler before the interrupt flag being cleared, resulting in the
     * CPU's entering the handler again and again. Adding DSB can prevent the issue from happening.
     */
    __DSB();
}







