/*
 * i2s.h
 *
 *  Created on: Feb 1, 2020
 *      Author: joa-m
 */

#ifndef I2S_H_
#define I2S_H_

#include "board.h"
#include "MK64F12.h"
#include <stdbool.h>
#include <stdint.h>
#include "i2s_regedit.h"
#include "hardware.h"

__ISR__ I2S0_Tx_IRQHandler(void);
__ISR__ I2S0_Rx_IRQHandler(void);


void i2s_set_pin(pin_t pin, uint8_t mux_alt, bool irqEnabled);

void i2s_init(void);

bool isFIFOFull(uint32_t tranfer_fifo_register_n);

void i2s_send_data(uint32_t msg);

void i2s_enable_tx(void);

void i2s_disable_tx(void);

void i2s_reset_tx_fifo(void);

#endif /* I2S_H_ */
