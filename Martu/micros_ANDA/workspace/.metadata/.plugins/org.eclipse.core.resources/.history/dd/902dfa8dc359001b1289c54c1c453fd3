/*
 * o2.h
 *
 *  Created on: Jan 14, 2021
 *      Author: martinamaspero
 */

#ifndef O2_H_
#define O2_H_

/*******************************************************************************
* Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*******************************************************************************
*/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>


/*******************************************************************************
 * Definitions
 ******************************************************************************/


#define I2C_WRITE_ADDR 0x57
#define I2C_READ_ADDR 0x57

//register addresses
#define REG_INTR_STATUS_1 0x00
#define REG_INTR_STATUS_2 0x01
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03
#define REG_FIFO_WR_PTR 0x04
#define REG_OVF_COUNTER 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA 0x07
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_LED1_PA 0x0C
#define REG_LED2_PA 0x0D
#define REG_LED4_PA 0x0E
#define REG_PILOT_PA 0x10
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12
#define REG_TEMP_INTR 0x1F
#define REG_TEMP_FRAC 0x20
#define REG_TEMP_CONFIG 0x21
#define REG_PROX_INT_THRESH 0x30
#define REG_REV_ID 0xFE
#define REG_PART_ID 0xFF
#define max30102_INTPin  PBin(7)




/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/**
* \brief        Initialize the MAX30102
* \par          Details
*               This function initializes the MAX30102
* \param        None
* \retval       true on success
*/
bool maxim_max30102_init(void);

/**
* \brief        Read a set of samples from the MAX30102 FIFO register
* \par          Details
*               This function reads a set of samples from the MAX30102 FIFO register
* \param[out]   *pun_red_led   - pointer that stores the red LED reading data
* \param[out]   *pun_ir_led    - pointer that stores the IR LED reading data
* \retval       true on success
*/
bool maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led);


/**
* \brief        Write a value to a MAX30102 register
* \par          Details
*               This function writes a value to a MAX30102 register
* \param[in]    uch_addr    - register address
* \param[in]    uch_data    - register data
* \retval       true on success
*/
bool maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data);

/**
* \brief        Read a MAX30102 register
* \par          Details
*               This function reads a MAX30102 register
* \param[in]    uch_addr    - register address
* \param[out]   puch_data    - pointer that stores the register data
* \retval       true on success
*/
bool maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data);


/**
* brief        Reset the MAX30102
* param        None
* retval       true on success
*/
bool maxim_max30102_reset(void);



bool maxim_max30102_write_reg_blocking(uint8_t uch_addr, uint8_t uch_data);
bool maxim_max30102_read_reg_blocking(uint8_t uch_addr, uint8_t *puch_data);

bool maxim_max30102_read_fifoNB(uint32_t *pun_red_led, uint32_t *pun_ir_led);



#endif /* O2_H_ */
