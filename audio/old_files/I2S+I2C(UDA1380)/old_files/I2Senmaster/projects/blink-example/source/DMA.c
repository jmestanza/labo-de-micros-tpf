#include "DMA.h"
#include "gpio.h"
#include "board.h"
#include "hardware.h"

void (*processData)(void);



void DMA0_Config(void(*funcallback)(void))
{
	processData = funcallback;

	/* Enable the clock for the eDMA and the DMAMUX. */
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
	/* Enable the eDMA channel 0 and set the FTM CH0 as the DMA request source. */
	DMAMUX->CHCFG[0] = DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(13);   // I2STx
	// the weird mask is kDmaRequestMux0I2S0Tx
	/* Enable the interrupts for the channel 0. */

}


void DMA0_ConfigClassic(uint16_t N, uint32_t L, uint32_t * src_buff_address, uint32_t * i2s_tx0_reg_address){
	/* Clear all the pending events. */
	NVIC_ClearPendingIRQ(DMA0_IRQn);
	uint16_t size_bit;
		switch(L)
		{
		case 1: size_bit = transfer_8bit; break;
		case 2: size_bit = transfer_16bit; break;
		case 4: size_bit = transfer_32bit; break;
		default: size_bit = transfer_32bit; break;
		}

	DMA0->TCD[0].NBYTES_MLOFFNO =  DMA_NBYTES_MLOFFNO_NBYTES(L);  // src minor loop offset enable and applied
    DMA0->TCD[0].SADDR = (uint32_t)src_buff_address;
    DMA0->TCD[0].SOFF = L;
    DMA0->TCD[0].SLAST = 0 ;
    DMA0->TCD[0].DADDR = (uint32_t)i2s_tx0_reg_address;
    DMA0->TCD[0].DOFF = L; // ✓
    DMA0->TCD[0].DLAST_SGA = 0; // ✓
    DMA0->TCD[0].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(N);
    DMA0->TCD[0].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(N);
	DMA0->TCD[0].CSR = DMA_CSR_INTHALF_MASK | DMA_CSR_INTMAJOR_MASK; // The half-point interrupt is enabled.
	NVIC_EnableIRQ(DMA0_IRQn);
	DMA0->TCD[0].ATTR = DMA_ATTR_SSIZE(size_bit) |
						DMA_ATTR_DSIZE(size_bit) |
						DMA_ATTR_SMOD(N) |
						DMA_ATTR_DMOD(N); // => circular buffer with modulus N
}

void DMA0_ConfigPingPongBuffer(uint32_t N, uint32_t L, uint32_t * src_buff_address, uint32_t * i2s_tx0_reg_address){
//    Each block has N samples, and each sample has L bytes.

	uint16_t size_bit;
	switch(L)
	{
	case 1: size_bit = transfer_8bit; break;
	case 2: size_bit = transfer_16bit; break;
	case 4: size_bit = transfer_32bit; break;
	default: size_bit = transfer_32bit; break;
	}

	/* Clear all the pending events. */
	NVIC_ClearPendingIRQ(DMA0_IRQn);


//    1. Minor loop offset is enabled and it is applied to source address only; value: L – [(N *2) * (L*2)].
//and 2. Minor loop transfer count is set to L*2.

	DMA0->TCD[0].NBYTES_MLOFFYES = DMA_NBYTES_MLOFFYES_SMLOE_MASK |
								   DMA_NBYTES_MLOFFYES_MLOFF(L - ((N *2) * (L*2))) |
								   DMA_NBYTES_MLOFFYES_NBYTES(L*2);  // src minor loop offset enable and applied



//    3. Source address is initialized to buffer base address.
    DMA0->TCD[0].SADDR = (uint32_t)src_buff_address;
//    4. Source address offset after each write is set to N* L*2.
    DMA0->TCD[0].SOFF = N* L *2 ;
//    5. Source address offset for the end of major loop is set to –[(N*2) * (L*3) – L].
    DMA0->TCD[0].SLAST = -((N*2) * (L*3) - L); // at the completition of the major it count
//    6. Destination address is initialized and fixed to the I2S_TX0 register.
    DMA0->TCD[0].DADDR = (uint32_t)i2s_tx0_reg_address;

//    7. Destination address offset after each write is set to 0.
    DMA0->TCD[0].DOFF = 0; // ✓
//    8. Destination address offset for the end of major loop is set to 0.
    DMA0->TCD[0].DLAST_SGA = 0; // ✓

//    9. Major loop count is set to N*2.
    DMA0->TCD[0].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(N*2);
    DMA0->TCD[0].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(N*2);

//    10. Enable half interrupt.
	DMA0->TCD[0].CSR = DMA_CSR_INTHALF_MASK | DMA_CSR_INTMAJOR_MASK; // The half-point interrupt is enabled.

	/* Enable the DMA interrupts. */
	NVIC_EnableIRQ(DMA0_IRQn);

	DMA0->SERQ = DMA_SERQ_SERQ(0); // nada

	DMA0->TCD[0].ATTR = DMA_ATTR_SSIZE(size_bit) |
						DMA_ATTR_DSIZE(size_bit) ;
						//| DMA_ATTR_SMOD(2);; // 2 => circular buffer with modulus 4
}

void DMA0_EnableRequest(void)
{
	/* Enable request signal for channel 0. */
	DMA0->ERQ &= ~(DMA_ERQ_ERQ0_MASK);   // esta linea causa todos los problemas
	DMA0->ERQ |= DMA_ERQ_ERQ0(1);        // y esta linea tambien

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

void DMA0_GenerateRequest(void){
	DMA0->TCD[0].CSR |= DMA_CSR_START(1); // automatically cleared by hardware
}

__ISR__ DMA0_IRQHandler(void)
{
	/* Clear the interrupt flag. */
	DMA0->CINT |= 1; // w1c,  all interrupts
	if(DMA0->TCD[0].CITER_ELINKNO == DMA0->TCD[0].BITER_ELINKNO){

	}
	if(DMA0->TCD[0].BITER_ELINKNO % 2 == 0){
		if(DMA0->TCD[0].CITER_ELINKNO == (DMA0->TCD[0].BITER_ELINKNO/2)){
			processData(); // we are in the half
		}
	}else{
		if(DMA0->TCD[0].CITER_ELINKNO == (DMA0->TCD[0].BITER_ELINKNO+1)/2){
			processData(); // we are in the half
		}
	}
}
