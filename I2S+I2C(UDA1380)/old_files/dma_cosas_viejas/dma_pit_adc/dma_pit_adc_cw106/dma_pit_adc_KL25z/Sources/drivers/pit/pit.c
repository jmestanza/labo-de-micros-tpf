/*
 * pit.c
 *
 *  Created on: Nov 25, 2014
 *      Author: Manuel Alejandro
 */

#include "pit.h"

/* Initializes the PIT module to produce an interrupt every second
 * 
 * */
void pit_init(void)
{
	// Enable PIT clock
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
	
	// Enable Green LED clock and MUX
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB_PCR19 = PORT_PCR_MUX(1);
	GPIOB_PDDR |= (1 << LED_GREEN);
	GPIOB_PSOR |= (1 << LED_GREEN);
	
	// Turn on PIT
	PIT_MCR = 0;
	
	// Configure PIT to produce an interrupt every 1s
	PIT_LDVAL0 = 0x1312CFF;	// 1/20Mhz = 50ns   (1s/50ns)-1= 19,999,999 cycles or 0x1312CFF
	PIT_TCTRL0 |= PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK; // Enable interrupt and enable timer
	
	// Enable interrupt registers ISER and ICPR
	enable_irq(INT_PIT - 16);
}

/*	Handles PIT interrupt if enabled
 * 
 * 	Starts conversion in ADC0 with single ended channel 8 (PTB0) as input
 * 
 * */
void PIT_IRQHandler(void)
{	
	// Clear interrupt
	PIT_TFLG0 = PIT_TFLG_TIF_MASK;
	
	// Write to SC1A to start conversion with channel 8 PTB0
	ADC0_SC1A = (ADC_SC1_ADCH(ADC_CHANNEL) | 
				 (ADC0_SC1A & (ADC_SC1_AIEN_MASK | ADC_SC1_DIFF_MASK)));  
	
	// Toggle Green LED
	GPIOB_PTOR = (1 << LED_GREEN);
}

/***********************************************************************/
/*
 * Initialize the NVIC to enable the specified IRQ.
 * 
 * NOTE: The function only initializes the NVIC to enable a single IRQ. 
 * Interrupts will also need to be enabled in the ARM core. This can be 
 * done using the EnableInterrupts macro.
 *
 * Parameters:
 * irq    irq number to be enabled (the irq number NOT the vector number)
 */

void enable_irq (int irq)
{   
    /* Make sure that the IRQ is an allowable number. Up to 32 is 
     * used.
     *
     * NOTE: If you are using the interrupt definitions from the header
     * file, you MUST SUBTRACT 16!!!
     */
    if (irq > 32)
    	__asm("bkpt"); // Invalid IRQ value passed to enable irq function!
    else
    {
      /* Set the ICPR and ISER registers accordingly */
      NVIC_ICPR |= 1 << (irq%32);
      NVIC_ISER |= 1 << (irq%32);
    }
}
