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



#include "board.h"
#include "peripherals.h"



#include "fsl_debug_console.h"

#include "fsl_pit.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_gpio.h"
#include "fsl_port.h"



#include <stdio.h>
#include <string.h>
#include "fsl_sd.h"

#include "fsl_debug_console.h"
#include "board.h"
//
#include "fsl_sysmpu.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "fsl_sgtl5000.h"

#include "SD.h"

#include "I2S.h"
#include "mp3Decoder.h"

#include "audio_and_song.h"
#include "music.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_I2C_ReleaseBus(void);


/*******************************************************************************
 * Variables
 ******************************************************************************/
codec_handle_t codecHandle = {0};
extern codec_config_t boardCodecConfig;
uint8_t song_index = 0;
playing_state_t playing_state;
/*******************************************************************************
 * Code
 ******************************************************************************/



void PIT_1_0_IRQHANDLER(void){
	PIT_ClearStatusFlags(PIT_1_PERIPHERAL, kPIT_Chnl_0, PIT_TFLG_TIF(1));
	PIT_StopTimer(PIT_1_PERIPHERAL, kPIT_Chnl_0);
	// si pongo un breakpoint aca funciona el codigo // si le pongo menos de 10ms al PIT, entra dos veces a este handler
	const sai_transfer_format_t * constFormat = (const sai_transfer_format_t *)getFormatData();
    CODEC_Init(&codecHandle, &boardCodecConfig);
    CODEC_SetFormat(&codecHandle, constFormat->masterClockHz, constFormat->sampleRate_Hz, constFormat->bitWidth);

}


void PIT_1_2_IRQHANDLER(void){
	PIT_ClearStatusFlags(PIT_1_PERIPHERAL, kPIT_Chnl_2, PIT_TFLG_TIF(1));
	if(isFinishedPlaying()){
		resetVariables();
	}else{
		if(playing_state.audio_state == AUDIO_READY_TO_SEND){
			mini_play_music();
		}
	}
}

void get_delay(uint32_t max){
	uint32_t delayCycle = max;
 	while (delayCycle)
	{
		__ASM("nop");
		delayCycle--;
	}
}

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

	playing_state.audio_state = AUDIO_PLAYING;
	while (playing_state.audio_state != AUDIO_STOP && playing_state.audio_state != AUDIO_ERROR)
	{
		if(playing_state.audio_state == AUDIO_PLAYING){
			mp3_decoder_result_t res = MP3GetDecodedFrame(buffer_out, MP3_DECODED_BUFFER_SIZE, &sampleCount, 0);
			if (res == MP3DECODER_NO_ERROR)
			{
				MP3GetLastFrameData(&frameData);

				 newSongPointerAndLength((uint8_t *)&buffer_out, sampleCount*2);
//				music_len = sampleCount*2;// las samples son de 2 bytes, asi que music_len (que se mide en bytes) es sampleCountx2

//				pointer_to_dst = (uint8_t *)&buffer_out; // esto deberia tener formato de L-R-L-R

				 playing_state.audio_state = AUDIO_READY_TO_SEND;
				// tengo las samples decodificadas, declaro que estoy listo para enviarlas
				// mientras este listo para enviarlas, mando sampels (ver la interrupcion de PIT2)
				// cuando termino de enviar todo, vuelvo a audio_state = AUDIO_PLAYING para que se decodifique la siguiente
			}
			else if (res == MP3DECODER_FILE_END)
			{
//				PRINTF("[APP] FILE ENDED.\n");
				playing_state.audio_state = AUDIO_STOP;
			}else{
				PRINTF("[APP] An error has ocurred probably\n");
				playing_state.audio_state = AUDIO_ERROR;
			}
		}else if(playing_state.audio_state == AUDIO_READY_TO_SEND){
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


void PIT_1_1_IRQHANDLER(void){
	PIT_ClearStatusFlags(PIT_1_PERIPHERAL, kPIT_Chnl_1, PIT_TFLG_TIF(1));
//	PIT_StopTimer(PIT_1_PERIPHERAL, kPIT_Chnl_1); // que pare de llamarse este timer que reproduce la cancion

	song_index = (song_index + 1)%3;
	// son 3 canciones, a partir del indice 1
	playing_state.song_state = song_index+1;

//	song_state = SONG_ECG_OUT_OF_RANGE;
//	play_mp3("/./ecg_oor.mp3");
	// NO PONER play_mp3 ACA!! ,
	// En este contexto se congela systick y es necesario para leer la SD!
	// Se traba el programa por completo si lo hago :(
}

void initialI2STestAndCodecInit(void){

	connectPlayingStates(&playing_state);

    PIT_StartTimer(PIT_1_PERIPHERAL, kPIT_Chnl_0);

    newSongPointerAndLength((uint8_t *)&music, MUSIC_LEN);

    initPlayState();

    play_music(); // dummy play music para q se pueda inicializar el codec uda 1380

    terminatePlayState();


    newSongPointerAndLength((uint8_t *)&music, MUSIC_LEN);

	initPlayState();

	play_music();			// dummy play to set codec UDA1380

	terminatePlayState();
}



int main(void)
{

	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitBootPeripherals();
	BOARD_I2C_ConfigurePins();
	BOARD_InitDebugConsole();
	SYSMPU_Enable(SYSMPU, false);
	BOARD_Codec_I2C_Init();

    PRINTF("Testing I2S!\n\r");

    initialI2STestAndCodecInit();

    PRINTF("If you heard sounds, then initialization is ok!\n\r");


	if(SD_init() == 0) { // if SD init is ok!
		MP3SetSDInitializedFlag(); // setInitializedFlag on MP3 decoder! => we can read files on SD Card!
	}else{
		return -1;
	}

	/* Aca empezaria el testbench de las reproducciones de audio */

	PIT_StartTimer(PIT_1_PERIPHERAL, kPIT_Chnl_1);
	// en el handler hago que cada 5 segundos cambie el estado de la cancion que se esta reproduciendo

	playing_state.song_state = SONG_NO_SOUND;

    while (1)
    {
    	// espero que PIT cambie el estado de song_state

    	if(playing_state.song_state != SONG_NO_SOUND){
    		switch(playing_state.song_state){
    		case SONG_ECG_OUT_OF_RANGE:
    			play_mp3("/./sonicpro.mp3");
    			// (!) Este dummy play es necesario (si entre las reproducciones hay intervalo mayor a 10 segundos)
    			// Muy probablemente la placa UDA1380 detecta silencio y se va a un modo bajo consumo.
    			// Es por eso que hay que hacer un "wakeup" con un sonido que se haya reproducido al menos 1 segundo antes
    			// de reproducir el mp3 verdadero. Si no es la placa quien detecta silencio, puede que sea i2s.
    			play_mp3("/./ecg_oor.mp3"); //
    			break;
    		case SONG_SPO2_OUT_OF_RANGE:
    			play_mp3("/./sonicpro.mp3");
    			play_mp3("/./spo2_oor.mp3");
    			break;
    		case SONG_TEMP_OUT_OF_RANGE:
    			play_mp3("/./sonicpro.mp3");
    			play_mp3("/./temp_oor.mp3");
    			break;
    		default:
    			PRINTF("Shouldn't be here");
    		}
    		playing_state.song_state = SONG_NO_SOUND;
    	}

	}
    return 0;
}



