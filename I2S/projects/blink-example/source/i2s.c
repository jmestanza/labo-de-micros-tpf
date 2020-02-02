/*
 * i2s.c
 *
 *  Created on: Feb 1, 2020
 *      Author: joa-m
 */

#include "i2s.h"


I2S_Type * i2s_ptr = I2S0;



tx_set_reg_0(i2sx_tx_rx_cr0_config * reg_0_tx){
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



i2s_init(){
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




}
