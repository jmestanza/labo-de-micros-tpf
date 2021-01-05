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




//SAI_XFER_QUEUE_SIZE


#define BUFFER_SIZE 4

// EL TAMAÑO DE ESTE BUFFER SIZE TIENE QUE SER IGUAL AL DE SAI_XFER_QUEUE_SIZE
// para que pueda por ejemplo, mandar en un saque todo el buffer.
// si es distinto tamaño, cambia la cantidad de datos que manda.
// por ejemplo si tengo que mandar 6 bytes y el SAI_XFER_QUEUE_SIZE es 4, entonces manda
// 10 bytes, por algun motivo


uint8_t buffer[BUFFER_SIZE] = {0xA0,0xC0,0xF0,0xE0}; // words de 16 bits
// primera word = 0xC0A0
// segunda word = 0xE0F0


static I2S_Type * i2s_ptr = I2S0;


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

bool isFIFOFull(uint32_t tranfer_fifo_register_n){

	uint32_t WFP = tranfer_fifo_register_n & I2S_TFR_WFP_MASK;
	uint32_t RFP = tranfer_fifo_register_n & I2S_TFR_RFP_MASK;
	uint32_t WFP_MSB = (WFP & (1<<19)) == (1<<19);
	uint32_t RFP_MSB = (RFP & (1<<3)) == (1<<3);
	uint32_t WFP_others = ( WFP & ~(1<<19) ) >> 16; // creo que es 16 y no 15, hay que testear
	uint32_t RFP_others = ( RFP & ~(1<<3) );

	if( (WFP_others == RFP_others) && (WFP_MSB != RFP_MSB) ){
		return true; // fifo full
	}else{
		return false; // fifo not full
	}
}

size_t count;
void callback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData){
	// write callback, here I can write userData

	// (!) handle->state es distinto a los states de status

	// este callback se llama en diversas situaciones
	if(handle->state == 0){ // sai busy

//		i2s_ptr->TCSR |= I2S_TCSR_FR_MASK; // FIFO RESET!
	}else if(handle->state == 1){ // sai idle

	}else if(handle->state == 2){ // sai error

	}

	if(status == kStatus_SAI_TxError){

	}

	if(status == kStatus_SAI_TxIdle){

		// aca deberia haber terminado de mandar todo
		int a = 5;
	}
	return;
}

int user_data;


sai_handle_t handle = {
	.state = 1, // sai idle
	.callback = callback,
	.userData = (void *) &user_data,
	.bitWidth = 16,
	.channel = 0,
	.watermark = 0
};

sai_transfer_t xfer = {
	.data = buffer,
	.dataSize = BUFFER_SIZE
};

I2S_Type * base = SAI_1_PERIPHERAL;

bool first_interrupt = true;

void SAI_1_SERIAL_TX_IRQHANDLER(void){

	SAI_TxDisableInterrupts(base, kSAI_FIFOErrorInterruptEnable | kSAI_FIFORequestInterruptEnable);

	if(isWordStartFlag(i2s_ptr->TCSR)){
		i2s_ptr->TCSR &= ~I2S_TCSR_WSF_MASK; //  clear the flag, w1c
	}
	if(isSyncErrorFlag(i2s_ptr->TCSR)){
		i2s_ptr->TCSR &= ~I2S_TCSR_SEF_MASK; //  clear the sync error flag w1c
	}
	if(isFIFOErrorFlag(i2s_ptr->TCSR)){
		i2s_ptr->TCSR |= I2S_TCSR_FEF_MASK; //  clear the fifo error flag w1c
		i2s_ptr->TCSR |= I2S_TCSR_FR_MASK; // FIFO RESET!
	}
	if(isFIFOWarningFlag(i2s_ptr->TCSR)){
		// Indicates that an enabled transmit FIFO is empty.
		i2s_ptr->TCSR &= ~I2S_TCSR_FWF_MASK; //  clear the fifo warning flag
		SAI_TransferTxHandleIRQ(base,&handle);
	}

	if(isFIFORequestFlag(i2s_ptr->TCSR)){
		int a = 5;
//		SAI_TransferTerminateSend(base, &handle);
	}

// (!) FIFORequestFlag se clearea automaticamente, no hay que escribirlo

	SAI_TxEnableInterrupts(base, kSAI_FIFOErrorInterruptEnable | kSAI_FIFORequestInterruptEnable);

// aca estan las interrupciones de SAI
	return;
}



int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    PRINTF("Hello World\n");

/* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
    	if(i==0){
        	SAI_TransferSendNonBlocking(base, &handle, &xfer);
    	}
        i++ ;
    }
    return 0 ;
}
