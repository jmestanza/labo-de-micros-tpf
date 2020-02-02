/*
 * i2s.h
 *
 *  Created on: Feb 1, 2020
 *      Author: joa-m
 */

#ifndef I2S_H_
#define I2S_H_

#include "MK64F12.h"

typedef struct {
	bool rx_tx_enable;
	bool stop_enable;
	bool debug_enable;
	bool bit_clock_enable;
	bool fifo_reset;
	bool software_reset;
	bool word_start_flag;
	bool sync_error_flag;
	bool fifo_error_flag;
	bool fifo_warning_flag;
	bool fifo_request_flag;
	bool word_start_interrupt_enable;
	bool sync_error_interrupt_enable;
	bool fifo_error_interrupt_enable;
	bool fifo_warning_interrupt_enable;
	bool fifo_request_interrupt_enable;
	bool fifo_warning_dma_enable;
	bool fifo_request_dma_enable;
} i2sx_tx_rx_cr0_config;

typedef struct {
	uint32_t transmit_fifo_watermark; // (3 bits - maxn 7)
} i2sx_tx_rx_cr1_config;

typedef struct {
	uint32_t synchronous_mode; // 2 bits
	bool bit_clock_swap;
	bool bit_clock_input;
	bool master_clock_select; // 2 bits, tiene config para BusClockSelected
	bool bit_clock_polarity;
	bool bit_clock_direction;
	uint8_t bit_clock_divide; // 8 bits
} i2sx_tx_rx_cr2_config;

typedef struct {
	bool transmit_channel_1_enable; // bit 17
	bool transmit_channel_2_enable; // bit 16
	uint32_t word_flag_configuration;  // 5 bits
} i2sx_tx_rx_cr3_config;

typedef struct {
	uint32_t frame_size; // bits 20-16 incluidos / 5 bits
	uint32_t sync_width; // 5 bits
	bool is_MSB_first;
	bool frame_sync_early;
	bool frame_sync_polarity;
	bool frame_sync_direction;
} i2sx_tx_rx_cr4_config;

typedef struct {
	uint32_t word_n_width; // 5 bits
	uint32_t word_0_width; // 5 bits
	uint32_t first_bit_shifted; // 5 bits

} i2sx_tx_rx_cr5_config;

typedef struct {
	uint32_t write_fifo_pointer;
	uint32_t read_fifo_pointer;
} i2sx_tx_rx_fr_n;

typedef struct {
	i2sx_tx_rx_cr0_config tx_cfg_0_reg; // (3 bits - maxn 7)
	i2sx_tx_rx_cr1_config tx_cfg_1_reg;
	i2sx_tx_rx_cr2_config tx_cfg_2_reg;
	i2sx_tx_rx_cr3_config tx_cfg_3_reg;
	i2sx_tx_rx_cr4_config tx_cfg_4_reg;
	i2sx_tx_rx_cr5_config tx_cfg_5_reg;
	uint32_t tx_data_reg[2];
	i2sx_tx_rx_fr_n tx_fifo_reg[2];
	uint32_t tx_word_mask_reg; // 0 = word N enabled ; 1 = word N is masked (The transmit data pins are tri-stated when masked.)
} tx_config_regs;

typedef struct {
	i2sx_tx_rx_cr0_config rx_cfg_0_reg; // (3 bits - maxn 7)
	i2sx_tx_rx_cr1_config rx_cfg_1_reg;
	i2sx_tx_rx_cr2_config rx_cfg_2_reg;
	i2sx_tx_rx_cr3_config rx_cfg_3_reg;
	i2sx_tx_rx_cr4_config rx_cfg_4_reg;
	i2sx_tx_rx_cr5_config rx_cfg_5_reg;
	uint32_t rx_data_reg[2];
	i2sx_tx_rx_fr_n rx_fifo_reg[2];
	uint32_t rx_word_mask_reg; // 0 = word N enabled ; 1 = word N is masked (The transmit data pins are tri-stated when masked.)
} rx_config_regs;

typedef struct{
	bool driver_update_flag;
	bool MCLK_output_enable;
	uint32_t MCLK_input_clk_select;// 2 bits
} i2sx_mclk_ctrl_reg;

typedef struct {
	uint8_t fract; // 8 bits
	uint32_t divide; // 12 bits
} i2sx_mclk_div_reg;



void i2s_init(void);


#endif /* I2S_H_ */
