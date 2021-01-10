
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"

//SAI_XFER_QUEUE_SIZE


//uint8_t buffer[] = {0xA0,0xC0};
//uint8_t dummy_buffer[] = {0xFF,0xFF};


//uint8_t signal[] = {0xA0,0xC0,0x00,0x00,0xFF,0xFF,0x00,0x00};

uint8_t buffer[] = {0xA0,0xC0,0x00,0x00,0xFF,0xFF,0x00,0x00};
uint8_t dummy_buffer[] = {0xA0,0xC0,0x00,0x00,0xFF,0xFF,0x00,0x00};


// words de 16 bits

// primera word = 0xC0A0
// segunda word = 0xE0F0



int count = 0;
uint8_t * buff_pnt = &buffer[0];



#define N_BUFFERS sizeof(signal)/sizeof(buffer) // tienen mismo tipo asi que esta bien

#define LEAP sizeof(buffer)/sizeof(buffer[0])

void TxCallback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData){

	// write callback, here I can write userData
	sai_transfer_t transfer;

	if(status == kStatus_SAI_TxIdle && handle->saiQueue[handle->queueDriver].dataSize == 0U){
		// aca deberia haber terminado de mandar todo el buffer!! (//https://mcuxpresso.nxp.com/api_doc/dev/1418/a00104.html)

//		count = (count + LEAP) % (LEAP*N_BUFFERS);
//		buffer[0] = signal[count];
//	    buffer[1] = signal[count+1];

		transfer.data = buffer;
		transfer.dataSize = sizeof(buffer)/sizeof(buffer[0]);

		SAI_TransferSendNonBlocking(I2S0, handle, &transfer);
	}
	return;
}

int user_data;


I2S_Type * base = SAI_1_PERIPHERAL;

sai_handle_t *global_handle = NULL;



bool sent_data = false;


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
	transfer.dataSize = sizeof(buffer)/sizeof(buffer[0]);

	someTransfer.data = dummy_buffer;
	someTransfer.dataSize = sizeof(dummy_buffer)/sizeof(dummy_buffer[0]);

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

//    startTransfer();

/* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++ ;
    }
    return 0 ;
}
