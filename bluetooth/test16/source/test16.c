/*
 * Copyright 2016-2018 NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    test16.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "bt_driver.h"
/* TODO: insert other definitions and declarations here. */
/*! @brief Ring buffer size (Unit: Byte). */
#define DEMO_RING_BUFFER_SIZE 16

uint8_t demoRingBuffer[DEMO_RING_BUFFER_SIZE];
uint8_t demoRingBuffer2[DEMO_RING_BUFFER_SIZE];
volatile uint16_t txIndex2;
volatile uint16_t rxIndex2;

/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    PRINTF("Hello World\n");

    LED_BLUE_INIT(0);
    LED_GREEN_INIT(0);
    LED_BLUE_ON();
    LED_GREEN_OFF();

    while(1) {
        /* Send data only when UART TX register is empty and ring buffer has data to send out. */
        while ((kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(UART0)) && (rxIndex2 != txIndex2))
        {
            UART_WriteByte(UART0, demoRingBuffer2[txIndex2]);
            txIndex2++;
            txIndex2 %= DEMO_RING_BUFFER_SIZE;
        }
    }
    return 0 ;
}

void UART_1_SERIAL_RX_TX_IRQHANDLER() {
    uint8_t data;

    /* If new data arrived. */
    if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(UART0))
    {
        data = UART_ReadByte(UART0);

        /* If ring buffer is not full, add data to ring buffer. */
        if (((rxIndex2 + 1) % DEMO_RING_BUFFER_SIZE) != txIndex2)
        {
            demoRingBuffer[rxIndex2] = data;
            rxIndex2++;
            rxIndex2 %= DEMO_RING_BUFFER_SIZE;
        }
    }

}

void UART_2_SERIAL_RX_TX_IRQHANDLER()
{
	bt_callback();
}

void FTM_1_IRQHANDLER()
{
	FTM_ClearStatusFlags(FTM_1_PERIPHERAL, kFTM_TimeOverflowFlag);
	bt_tim_callback();
}
