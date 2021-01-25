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

#define PIT_BASEADDR PIT
/* Get source clock for PIT driver */
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)


typedef void (* pfunc) (void);

void init_pit (uint8_t ch, uint32_t timeInUSEC);

void pit_enable_int (uint8_t ch, pfunc callback);


#endif /* PIT_H_ */
