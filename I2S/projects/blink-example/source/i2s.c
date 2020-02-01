/*
 * i2s.c
 *
 *  Created on: Feb 1, 2020
 *      Author: joa-m
 */

#include "i2s.h"


I2S_Type * i2s_ptr = I2S0;



i2s_init(){
	tx_config_regs tx_cfg;
	tx_cfg.tx_cfg_0_reg.bit_clock_enable = true; //example

}
