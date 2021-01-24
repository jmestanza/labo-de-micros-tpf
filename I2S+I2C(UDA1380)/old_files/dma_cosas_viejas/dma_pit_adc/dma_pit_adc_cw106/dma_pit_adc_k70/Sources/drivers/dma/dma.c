/*
 * dma.c
 *
 *  Created on: Nov 25, 2014
 *      Author: Manuel Alejandro
 */
#include "dma.h"

/*	dma_init()
 * Initializes the DMA module to read the ADC results every time a conversion has
 * finished and stores its value in a buffer
 * 
 * */
void dma_init(void)
{
	// Enable clock for DMAMUX and DMA
	SIM_SCGC6 |= SIM_SCGC6_DMAMUX0_MASK;
	SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;	
			
	// Enable Channel 0 and set ADC1 as DMA request source 
	DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(41);

	// Enable request signal for channel 0 
	DMA_ERQ = DMA_ERQ_ERQ0_MASK;
		
	// Set memory address for source and destination 
	DMA_TCD0_SADDR = (uint32_t)&ADC1_RA;
	DMA_TCD0_DADDR = (uint32_t)&value;

	// Set an offset for source and destination address
	DMA_TCD0_SOFF = 0x00; // Source address offset of 2 bits per transaction
	DMA_TCD0_DOFF = 0x02; // Destination address offset of 1 bit per transaction
		
	// Set source and destination data transfer size
	DMA_TCD0_ATTR = DMA_ATTR_SSIZE(1) | DMA_ATTR_DSIZE(1);
		
	// Number of bytes to be transfered in each service request of the channel
	DMA_TCD0_NBYTES_MLNO = 0x02;
		
	// Current major iteration count (a single iteration of 5 bytes)
	DMA_TCD0_CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(ADC_READS); 
	DMA_TCD0_BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(ADC_READS);
	
	// Adjustment value used to restore the source and destiny address to the initial value
	DMA_TCD0_SLAST = 0x00;		// Source address adjustment
	DMA_TCD0_DLASTSGA = -0x10;	// Destination address adjustment
	
	// Setup control and status register
	DMA_TCD0_CSR = 0;
}
