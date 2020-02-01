/*
 * i2s.h
 *
 *  Created on: Feb 1, 2020
 *      Author: joa-m
 */

#ifndef I2S_H_
#define I2S_H_



typedef struct{
	bool transmitter_enable;
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
} i2sx_tcsr_config;

typedef struct {
	uint32_t transmit_fifo_watermark; // (3 bits - maxn 7)
} i2sx_tcr1_config;


typedef struct {
	uint32_t synchronous_mode; // 2 bits
	bool bit_clock_swap;
	bool bit_clock_input;
	bool master_clock_select; // 2 bits, tiene config para BusClockSelected
	bool bit_clock_polarity;
	bool bit_clock_direction;
	uint8_t bit_clock_divide; // 8 bits
} i2sx_tcr2_config;

typedef struct {
	bool transmit_channel_1_enable; // bit 17
	bool transmit_channel_2_enable; // bit 16
	uint32_t word_flag_configuration;  // 5 bits
} i2sx_tcr3_config;

typedef struct {
	uint32_t frame_size; // bits 20-16 incluidos / 5 bits
	uint32_t sync_width; // 5 bits
	bool is_MSB_first;
	bool frame_sync_early;
	bool frame_sync_polarity;
	bool frame_sync_direction;
} i2sx_tcr4_config;


#endif /* I2S_H_ */
