/*
 * i2s.c
 *
 *  Created on: Feb 1, 2020
 *      Author: joa-m
 */

#include "i2s.h"

static I2S_Type * i2s_ptr = I2S0;
static SIM_Type* sim_ptr = SIM;
static PORT_Type* portPointers[] = PORT_BASE_PTRS;

bool isWordStartFlag(uint32_t csr){
	return (csr & I2S_TCSR_WSF_MASK) == I2S_TCSR_WSF_MASK;
}

bool isSyncErrorFlag(uint32_t csr){
	return (csr & I2S_TCSR_SEF_MASK) == I2S_TCSR_SEF_MASK;
}

bool isFIFOErrorFlag(uint32_t csr){
	return (csr & I2S_TCSR_FEF_MASK) == I2S_TCSR_FEF_MASK;
}

bool isFIFOWarningFlag(uint32_t csr){
	return (csr & I2S_TCSR_FWF_MASK) == I2S_TCSR_FWF_MASK;
}

bool isFIFORequestFlag(uint32_t csr){
	return (csr & I2S_TCSR_FRF_MASK) == I2S_TCSR_FRF_MASK;
}


__ISR__ I2S0_Tx_IRQHandler(void){
	if(isWordStartFlag(i2s_ptr->TCSR)){
		i2s_ptr->TCSR &= ~I2S_TCSR_WSF_MASK; //  clear the flag
	}
	if(isSyncErrorFlag(i2s_ptr->TCSR)){
		i2s_ptr->TCSR &= ~I2S_TCSR_SEF_MASK; //  clear the sync error flag
	}
	if(isFIFOErrorFlag(i2s_ptr->TCSR)){
		i2s_ptr->TCSR &= ~I2S_TCSR_FEF_MASK; //  clear the fifo error flag
	}
	if(isFIFOWarningFlag(i2s_ptr->TCSR)){
		i2s_ptr->TCSR &= ~I2S_TCSR_FWF_MASK; //  clear the fifo warning flag
	}
	if(isFIFORequestFlag(i2s_ptr->TCSR)){
		i2s_ptr->TCSR &= ~I2S_TCSR_FRF_MASK; //  clear the fifo request flag
	}
}

__ISR__ I2S0_Rx_IRQHandler(void){
	if(isWordStartFlag(i2s_ptr->RCSR)){
		i2s_ptr->RCSR &= ~I2S_RCSR_WSF_MASK; //  clear the flag
	}
	if(isSyncErrorFlag(i2s_ptr->RCSR)){
		i2s_ptr->RCSR &= ~I2S_RCSR_SEF_MASK; //  clear the sync error flag
	}
	if(isFIFOErrorFlag(i2s_ptr->RCSR)){
		i2s_ptr->RCSR &= ~I2S_RCSR_FEF_MASK; //  clear the fifo error flag
	}
	if(isFIFOWarningFlag(i2s_ptr->RCSR)){
		i2s_ptr->RCSR &= ~I2S_RCSR_FWF_MASK; //  clear the fifo warning flag
	}
	if(isFIFORequestFlag(i2s_ptr->RCSR)){
		i2s_ptr->RCSR &= ~I2S_RCSR_FRF_MASK; //  clear the fifo request flag
	}
}


void i2s_set_pin(pin_t pin, uint8_t mux_alt, bool irqEnabled){
	sim_ptr->SCGC6 |= SIM_SCGC6_I2S_MASK;  // module clk gating

	sim_ptr->SCGC5 |= SIM_SCGC5_PORTB_MASK; // clk gating port B and C
	sim_ptr->SCGC5 |= SIM_SCGC5_PORTC_MASK;

	PORT_Type * port = portPointers[PIN2PORT(pin)];

	port-> PCR[PIN2NUM(pin)] &=~(PORT_PCR_PE_MASK |
								PORT_PCR_PS_MASK |
								PORT_PCR_MUX_MASK |
								PORT_PCR_IRQC_MASK); // set all values to edit to zero

	port-> PCR[PIN2NUM(pin)] |= PORT_PCR_PE(0) |
								PORT_PCR_PS(0) |
								PORT_PCR_MUX(mux_alt) |
								PORT_PCR_IRQC(irqEnabled); // edit them
}

void i2s_init(void){
	bool irqEnabled = false;
	//clock gating (ports and module) and mux alternative selection
	i2s_set_pin( PIN_I2S_TX_BLCK, 4, irqEnabled);
	i2s_set_pin( PIN_I2S_TX_FS, 4, irqEnabled);
	i2s_set_pin( PIN_I2S_TX_D0, 6, irqEnabled);
	i2s_set_pin( PIN_I2S_MCLK, 4, irqEnabled);
	i2s_set_pin( PIN_I2S_RX_BCLK, 4, irqEnabled);
	i2s_set_pin( PIN_I2S_RX_FS, 4, irqEnabled);
	i2s_set_pin( PIN_I2S_RX_D0, 4, irqEnabled);

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
	tx_cfg.tx_cfg_0_reg.word_start_interrupt_enable = true;
	tx_cfg.tx_cfg_0_reg.sync_error_interrupt_enable = true;
	tx_cfg.tx_cfg_0_reg.fifo_error_interrupt_enable = true;
	tx_cfg.tx_cfg_0_reg.fifo_warning_interrupt_enable = true;
	tx_cfg.tx_cfg_0_reg.fifo_request_interrupt_enable = true;
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

	NVIC_EnableIRQ(I2S0_Rx_IRQn);
	NVIC_EnableIRQ(I2S0_Tx_IRQn);

}

void i2s_send_16bit_data(uint32_t msg){
	i2s_ptr->TDR[0] = msg;
	i2s_ptr->TDR[1] = msg;
	i2s_ptr->TDR[0] = msg;
	i2s_ptr->TDR[1] = msg;
}
