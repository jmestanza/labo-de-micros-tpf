/*
 * i2s.c
 *
 *  Created on: Feb 1, 2020
 *      Author: joa-m
 */

#include "i2s.h"


I2S_Type * i2s_ptr = I2S0;

void tx_set_reg_0(i2sx_tx_rx_cr0_config * reg_0_tx){
	i2s_ptr->TCSR = ~(I2S_TCSR_TE_MASK) & I2S_TCSR_TE(reg_0_tx->rx_tx_enable);
	i2s_ptr->TCSR = ~(I2S_TCSR_STOPE_MASK) & I2S_TCSR_STOPE(reg_0_tx->stop_enable);
	i2s_ptr->TCSR = ~(I2S_TCSR_DBGE_MASK) & I2S_TCSR_DBGE(reg_0_tx->debug_enable);
	i2s_ptr->TCSR = ~(I2S_TCSR_BCE_MASK) & I2S_TCSR_BCE(reg_0_tx->bit_clock_enable);
	i2s_ptr->TCSR = ~(I2S_TCSR_FR_MASK) & I2S_TCSR_FR(reg_0_tx->fifo_reset);
	i2s_ptr->TCSR = ~(I2S_TCSR_SR_MASK) & I2S_TCSR_SR(reg_0_tx->software_reset);
	i2s_ptr->TCSR = ~(I2S_TCSR_WSIE_MASK) & I2S_TCSR_WSIE(reg_0_tx->word_start_interrupt_enable);
	i2s_ptr->TCSR = ~(I2S_TCSR_SEIE_MASK) & I2S_TCSR_SEIE(reg_0_tx->sync_error_interrupt_enable);
	i2s_ptr->TCSR = ~(I2S_TCSR_FEIE_MASK) & I2S_TCSR_FEIE(reg_0_tx->fifo_error_interrupt_enable);
	i2s_ptr->TCSR = ~(I2S_TCSR_FWIE_MASK) & I2S_TCSR_FWIE(reg_0_tx->fifo_warning_interrupt_enable);
	i2s_ptr->TCSR = ~(I2S_TCSR_FRIE_MASK) & I2S_TCSR_FRIE(reg_0_tx->fifo_request_interrupt_enable);
	i2s_ptr->TCSR = ~(I2S_TCSR_FWDE_MASK) & I2S_TCSR_FWDE(reg_0_tx->fifo_warning_dma_enable);
	i2s_ptr->TCSR = ~(I2S_TCSR_FRDE_MASK) & I2S_TCSR_FRDE(reg_0_tx->fifo_request_dma_enable);
}

void tx_set_reg_1(i2sx_tx_rx_cr1_config * reg_1_tx){
	i2s_ptr->TCR1 = ~(I2S_TCR1_TFW_MASK) & I2S_TCR1_TFW(reg_1_tx->transmit_fifo_watermark);
}

void tx_set_reg_2(i2sx_tx_rx_cr2_config * reg_2_tx){
	i2s_ptr->TCR2 = ~(I2S_TCR2_SYNC_MASK) & I2S_TCR2_SYNC(reg_2_tx->synchronous_mode);
	i2s_ptr->TCR2 = ~(I2S_TCR2_BCS_MASK) & I2S_TCR2_BCS(reg_2_tx->bit_clock_swap);
	i2s_ptr->TCR2 = ~(I2S_TCR2_BCI_MASK) & I2S_TCR2_BCI(reg_2_tx->bit_clock_input);
	i2s_ptr->TCR2 = ~(I2S_TCR2_MSEL_MASK) & I2S_TCR2_MSEL(reg_2_tx->master_clock_select);
	i2s_ptr->TCR2 = ~(I2S_TCR2_BCP_MASK) & I2S_TCR2_BCP(reg_2_tx->bit_clock_polarity);
	i2s_ptr->TCR2 = ~(I2S_TCR2_BCD_MASK) & I2S_TCR2_BCD(reg_2_tx->bit_clock_direction);
	i2s_ptr->TCR2 = ~(I2S_TCR2_DIV_MASK) & I2S_TCR2_DIV(reg_2_tx->bit_clock_divide);
}

void tx_set_reg_3(i2sx_tx_rx_cr3_config * reg_3_tx){
	uint32_t channels = (reg_3_tx->transmit_channel_1_enable << 16) | (reg_3_tx->transmit_channel_2_enable << 17);
	i2s_ptr->TCR3 = ~(I2S_TCR3_TCE_MASK) & I2S_TCR3_TCE(channels);
	i2s_ptr->TCR3 = ~(I2S_TCR3_WDFL_MASK) & I2S_TCR3_WDFL(reg_3_tx->word_flag_configuration);
}

void tx_set_reg_4(i2sx_tx_rx_cr4_config * reg_4_tx){
	i2s_ptr->TCR4 = ~(I2S_TCR4_FRSZ_MASK) & I2S_TCR4_FRSZ(reg_4_tx->frame_size);
	i2s_ptr->TCR4 = ~(I2S_TCR4_SYWD_MASK) & I2S_TCR4_SYWD(reg_4_tx->sync_width);
	i2s_ptr->TCR4 = ~(I2S_TCR4_MF_MASK) & I2S_TCR4_MF(reg_4_tx->is_MSB_first);
	i2s_ptr->TCR4 = ~(I2S_TCR4_FSE_MASK) & I2S_TCR4_FSE(reg_4_tx->frame_sync_early);
	i2s_ptr->TCR4 = ~(I2S_TCR4_FSP_MASK) & I2S_TCR4_FSP(reg_4_tx->frame_sync_polarity);
	i2s_ptr->TCR4 = ~(I2S_TCR4_FSD_MASK) & I2S_TCR4_FSD(reg_4_tx->frame_sync_direction);
}


void tx_set_reg_5(i2sx_tx_rx_cr5_config * reg_5_tx){
	i2s_ptr->TCR5 = ~(I2S_TCR5_WNW_MASK) & I2S_TCR5_WNW(reg_5_tx->word_n_width);
	i2s_ptr->TCR5 = ~(I2S_TCR5_W0W_MASK) & I2S_TCR5_W0W(reg_5_tx->word_0_width);
	i2s_ptr->TCR5 = ~(I2S_TCR5_FBT_MASK) & I2S_TCR5_FBT(reg_5_tx->first_bit_shifted);
}

void i2s_init(void){
	//clock gating missing
	tx_config_regs tx_cfg;
	tx_cfg.tx_cfg_0_reg.rx_tx_enable = true;
	tx_cfg.tx_cfg_0_reg.stop_enable = true; // transmitter enabled in stop mode
	tx_cfg.tx_cfg_0_reg.debug_enable = true; // transmitter enabled in debug mode
	tx_cfg.tx_cfg_0_reg.bit_clock_enable = true; // if Trans. En. is set, this too. when software clear this field the T.E remains enabled and this bit too until the end of the frame
	tx_cfg.tx_cfg_0_reg.fifo_reset = false; // should reset when Tx is disabled or the fifo error flag is set
	tx_cfg.tx_cfg_0_reg.software_reset = false; // software resets if 1
	//tx_cfg.tx_cfg_0_reg.fifo_error_flag   don't config
	//tx_cfg.tx_cfg_0_reg.fifo_warning_flag  don't config
	//tx_cfg.tx_cfg_0_reg.fifo_request_flag don't config
	tx_cfg.tx_cfg_0_reg.word_start_interrupt_enable = false;
	tx_cfg.tx_cfg_0_reg.sync_error_interrupt_enable = false;
	tx_cfg.tx_cfg_0_reg.fifo_error_interrupt_enable = false;
	tx_cfg.tx_cfg_0_reg.fifo_warning_interrupt_enable = false;
	tx_cfg.tx_cfg_0_reg.fifo_request_interrupt_enable = false;
	tx_cfg.tx_cfg_0_reg.fifo_warning_dma_enable = false;
	tx_cfg.tx_cfg_0_reg.fifo_request_dma_enable = false;

	tx_set_reg_0(& tx_cfg.tx_cfg_0_reg);

	tx_cfg.tx_cfg_1_reg.transmit_fifo_watermark = 7; // max value just in case

	tx_set_reg_1(& tx_cfg.tx_cfg_1_reg);

	tx_cfg.tx_cfg_2_reg.synchronous_mode = asynchronous_mode; // ??
	tx_cfg.tx_cfg_2_reg.bit_clock_swap = false; // use the normal bit clock source
	tx_cfg.tx_cfg_2_reg.bit_clock_input = false; //??
	tx_cfg.tx_cfg_2_reg.master_clock_select = bus_clock;
	tx_cfg.tx_cfg_2_reg.bit_clock_polarity = false; // clk active high when => out rising edge and sample input falling edge
	tx_cfg.tx_cfg_2_reg.bit_clock_direction = true; // bit clk is gneerated internally in Master mode
	tx_cfg.tx_cfg_2_reg.bit_clock_divide = 0; // divides by (DIV+1)*2

	tx_set_reg_2(& tx_cfg.tx_cfg_2_reg);

	tx_cfg.tx_cfg_3_reg.transmit_channel_1_enable = true;
	tx_cfg.tx_cfg_3_reg.transmit_channel_2_enable = true;
	tx_cfg.tx_cfg_3_reg.word_flag_configuration = 0; // ??

	tx_set_reg_3(& tx_cfg.tx_cfg_3_reg);

	tx_cfg.tx_cfg_4_reg.frame_size = 0; // (N+1) word per frame, in this case 1 word per frame
	tx_cfg.tx_cfg_4_reg.sync_width = 0; // ??
	tx_cfg.tx_cfg_4_reg.is_MSB_first = true;
	tx_cfg.tx_cfg_4_reg.frame_sync_early = false; // frame sync asserts with first bit of the frame
	tx_cfg.tx_cfg_4_reg.frame_sync_polarity = false; // frame sync active high
	tx_cfg.tx_cfg_4_reg.frame_sync_direction = true ; // frame sync is generated internally in master mode

	tx_set_reg_4(& tx_cfg.tx_cfg_4_reg);

	tx_cfg.tx_cfg_5_reg.word_n_width = 15; // (N-1)number of bits in each word except for the first
	tx_cfg.tx_cfg_5_reg.word_0_width = 15;
	tx_cfg.tx_cfg_5_reg.first_bit_shifted = 0;

	tx_set_reg_5(& tx_cfg.tx_cfg_5_reg);
}
