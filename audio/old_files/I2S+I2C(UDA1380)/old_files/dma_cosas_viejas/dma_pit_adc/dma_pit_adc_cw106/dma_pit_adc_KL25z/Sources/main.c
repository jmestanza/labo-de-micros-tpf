/*
 * DMA-PIT-ADC
 * 
 * The following example uses a PIT to start an adc conversion, 
 * once the conversion has finished it issues a DMA request and 
 * the DMA controller stores the converted value in a buffer.
 * 
 * Additional considerations:
 * Since the DMA controller is being implemented with a circular buffer
 * and this requires the address of the buffer to be 0 modular 
 * with respect the initial address, therefore a buffer with an absolute
 * address is being declared as extern in dma.h and its address is being set
 * at the linker file Project_Settings/Linker_Files/MKL25Z128_flash.ld
 * as follows 
 * 
  Define output sections 
	SECTIONS
	{
  	  Set absolute address for variable
  	  value = 0x20001000;
 * 
 * Tested on Freedom KL25z
 * 
 * By : Technical Information Center - Freescale
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "adc.h"
#include "pit.h"
#include "dma.h"

extern char ready;

int main(void)
{
	int i, avg;
	avg = 0;
	ready = 0;
	
	dma_init();
	adc_init();
	pit_init();
	
	for(;;) 
	{	   
		if(ready > ADC_READS)
		{
			for(i = 0; i < ADC_READS; i++)
				avg += value[i];
			avg /= ADC_READS;
			
			ready = 0;
		}
	}
	
	return 0;
}
