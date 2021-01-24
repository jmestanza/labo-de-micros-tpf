/*
 * dma.c
 *
 *  Created on: Nov 25, 2014
 *      Author: Manuel Alejandro
 */
#include "dma.h"

extern char ready;

void dma_init(void)
{
	ready = 0;
	
	// Enable clocks
	SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
	SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;
	
	// Config DMA Mux for ADC operation
	// Disable DMA Mux channel first
	DMAMUX0_CHCFG0 = 0x00;
	
	// Configure DMA
	DMA_SAR0 = (uint32_t)&ADC0_RA;
	DMA_DAR0 = (uint32_t)&value;
	DMA_DSR_BCR0 = DMA_DSR_BCR_BCR(2); // 2 bytes (16 bits) per transfer
	
	DMA_DCR0 |= (DMA_DCR_EINT_MASK|		// Enable interrupt
				 DMA_DCR_ERQ_MASK |		// Enable peripheral request
				 DMA_DCR_CS_MASK  |
				 DMA_DCR_SSIZE(2) |		// Set source size to 16 bits
				 DMA_DCR_DINC_MASK|		// Set increments to destination address
				 DMA_DCR_DMOD(1)  |     // Destination address modulo of 16 bytes
				 DMA_DCR_DSIZE(2));		// Set destination size of 16 bits 
				 
	
	// Enable DMA channel and source
	DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(40); // Enable DMA channel and set ADC0 as source
	
	// Enable interrupt
	enable_irq(INT_DMA0 - 16);
}

/*
 * Handles DMA0 interrupt
 * Resets the BCR register and clears the DONE flag
 * */
void DMA0_IRQHandler(void)
{
	/* Enable DMA0*/ 
	DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;	// Clear Done Flag
	DMA_DSR_BCR0 |= DMA_DSR_BCR_BCR(2);		// Set byte count register
	ready += 1;
}
