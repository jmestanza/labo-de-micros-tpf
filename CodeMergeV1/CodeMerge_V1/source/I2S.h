/*
 * I2S.h
 *
 *  Created on: Feb 3, 2021
 *      Author: joa-m
 */

#ifndef I2S_H_
#define I2S_H_

#include "fsl_edma.h"
//#include "music.h" en algun otro lado esta incluido
#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
#include "fsl_dmamux.h"
#endif
#include "fsl_sai_edma.h"
#include "fsl_sgtl5000.h"
#include "fsl_debug_console.h"

#include "audio_and_song.h"

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


void mini_play_music(void);
void resetVariables(void);
void play_music(void);
void enableRequests(void);
void disableRequests(void);
void connectPlayingStates(playing_state_t * main_playing_state);
void init_DMA(void);
void init_I2S(void);
void initPlayState(void);
void terminatePlayState(void);
void newSongPointerAndLength(uint8_t * dst, uint32_t len);
bool isFinishedPlaying(void);
const sai_transfer_format_t * getFormatData(void);

#endif /* I2S_H_ */
