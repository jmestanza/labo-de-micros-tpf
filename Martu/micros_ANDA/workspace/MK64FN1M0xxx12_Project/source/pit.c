/*
 * pit.c
 *
 *  Created on: Jan 15, 2021
 *      Author: martinamaspero
 */

#include "pit.h"
#include "fsl_pit.h"
#include "clock_config.h"


pfunc this_callback;

void init_pit (uint32_t timeInUSEC, pfunc callback)
{
    pit_config_t pitConfig;
    this_callback = callback;

    /*
     * pitConfig.enableRunInDebug = false;
     */
    PIT_GetDefaultConfig(&pitConfig);

    /* Init pit module */
    PIT_Init(DEMO_PIT_BASEADDR, &pitConfig);

    /* Set timer period for channel 0 */
    PIT_SetTimerPeriod(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL, USEC_TO_COUNT(timeInUSEC, PIT_SOURCE_CLOCK));

    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL, kPIT_TimerInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(PIT_IRQ_ID);

    /* Start channel 0 */
    PIT_StartTimer(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL);

}

void PIT_LED_HANDLER(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL, kPIT_TimerFlag);
    this_callback();

    /* Added for, and affects, all PIT handlers. For CPU clock which is much larger than the IP bus clock,
     * CPU can run out of the interrupt handler before the interrupt flag being cleared, resulting in the
     * CPU's entering the handler again and again. Adding DSB can prevent the issue from happening.
     */
    __DSB();
}


