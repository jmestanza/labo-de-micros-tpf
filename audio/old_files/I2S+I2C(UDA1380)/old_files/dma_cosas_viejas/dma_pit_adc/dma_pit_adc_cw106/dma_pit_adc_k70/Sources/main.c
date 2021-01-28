/*
 * DMA-PIT-ADC
 * 
 * The following example uses a PIT to start an adc conversion, 
 * once the conversion has finished it issues a DMA request and 
 * the DMA controller stores the converted value in a buffer.
 * 
 * Tested on Freedom K70
 * 
 * By : Technical Information Center - Freescale
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "adc.h"
#include "pit.h"
#include "dma.h"


int main(void)
{
	int counter = 0;
	adc_init();
	pit_init();
	dma_init();
	
	
	
	for(;;) {	   
	   	counter++;
	}
	
	return 0;
}
