/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

#include "fsl_edma.h"

#include "board.h"
#include "peripherals.h"
#include "music.h"
#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
#include "fsl_dmamux.h"
#endif
#include "fsl_sai_edma.h"
#include "fsl_debug_console.h"

#include "fsl_pit.h"

#include "fsl_sgtl5000.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_gpio.h"
#include "fsl_port.h"



#include <stdio.h>
#include <string.h>
#include "fsl_sd.h"

#include "fsl_debug_console.h"
#include "ff.h"

#include "diskio.h"
#include "fsl_sd_disk.h"
#include "board.h"
//
#include "fsl_sysmpu.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "mp3Decoder.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SAI, I2C instance and clock */
#define DEMO_SAI I2S0
#define DEMO_I2C I2C0
#define DEMO_SAI_CLKSRC kCLOCK_CoreSysClk
#define DEMO_SAI_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define DEMO_I2C_CLKSRC kCLOCK_BusClk
#define DEMO_I2C_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)
#define EXAMPLE_DMA (DMA0)
#define EXAMPLE_CHANNEL (0U)
#define EXAMPLE_SAI_TX_SOURCE kDmaRequestMux0I2S0Tx

//#define DEMO_SAI_IRQ I2S0_Tx_IRQn

#define I2C_RELEASE_SDA_PORT PORTE
#define I2C_RELEASE_SCL_PORT PORTE
#define I2C_RELEASE_SDA_GPIO GPIOE
#define I2C_RELEASE_SDA_PIN 25U
#define I2C_RELEASE_SCL_GPIO GPIOE
#define I2C_RELEASE_SCL_PIN 24U
#define I2C_RELEASE_BUS_COUNT 100U
#define OVER_SAMPLE_RATE (256U)
//#define BUFFER_SIZE (1600U) // recordar: #define MP3_DECODED_BUFFER_SIZE (4*1152) y que este buffer es la mitad
//#define BUFFER_SIZE (2*1152)
#define BUFFER_SIZE (1152)
//#define BUFFER_SIZE (1152)

#define BUFFER_NUM (2U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_I2C_ReleaseBus(void);
static void callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/

typedef enum
{
	AUDIO_PLAYING,
	AUDIO_READY_TO_SEND,
    AUDIO_STOP,
	AUDIO_ERROR
} audio_state_t;

typedef enum
{
	SONG_NO_SOUND,
	SONG_ECG_OUT_OF_RANGE,
	SONG_SPO2_OUT_OF_RANGE,
	SONG_TEMP_OUT_OF_RANGE
} song_state_t;


AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle) = {0};

edma_handle_t dmaHandle = {0};
codec_handle_t codecHandle = {0};
extern codec_config_t boardCodecConfig;
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

audio_state_t audio_state;
song_state_t song_state;
/*******************************************************************************
 * Code
 ******************************************************************************/

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
			audio_state = AUDIO_PLAYING;
        }
    }
}


void PIT_1_0_IRQHANDLER(void){
	PIT_ClearStatusFlags(PIT_1_PERIPHERAL, kPIT_Chnl_0, PIT_TFLG_TIF(1));
	PIT_StopTimer(PIT_1_PERIPHERAL, kPIT_Chnl_0);
	// si pongo un breakpoint aca funciona el codigo // si le pongo menos de 10ms al PIT, entra dos veces a este handler
    CODEC_Init(&codecHandle, &boardCodecConfig);
    CODEC_SetFormat(&codecHandle, format.masterClockHz, format.sampleRate_Hz, format.bitWidth);

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



void PIT_1_2_IRQHANDLER(void){
	PIT_ClearStatusFlags(PIT_1_PERIPHERAL, kPIT_Chnl_2, PIT_TFLG_TIF(1));
	if(isFinished){
		resetVariables();
	}else{
		if(audio_state == AUDIO_READY_TO_SEND){
			mini_play_music();
		}
	}
}

void get_delay(uint32_t max){
	delayCycle = max;
 	while (delayCycle)
	{
		__ASM("nop");
		delayCycle--;
	}
}


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
* @brief wait card insert function.
*/
//static status_t sdcardWaitCardInsert(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
//static FATFS g_fileSystem; /* File system object */
//static FIL g_fileObject;   /* File object */

//static const sdmmchost_detect_card_t s_sdCardDetect = {
//#ifndef BOARD_SD_DETECT_TYPE
//    .cdType = kSDMMCHOST_DetectCardByGpioCD,
//#else
//    .cdType = BOARD_SD_DETECT_TYPE,
//#endif
//    .cdTimeOut_ms = (~0U),
//};

/*******************************************************************************
 * Code
 ******************************************************************************/


static short buffer_out[MP3_DECODED_BUFFER_SIZE];


void play_mp3(const char* file_name){
	MP3DecoderInit();
	uint16_t sampleCount;
	mp3_decoder_frame_data_t frameData;

	MP3LoadFile(file_name);

	initPlayState();
	//en el init ya se hace reset de SAI antes de configurar!
	resetVariables();
	bool can_start_pit = true;

	audio_state = AUDIO_PLAYING;
	while (audio_state != AUDIO_STOP && audio_state != AUDIO_ERROR)
	{
		if(audio_state == AUDIO_PLAYING){
			mp3_decoder_result_t res = MP3GetDecodedFrame(buffer_out, MP3_DECODED_BUFFER_SIZE, &sampleCount, 0);
			if (res == MP3DECODER_NO_ERROR)
			{
				MP3GetLastFrameData(&frameData);

				music_len = sampleCount*2;// las samples son de 2 bytes, asi que music_len (que se mide en bytes) es sampleCountx2

				pointer_to_dst = (uint8_t *)&buffer_out; // esto deberia tener formato de L-R-L-R

				audio_state = AUDIO_READY_TO_SEND;
				// tengo las samples decodificadas, declaro que estoy listo para enviarlas
				// mientras este listo para enviarlas, mando sampels (ver la interrupcion de PIT2)
				// cuando termino de enviar todo, vuelvo a audio_state = AUDIO_PLAYING para que se decodifique la siguiente
			}
			else if (res == MP3DECODER_FILE_END)
			{
//				PRINTF("[APP] FILE ENDED.\n");
				audio_state = AUDIO_STOP;
			}else{
				PRINTF("[APP] An error has ocurred probably\n");
				audio_state = AUDIO_ERROR;
			}
		}else if(audio_state == AUDIO_READY_TO_SEND){
			if(can_start_pit){
				PIT_StartTimer(PIT_1_PERIPHERAL, kPIT_Chnl_2); // hago que pit se encienda y empiece a transmitir ese buffer
				can_start_pit = false;
			}
		}
	}

	PIT_StopTimer(PIT_1_PERIPHERAL, kPIT_Chnl_2); // que pare de llamarse este timer que reproduce la cancion
	terminatePlayState();
	MP3CloseFile();
}


uint8_t song_index = 0;

//static FIL g_fileObject;   /* File object */

void PIT_1_1_IRQHANDLER(void){
	PIT_ClearStatusFlags(PIT_1_PERIPHERAL, kPIT_Chnl_1, PIT_TFLG_TIF(1));
//	PIT_StopTimer(PIT_1_PERIPHERAL, kPIT_Chnl_1); // que pare de llamarse este timer que reproduce la cancion

	song_index = (song_index + 1)%3;
	// son 3 canciones, a partir del indice 1
	song_state = song_index+1;

//	song_state = SONG_ECG_OUT_OF_RANGE;
//	play_mp3("/./ecg_oor.mp3");
	// NO PONER play_mp3 ACA!! ,
	// En este contexto se congela systick y es necesario para leer la SD!
	// Se traba el programa por completo si lo hago :(
}

const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
static const sdmmchost_detect_card_t s_sdCardDetect = {
#ifndef BOARD_SD_DETECT_TYPE
    .cdType = kSDMMCHOST_DetectCardByGpioCD,
#else
    .cdType = BOARD_SD_DETECT_TYPE,
#endif
    .cdTimeOut_ms = (~0U),
};

FATFS g_fileSystem;

static status_t sdcardWaitCardInsert(void);

FRESULT SD_init(void){
	FRESULT error;
	PRINTF("\r\nFATFS example to demonstrate how to use FATFS with SD card.\r\n");

	PRINTF("\r\nPlease insert a card into board.\r\n");

	if (sdcardWaitCardInsert() != kStatus_Success)
	{
		return -1;
	}

	if (f_mount(&g_fileSystem, driverNumberBuffer, 0U))
	{
		PRINTF("Mount volume failed.\r\n");
		return -1;
	}

#if (FF_FS_RPATH >= 2U)
	error = f_chdrive((char const *)&driverNumberBuffer[0U]);
	if (error)
	{
		PRINTF("Change drive failed.\r\n");
		return -1;
	}
#endif
	return error;
}

static status_t sdcardWaitCardInsert(void)
{
    /* Save host information. */
    g_sd.host.base = SD_HOST_BASEADDR;
    g_sd.host.sourceClock_Hz = SD_HOST_CLK_FREQ;
    /* card detect type */
    g_sd.usrParam.cd = &s_sdCardDetect;
#if defined DEMO_SDCARD_POWER_CTRL_FUNCTION_EXIST
    g_sd.usrParam.pwr = &s_sdCardPwrCtrl;
#endif
    /* SD host init function */
    if (SD_HostInit(&g_sd) != kStatus_Success)
    {
        PRINTF("\r\nSD host init fail\r\n");
        return kStatus_Fail;
    }
    /* power off card */
    SD_PowerOffCard(g_sd.host.base, g_sd.usrParam.pwr);
    /* wait card insert */
    if (SD_WaitCardDetectStatus(SD_HOST_BASEADDR, &s_sdCardDetect, true) == kStatus_Success)
    {
        PRINTF("\r\nCard inserted.\r\n");
        /* power on the card */
        SD_PowerOnCard(g_sd.host.base, g_sd.usrParam.pwr);
    }
    else
    {
        PRINTF("\r\nCard detect fail.\r\n");
        return kStatus_Fail;
    }

    return kStatus_Success;
}




int main(void)
{

//	FRESULT error;


	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitBootPeripherals();
	BOARD_I2C_ConfigurePins();
	BOARD_InitDebugConsole();
	SYSMPU_Enable(SYSMPU, false);
	BOARD_Codec_I2C_Init();


	PRINTF("len del arreglo music (READ ONLY) = %d \n",sizeof(music));
	PRINTF("MUSIC_LEN (el define)= %d \n",MUSIC_LEN);

    PRINTF("SAI example started!\n\r");

    PIT_StartTimer(PIT_1_PERIPHERAL, kPIT_Chnl_0);

    music_len = MUSIC_LEN;
	pointer_to_dst = (uint8_t *)&music;

    initPlayState();

    play_music(); // dummy play music para q se pueda inicializar el codec uda 1380

    terminatePlayState();


	music_len = MUSIC_LEN;
	pointer_to_dst = (uint8_t *)&music;

	initPlayState();

	play_music();			// dummy play to set codec UDA1380

	terminatePlayState();

	if(SD_init() == FR_OK) {
		MP3SetSDInitializedFlag();
	}else{
		return -1;
	}


	PIT_StartTimer(PIT_1_PERIPHERAL, kPIT_Chnl_1); // en el handler hago que cada 5 segundos se reproduzca un mp3

	song_state = SONG_NO_SOUND;

    while (1)
    {
    	// espero que PIT cambie el estado de song_state

    	if(song_state != SONG_NO_SOUND){
    		switch(song_state){
    		case SONG_ECG_OUT_OF_RANGE:
//    			play_mp3("/./no_sound.mp3"); // dummy play de 1s en total para saber si es porque detecta silencio
//    			play_mp3("/./no_sound.mp3"); // si detecta silencio probablmeente sea la placa UDA1380 la que se va a sleep  y no I2S, sino, es al reves.
    			play_mp3("/./sonicpro.mp3"); // efectivamente este dummy play es necesario (si entre las reproducciones hay intervalo mayor a 10 segundos)
    			play_mp3("/./ecg_oor.mp3"); //
//    			play_mp3("/./ahh_ecg_oor.mp3");
    			break;
    		case SONG_SPO2_OUT_OF_RANGE:
//    			play_mp3("/./no_sound.mp3");
//    			play_mp3("/./no_sound.mp3");
    			play_mp3("/./sonicpro.mp3");
    			play_mp3("/./spo2_oor.mp3");
//    			play_mp3("/./ahh_spo2_oor.mp3");
//    			play_mp3("/./ahh_ecg_oor.mp3");
    			break;
    		case SONG_TEMP_OUT_OF_RANGE:
//    			play_mp3("/./no_sound.mp3");
//    			play_mp3("/./no_sound.mp3");
    			play_mp3("/./sonicpro.mp3");
    			play_mp3("/./temp_oor.mp3");
//    			play_mp3("/./ahh_temp_oor.mp3");
//    			play_mp3("/./ahh_ecg_oor.mp3");
    			break;
    		default:
//    			play_mp3("/./no_sound.mp3");
    			PRINTF("Shouldn't be here");
    		}
			song_state = SONG_NO_SOUND;
    	}

	}
    return 0;
}



