/*
 * DMA.c
 *
 *  Created on: 26 Oct 2019
 *      Author: Marcelo
 */

#include "DMA.h"
#include "gpio.h"
#include "board.h"
#include "hardware.h"

void (*LoopCallback)(void);
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void DMA0_Config(void(*funcallback)(void))
{
	LoopCallback = funcallback;

	/* Enable the clock for the eDMA and the DMAMUX. */
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
	/* Enable the eDMA channel 0 and set the FTM CH0 as the DMA request source. */
	DMAMUX->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(13);   // I2STx
	// the weird mask is kDmaRequestMux0I2S0Tx
	/* Enable the interrupts for the channel 0. */

	/* Clear all the pending events. */
	NVIC_ClearPendingIRQ(DMA0_IRQn);
	/* Enable the DMA interrupts. */
	NVIC_EnableIRQ(DMA0_IRQn);

	/// ============= INIT TCD0 ===================//
	//uint32_t a[] = {1,2,3,4};
	//uint32_t b[] = {1,2,3,4};
	//uint32_t N = 100;
	//uint32_t L = 4; // 4 bytes (?)
//	DMA0_ConfigPingPongBuffer(N, L, &a[0], &b[0]);

	/* Set source and destination data transfer size to 16 bits (CnV is 2 bytes wide). */
	//DMA0->TCD[0].ATTR = DMA_ATTR_SSIZE(0) | DMA_ATTR_DSIZE(0);

}

void DMA0_ConfigPingPongBuffer(uint32_t N, uint32_t L, uint32_t * src_buff_address, uint32_t * i2s_tx0_reg_address){
//    Each block has N samples, and each sample has L bytes.
//    1. Minor loop offset is enabled and it is applied to source address only and the value is L – [(N *2) * (L*2)].
//    2. Minor loop transfer count is set to L*2.
//    3. Source address is initialized to buffer base address.
//    4. Source address offset after each write is set to N* L*2.
//    5. Source address offset for the end of major loop is set to –[(N*2) * (L*3) – L].
//    6. Destination address is initialized and fixed to the I2S_TX0 register.
//    7. Destination address offset after each write is set to 0.
//    8. Destination address offset for the end of major loop is set to 0.
//    9. Major loop count is set to N*2.
//    10. Enable half interrupt.
    DMA0->TCD[0].SOFF = L - ((N *2) * (L*2));  // signed source adress offset
    DMA0->TCD[0].NBYTES_MLNO = L*2; // Minor Byte Count (Minor Loop Disabled)
    DMA0->TCD[0].SADDR = (uint32_t)src_buff_address;
    DMA0->TCD[0].SOFF = N* L *2 ;
    DMA0->TCD[0].SLAST = -((N*2) * (L*3) - L);
    DMA0->TCD[0].DADDR = (uint32_t)i2s_tx0_reg_address;
    DMA0->TCD[0].DOFF = 0;
    DMA0->TCD[0].DLAST_SGA = 0;
    DMA0->TCD[0].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(N*2);
    DMA0->TCD[0].CSR = DMA_CSR_INTHALF(1); // The half-point interrupt is enabled.
//	DMA0->TCD[channel].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(source_full_size/source_unit_size);
//  CITER not configured because I think it's not necessary (would have the same value of BITER)
}

void DMA0_EnableRequest(void)
{
	/* Enable request signal for channel 0. */
	DMA0->ERQ &= ~(DMA_ERQ_ERQ0_MASK);
	DMA0->ERQ |= DMA_ERQ_ERQ0(1);

	DMAMUX->CHCFG[0] &= ~(DMAMUX_CHCFG_ENBL_MASK);
	DMAMUX->CHCFG[0] |= DMAMUX_CHCFG_ENBL(1);
}

void DMA0_DisableRequest(void)
{
	/* Enable request signal for channel 0. */
	DMA0->ERQ &= ~(DMA_ERQ_ERQ0_MASK);
	DMA0->ERQ |= DMA_ERQ_ERQ0(0);

	DMAMUX->CHCFG[0] &= ~(DMAMUX_CHCFG_ENBL_MASK);
	DMAMUX->CHCFG[0] |= DMAMUX_CHCFG_ENBL(0);
}

void DMA0_IRQHandler()
{
	//gpioToggle(PIN_GPIO_TEST);
	/* Clear the interrupt flag. */
	DMA0->CINT |= 0;
	LoopCallback();
	//gpioToggle(PIN_GPIO_TEST);
}

/*******************************************************************************
 ******************************************************************************/
