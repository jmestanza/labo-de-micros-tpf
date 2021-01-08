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
 * @file    i2s_sai_test.c
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


/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

uint8_t buffer[] = {0xA0,0xC0,0x00,0x00,0xFF,0xFF,0x00,0x00};

uint8_t dummy_buffer[] = {0xFF,0xFF};


//,0xF0,0xE0}; // words de 16 bits

// primera word = 0xC0A0
// segunda word = 0xE0F0


int count = 0;
uint8_t * buff_pnt = &buffer[0];

void TxCallback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData){

//	// write callback, here I can write userData
//	sai_transfer_t transfer;
//	if(status == kStatus_SAI_TxIdle){
//		// aca deberia haber terminado de mandar todo el buffer!! (//https://mcuxpresso.nxp.com/api_doc/dev/1418/a00104.html)
//
//		transfer.data = buffer;
//		transfer.dataSize = sizeof(buffer);
//
//		SAI_TransferSendNonBlocking(I2S0, handle, &transfer);
//	}
//	count++;

	return;
}

int user_data;


I2S_Type * base = SAI_1_PERIPHERAL;

PIT_Type *base_pit = PIT;

sai_handle_t *global_handle;



bool sent_data = false;


void PIT_1_0_IRQHANDLER(void){
	PIT_ClearStatusFlags(base_pit,kPIT_Chnl_0, PIT_TFLG_TIF(1)); // w1c
	if(global_handle){
		sai_transfer_t transfer;
		transfer.data = buffer;
		transfer.dataSize = sizeof(buffer);
		SAI_TransferSendNonBlocking(I2S0, global_handle, &transfer);
		count++;
	}
}

void configTools_format_to_handle(sai_handle_t * handle){
	handle->state = 1 ; // sai state
	handle->bitWidth = SAI_1_tx_format.bitWidth;
    handle->watermark = SAI_1_tx_format.watermark;
    handle->channel = SAI_1_tx_format.channel;
}


void startTransfer(){

	sai_transfer_t transfer;
	sai_transfer_t someTransfer;
	sai_handle_t handle;


	SAI_TransferTxCreateHandle(I2S0, &handle, TxCallback, NULL); // no user data

	configTools_format_to_handle(&handle);

	global_handle = &handle;

	transfer.data = buffer;
	transfer.dataSize = sizeof(buffer);

	someTransfer.data = dummy_buffer;
	someTransfer.dataSize = sizeof(dummy_buffer);

	SAI_TransferSendNonBlocking(I2S0, &handle, &transfer);

    /* Enqueue next buffer right away so there is no drop in audio data stream when the first buffer finishes */
	SAI_TransferSendNonBlocking(I2S0, &handle, &someTransfer);
}



int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();
    PRINTF("Hello World\n");

    startTransfer();

/* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++ ;
    }
    return 0 ;
}
