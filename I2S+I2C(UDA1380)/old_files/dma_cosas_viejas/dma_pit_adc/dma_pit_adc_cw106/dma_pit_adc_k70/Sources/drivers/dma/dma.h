/*
 * dma.h
 *
 *  Created on: Nov 25, 2014
 *      Author: Manuel Alejandro
 */

#ifndef DMA_H_
#define DMA_H_

#include "derivative.h"

#define  ADC_READS 8
uint16_t  value[ADC_READS];

void dma_init(void);

#endif /* DMA_H_ */
