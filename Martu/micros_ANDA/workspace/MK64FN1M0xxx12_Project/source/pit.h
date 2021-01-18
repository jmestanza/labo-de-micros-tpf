/*
 * pit.h
 *
 *  Created on: Jan 15, 2021
 *      Author: martinamaspero
 */

#ifndef PIT_H_
#define PIT_H_

#include <stdint.h>
#include <stdbool.h>


#define DEMO_PIT_BASEADDR PIT
#define DEMO_PIT_CHANNEL  kPIT_Chnl_0
#define PIT_LED_HANDLER   PIT0_IRQHandler
#define PIT_IRQ_ID        PIT0_IRQn
/* Get source clock for PIT driver */
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
#define LED_INIT()       LED_RED_INIT(LOGIC_LED_ON)
#define LED_TOGGLE()     LED_RED_TOGGLE()

typedef void (* pfunc) (void);

void init_pit (uint32_t timeInUSEC, pfunc callback);


#endif /* PIT_H_ */
