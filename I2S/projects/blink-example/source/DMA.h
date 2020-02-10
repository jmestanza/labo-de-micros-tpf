/*
 * DMA.h
 *
 *  Created on: 26 Oct 2019
 *      Author: Marcelo
 */

#ifndef DMA_H_
#define DMA_H_

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void DMA0_Config(void(*funcallback)(void));

void DMA0_ConfigPingPongBuffer(uint32_t N, uint32_t L, uint32_t * src_buff_address, uint32_t * i2s_tx0_reg_address);


void DMA0_EnableRequest(void);

void DMA0_DisableRequest(void);

#endif /* DMA_H_ */
