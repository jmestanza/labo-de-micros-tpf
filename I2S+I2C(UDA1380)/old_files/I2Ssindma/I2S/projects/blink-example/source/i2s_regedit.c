
#include "i2s_regedit.h"

static I2S_Type * i2s_ptr = I2S0;

void tx_set_reg_0(i2sx_tx_rx_cr0_config * reg_0_tx){
	i2s_ptr->TCSR &=~(I2S_TCSR_TE_MASK |
					  I2S_TCSR_STOPE_MASK |
					  I2S_TCSR_DBGE_MASK |
					  I2S_TCSR_BCE_MASK |
					  I2S_TCSR_FR_MASK |
					  I2S_TCSR_SR_MASK |
					  I2S_TCSR_WSIE_MASK |
					  I2S_TCSR_SEIE_MASK |
					  I2S_TCSR_FEIE_MASK |
					  I2S_TCSR_FWIE_MASK |
					  I2S_TCSR_FRIE_MASK |
					  I2S_TCSR_FWDE_MASK |
					  I2S_TCSR_FRDE_MASK) ;

	i2s_ptr->TCSR|= I2S_TCSR_TE(reg_0_tx->rx_tx_enable) |
					I2S_TCSR_STOPE(reg_0_tx->stop_enable) |
					I2S_TCSR_DBGE(reg_0_tx->debug_enable) |
					I2S_TCSR_BCE(reg_0_tx->bit_clock_enable) |
					I2S_TCSR_FR(reg_0_tx->fifo_reset) |
					I2S_TCSR_SR(reg_0_tx->software_reset) |
					I2S_TCSR_WSIE(reg_0_tx->word_start_interrupt_enable) |
					I2S_TCSR_SEIE(reg_0_tx->sync_error_interrupt_enable) |
					I2S_TCSR_FEIE(reg_0_tx->fifo_error_interrupt_enable) |
					I2S_TCSR_FWIE(reg_0_tx->fifo_warning_interrupt_enable) |
					I2S_TCSR_FRIE(reg_0_tx->fifo_request_interrupt_enable) |
					I2S_TCSR_FWDE(reg_0_tx->fifo_warning_dma_enable) |
					I2S_TCSR_FRDE(reg_0_tx->fifo_request_dma_enable);
}

void tx_set_reg_1(i2sx_tx_rx_cr1_config * reg_1_tx){
	i2s_ptr->TCR1 = ~(I2S_TCR1_TFW_MASK);
	i2s_ptr->TCR1 |=  I2S_TCR1_TFW(reg_1_tx->transmit_fifo_watermark);
}
// MCLK Control Register (MCR[MICS]) and MDR also
void tx_set_reg_2(i2sx_tx_rx_cr2_config * reg_2_tx){
	i2s_ptr->TCR2 = ~(I2S_TCR2_SYNC_MASK |
					  I2S_TCR2_BCS_MASK |
					  I2S_TCR2_BCI_MASK |
					  I2S_TCR2_MSEL_MASK |
					  I2S_TCR2_BCP_MASK |
					  I2S_TCR2_BCD_MASK |
					  I2S_TCR2_DIV_MASK);

	i2s_ptr->TCR2 |=  I2S_TCR2_SYNC(reg_2_tx->synchronous_mode) |
					  I2S_TCR2_BCS(reg_2_tx->bit_clock_swap) |
					  I2S_TCR2_BCI(reg_2_tx->bit_clock_input) |
					  I2S_TCR2_MSEL(reg_2_tx->master_clock_select) |
					  I2S_TCR2_BCP(reg_2_tx->bit_clock_polarity) |
					  I2S_TCR2_BCD(reg_2_tx->bit_clock_direction) |
					  I2S_TCR2_DIV(reg_2_tx->bit_clock_divide);
}

void tx_set_reg_3(i2sx_tx_rx_cr3_config * reg_3_tx){
	i2s_ptr->TCR3 = ~(I2S_TCR3_TCE_MASK |
					  I2S_TCR3_WDFL_MASK);

	i2s_ptr->TCR3 |=  I2S_TCR3_TCE_MASK |  // set both channels
					  I2S_TCR3_WDFL(reg_3_tx->word_flag_configuration);
}

void tx_set_reg_4(i2sx_tx_rx_cr4_config * reg_4_tx){
	i2s_ptr->TCR4 = ~(I2S_TCR4_FRSZ_MASK |
					  I2S_TCR4_SYWD_MASK |
					  I2S_TCR4_MF_MASK |
					  I2S_TCR4_FSE_MASK |
					  I2S_TCR4_FSP_MASK |
					  I2S_TCR4_FSD_MASK);

	i2s_ptr->TCR4 |=  I2S_TCR4_FRSZ(reg_4_tx->frame_size) |
					  I2S_TCR4_SYWD(reg_4_tx->sync_width) |
					  I2S_TCR4_MF(reg_4_tx->is_MSB_first) |
					  I2S_TCR4_FSE(reg_4_tx->frame_sync_early) |
					  I2S_TCR4_FSP(reg_4_tx->frame_sync_polarity) |
					  I2S_TCR4_FSD(reg_4_tx->frame_sync_direction);
}


void tx_set_reg_5(i2sx_tx_rx_cr5_config * reg_5_tx){

	i2s_ptr->TCR5 = ~(I2S_TCR5_WNW_MASK |
					  I2S_TCR5_W0W_MASK |
					  I2S_TCR5_FBT_MASK);

	i2s_ptr->TCR5 |= I2S_TCR5_WNW(reg_5_tx->word_n_width) |
					 I2S_TCR5_W0W(reg_5_tx->word_0_width) |
					 I2S_TCR5_FBT(reg_5_tx->first_bit_shifted);
}
