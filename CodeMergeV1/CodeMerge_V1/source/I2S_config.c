/*
 * I2S.c
 *
 *  Created on: Feb 3, 2021
 *      Author: joa-m
 */
#include <I2S_config.h>

AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle) = {0};


edma_handle_t dmaHandle = {0};

AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t buffer[BUFFER_NUM*BUFFER_SIZE], 4);
volatile bool isFinished = false;
volatile uint32_t finishIndex = 0U;
volatile uint32_t emptyBlock = BUFFER_NUM;

uint32_t cpy_index = 0U;
uint32_t tx_index=0U;
sai_transfer_t xfer;
sai_config_t config;
uint32_t delayCycle = 5000000U;
edma_config_t dmaConfig = {0};
uint32_t mclkSourceClockHz = 0U;
sai_transfer_format_t format;
bool first_time = true;
bool startedPlaying = false;
bool total_decode = false;
uint8_t * pointer_to_dst = NULL;
uint32_t music_len = 0;

playing_state_t *i2s_playing_state;


static void callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);



static void callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if(kStatus_SAI_TxError == status)   // me parece que deberia ser con Tx
    {
    	PRINTF("Hubo un error en el SAI");
    }
    else
    {
    	// se completo una transferencia, a partir de aca hay un t para que salga por la fifo, hay que avisar que hay que hacer delay
        finishIndex++;
        emptyBlock++;
        /* Judge whether the music array is completely transfered. */
        if(music_len/BUFFER_SIZE == finishIndex)
        {
            isFinished = true; // hay que resetear los indices
            i2s_playing_state->audio_state = AUDIO_PLAYING;
        }
    }
}


const sai_transfer_format_t * getFormatData(void){
	 return (const sai_transfer_format_t *) &format;
}

bool isFinishedPlaying(void){
	return isFinished;
}


void mini_play_music(void){
	if(!isFinished){
		if((emptyBlock > 0U) && (cpy_index < music_len/BUFFER_SIZE))
		{
			 /* Fill in the buffers. */
			 memcpy((uint8_t *)&buffer[BUFFER_SIZE*(cpy_index%BUFFER_NUM)],(uint8_t *)&pointer_to_dst[cpy_index*BUFFER_SIZE],sizeof(uint8_t)*BUFFER_SIZE);
			 emptyBlock--;
			 cpy_index++;
		}
		if(emptyBlock < BUFFER_NUM)
		{
			/*  xfer structure */
			xfer.data = (uint8_t *)&buffer[BUFFER_SIZE*(tx_index%BUFFER_NUM)];
			xfer.dataSize = BUFFER_SIZE;
			/* Wait for available queue. */
			status_t status = SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer);

			if(kStatus_Success == status)
			{
				tx_index++;
			}
		}
	}

}

void resetVariables(void){
	isFinished = false;
	finishIndex = 0U;
	emptyBlock = BUFFER_NUM;
	cpy_index = 0U;
	tx_index=0U;
}


void play_music(void){
	resetVariables();
    while(!isFinished){
    	mini_play_music();
    }
}


void enableRequests(void){
    DMA_Type * base_dma = EXAMPLE_DMA;
    base_dma->ERQ |= DMA_ERQ_ERQ0_MASK;
}

void disableRequests(void){
    DMA_Type * base_dma = EXAMPLE_DMA;
    base_dma->ERQ &= ~DMA_ERQ_ERQ0_MASK;
}


void connectPlayingStates(playing_state_t * main_playing_state){
    i2s_playing_state = main_playing_state;
}

void init_DMA(void){
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(EXAMPLE_DMA, &dmaConfig);

    EDMA_CreateHandle(&dmaHandle, EXAMPLE_DMA, EXAMPLE_CHANNEL);

    DMAMUX_Init(DMAMUX0);
    DMAMUX_SetSource(DMAMUX0, EXAMPLE_CHANNEL, EXAMPLE_SAI_TX_SOURCE);
    DMAMUX_EnableChannel(DMAMUX0, EXAMPLE_CHANNEL);
}

void init_I2S(void){
    SAI_TxGetDefaultConfig(&config);
    config.mclkOutputEnable = true;
    config.protocol = kSAI_BusI2S;
    SAI_TxInit(DEMO_SAI, &config);

    /* Configure the audio format */
    format.bitWidth = kSAI_WordWidth16bits;
    format.channel = 0U;
    format.sampleRate_Hz = kSAI_SampleRate16KHz;
    format.masterClockHz = OVER_SAMPLE_RATE * format.sampleRate_Hz;
    format.protocol = config.protocol;
    format.stereo = kSAI_Stereo;
    format.isFrameSyncCompact = false;
    format.watermark = FSL_FEATURE_SAI_FIFO_COUNT / 2U;

///* If need to handle audio error, enable sai interrupt */
//    EnableIRQ(I2S0_Tx_IRQn);
//    SAI_TxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);

    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &txHandle, callback, NULL, &dmaHandle);

    mclkSourceClockHz = DEMO_SAI_CLK_FREQ;
    SAI_TransferTxSetFormatEDMA(DEMO_SAI, &txHandle, &format, mclkSourceClockHz, format.masterClockHz);
}


void newSongPointerAndLength(uint8_t * dst, uint32_t len){
	pointer_to_dst = dst;
	music_len = len;
}

void initPlayState(void){
	init_DMA();
	init_I2S();
}

void terminatePlayState(void){
	I2S_Type * sai_base = I2S0;
	disableRequests();
	while(!(sai_base->TCSR & I2S_TCSR_FWF_MASK)){ // wait for FIFO Empty Warning
	} // a veces se traba con esta linea, ver si hay que descomentarla

	SAI_TransferTerminateSendEDMA(DEMO_SAI, &txHandle);
	SAI_Deinit(DEMO_SAI);
	DMAMUX_DisableChannel(DMAMUX0, EXAMPLE_CHANNEL);
	DMAMUX_Deinit(DMAMUX0);
	EDMA_Deinit(EXAMPLE_DMA);
	resetVariables();
}
