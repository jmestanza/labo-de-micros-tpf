/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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
 * This is template for main module created by New Kinetis SDK 2.x Project Wizard. Enjoy!
 **/

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_sai_edma.h"
#include "fsl_port.h"
#include "fsl_edma.h"
#include "fsl_dmamux.h"

void txEdmaCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
	__asm("bkpt #0");
}

uint32_t txBuffer[64]; // 64 * 32-bit = 256 bytes

/*!
 * @brief Application entry point.
 */
int main(void) {
	/* Init board hardware. */
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();

	memset(txBuffer, 0x55, 128);
	memset(&txBuffer[128], 0b10110100, 128);

	CLOCK_EnableClock(kCLOCK_Dmamux0);
	CLOCK_EnableClock(kCLOCK_Sai0);
	CLOCK_EnableClock(kCLOCK_Dma0);
    CLOCK_EnableClock(kCLOCK_PortC);

	PORT_SetPinMux(PORTB, 18UL, kPORT_MuxAlt4);
	PORT_SetPinMux(PORTC, 2UL, kPORT_MuxAlt6);
	PORT_SetPinMux(PORTC, 1UL, kPORT_MuxAlt6);

	DMAMUX_Init(DMAMUX0);
	DMAMUX_SetSource(DMAMUX0, 0L, kDmaRequestMux0I2S0Tx);
	DMAMUX_EnableChannel(DMAMUX0, 0);

	edma_config_t txEdmaConfig = {
			.enableContinuousLinkMode = false,
			.enableHaltOnError = false,
			.enableRoundRobinArbitration = true,
			.enableDebugMode = false
	};
	EDMA_Init(DMA0, &txEdmaConfig);

	edma_handle_t txEdmaHandle = {0};
	EDMA_CreateHandle(&txEdmaHandle, DMA0, 0);

	sai_config_t const txSaiConfig = {
			.protocol = kSAI_BusI2S,
			.syncMode = kSAI_ModeAsync,
			.mclkOutputEnable = true,
			.mclkSource = kSAI_MclkSourceSysclk,
			.bclkSource = kSAI_BclkSourceMclkDiv,
			.masterSlave = kSAI_Master,
	};
	SAI_TxInit(I2S0, &txSaiConfig);

	sai_edma_handle_t txSaiEdmaHandle = {0};
	SAI_TransferTxCreateHandleEDMA(I2S0, &txSaiEdmaHandle, txEdmaCallback, NULL, &txEdmaHandle);

	sai_transfer_format_t txSaiTransferFormat = {
			.sampleRate_Hz = 48000,
			.bitWidth = 32,
			.stereo = kSAI_Stereo,
			.masterClockHz = 120000000,
			.watermark = 4,
			.channel = 0,
			.protocol = kSAI_BusI2S
	};
	SAI_TransferTxSetFormatEDMA(I2S0, &txSaiEdmaHandle, &txSaiTransferFormat, 120000000, 120000000);

	sai_transfer_t txSaiTransfer = {
			.data = (void *)txBuffer,
			.dataSize = 256
	};
	status_t txSendStatus = SAI_TransferSendEDMA(I2S0, &txSaiEdmaHandle, &txSaiTransfer);

	/* Add your code here */

	for(;;) { /* Infinite loop to avoid leaving the main function */
		__asm("NOP"); /* something to use as a breakpoint stop while looping */
	}
}
