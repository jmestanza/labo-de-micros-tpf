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
 * @file    CodeMerge_V1.c
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
#include "GFX_Lib_K64F.h"
#include "ecg.h"
#include "fever.h"
#include "i2c_config.h"
#include "o2.h"
#include "algorithm.h"
#include "bt_driver.h"
/*
 * Includes Audio
 */
#include "fsl_sd.h"
#include "fsl_sysmpu.h"
#include "fsl_sgtl5000.h"
#include "SD.h"
#include "I2S.h"
#include "mp3Decoder.h"
#include "audio_and_song.h"
#include "music.h"
/* TODO: insert other definitions and declarations here. */
void ecg_callback(void);
void initialI2STestAndCodecInit(void);
void play_mp3(const char* file_name);
/*
 * Audio data
 */
static short buffer_out[MP3_DECODED_BUFFER_SIZE];
codec_handle_t codecHandle = {0};
extern codec_config_t boardCodecConfig;
uint8_t song_index = 0;
playing_state_t playing_state;

uint16_t i,j,l;
uint16_t bpm[160];
uint16_t oxy[160];
/*
 * Temperature data
 */
float temp;
/*
 * ECG data
 */
uint16_t data[ECG_VALUES];
/*
 * Oximeter data
 */
uint8_t a2 = 0;
int32_t sample_avg = 0;

uint32_t aun_ir_buffer[FS*ST]; //infrared LED sensor data
uint32_t aun_red_buffer[FS*ST];  //red LED sensor data
float n_spo2,prev_spo2,ratio,correl;  //SPO2 value
int8_t ch_spo2_valid;  //indicator to show if the SPO2 calculation is valid
int32_t n_heart_rate,prev_heart_rate; //heart rate value
int8_t  ch_hr_valid;  //indicator to show if the heart rate calculation is valid
/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitSDHC();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    PRINTF("Hello World\n");

    /*
     * Bluetooth Init
     */
    BOARD_InitLEDs();
    LED_BLUE_ON();
    LED_GREEN_OFF();
    bt_init();

    // Test graphics vectors (generated manually)

    for(uint16_t i=0;i<160;i++)
    {
    	oxy[i] = 20*sin(2*PI*i/30)+40;
    }

    for(uint16_t i=0;i<160;i++)
    {
    	bpm[i] = 40;
    }
    for(uint16_t i=0;i<10;i++)
    {
    	bpm[i+40] = 40+(i*40/10);
    	bpm[i+40+80] = 40+(i*40/10);
    }
    for(uint16_t i=0;i<15;i++)
    {
    	bpm[i+50] = 80-(i*60/15);
    	bpm[i+50+80] = 80-(i*60/15);
    }
    for(uint16_t i=0;i<5;i++)
    {
    	bpm[i+65] = 20+(i*60/15);
    	bpm[i+65+80] = 20+(i*60/15);
    }

    prev_spo2 = 00.00;
    prev_heart_rate = 00;

    /*
     * Audio
     */

    SYSMPU_Enable(SYSMPU, false);
	BOARD_Codec_I2C_Init();

	PRINTF("Testing I2S!\n\r");

	initialI2STestAndCodecInit();

	PRINTF("If you heard sounds, then initialization is ok!\n\r");

    /*
     * LCD Init
     */
    lcdGFX_init();

    /*
     * Init ECG
     */
    ecg_init(ecg_callback);

    /*
     * Temp init
     */
    i2cInit();

    /*
     * Oximeter init
     */
    uint8_t uch_dummy;

    maxim_max30102_reset(); //resets the MAX30102
    maxim_max30102_read_reg_blocking(REG_INTR_STATUS_1, &uch_dummy);  //Reads/clears the interrupt status register
    maxim_max30102_init();  //initialize the MAX30102



	if(SD_init() == 0) { // if SD init is ok!
		MP3SetSDInitializedFlag(); // setInitializedFlag on MP3 decoder! => we can read files on SD Card!
	}else{
		return -1;
	}

	/* Aca empezaria el testbench de las reproducciones de audio */

	// PIT_StartTimer(PIT_1_PERIPHERAL, kPIT_Chnl_1);

	// en el handler hago que cada 5 segundos cambie el estado de la cancion que se esta reproduciendo

     playing_state.song_state = SONG_ECG_OUT_OF_RANGE;

//    gpioMode(PIN_SPO2, INPUT);
//    gpioIRQ(PIN_SPO2, GPIO_IRQ_MODE_FALLING_EDGE, callback_pin);
//    NVIC_EnableIRQ(PORTB_IRQn);

    /* Force the counter to be placed into memory. */
//  volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
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
    return 0 ;
}

//void PIT0_IRQHandler(void)
//{
//
//}

void PIT2_IRQHandler(void)
{

	PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);
	i++;
	j++;
	if(!(j%80))
	{// Cada 2 seg
		PIT_StopTimer(PIT_1_PERIPHERAL, kPIT_Chnl_2);
		fever_temp_meassurement();
		temp = fever_get_temperature();
		if(ch_spo2_valid || (ch_hr_valid))
		{
			prev_spo2 = n_spo2;
			prev_heart_rate = n_heart_rate;
		}
		else
		{
			prev_spo2 = 0;
			prev_heart_rate = 0;
		}
		if(playing_state.audio_state == AUDIO_STOP)
		{
			lcdGFX_updateDATA(prev_heart_rate, prev_spo2, temp);
		}
 		PIT_StartTimer(PIT_1_PERIPHERAL, kPIT_Chnl_2);
	}

	if(!(j%8) && bt_getState())
	{
		bt_tim_callback();
	}
}

void PORTB_IRQHandler(void)
{
    /* Clear external interrupt flag. */
    GPIO_PortClearInterruptFlags(BOARD_PIN_SPO2_GPIO, 1U << BOARD_PIN_SPO2_PIN);

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
	bool meassure = maxim_max30102_read_fifo(( aun_ir_buffer  + a2), (aun_red_buffer + a2));

	if (meassure == true)
	{
		a2++;

	}

	if(a2 == FS*ST)
	{
		rf_heart_rate_and_oxygen_saturation(aun_ir_buffer, FS*ST, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid, &ratio, &correl);

		if (ch_spo2_valid ){
			//printf("spo2 %d heart rate %d \n",(int)n_spo2, n_heart_rate);
		}
		else
		{
			//printf("no valido \n");
		}

		a2 = 0;
	}

#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void ecg_callback(void)
{
	ecg_get_samples(data);
	for(int k=0;k<ECG_VALUES-1;k++)
	{
//		uint32_t uni,dec;
//		uni = (aun_red_buffer[l]/100)%10;
//		dec = (aun_red_buffer[l]/1000)%10;

		sample_avg += (aun_red_buffer[l] - sample_avg/12);
		int32_t new_sample;
		new_sample = aun_red_buffer[l] - sample_avg/12; //muestra a graficar
		int32_t dataSPO2ToPlot,dataHRToPlot;
		dataSPO2ToPlot = (new_sample/10)+50;
		dataHRToPlot = (data[k]+data[k+1])/82;

		// Anti-WindUp for graphics
		if(dataSPO2ToPlot > 100) dataSPO2ToPlot = 100;
		if(dataSPO2ToPlot < 0) dataSPO2ToPlot = 0;
		if(dataHRToPlot > 100) dataHRToPlot = 100;
		if(dataHRToPlot < 0) dataHRToPlot = 0;
		//

		lcdGFX_updateGFX(dataHRToPlot, dataSPO2ToPlot);
		l++;
		if(l == 100)
		{
			l = 0;
		}
	}
}

void UART_2_SERIAL_RX_TX_IRQHANDLER()
{
	bt_callback();
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
				PIT_StartTimer(PIT_1_PERIPHERAL, kPIT_Chnl_3); // hago que pit se encienda y empiece a transmitir ese buffer
				can_start_pit = false;
			}
		}
	}

	PIT_StopTimer(PIT_1_PERIPHERAL, kPIT_Chnl_3); // que pare de llamarse este timer que reproduce la cancion
	terminatePlayState();
	MP3CloseFile();
}

void PIT_1_0_IRQHANDLER(void){
	if(lcdGFX_getState())
	{
	    /* Clear interrupt flag.*/
		PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
		pit_callback();
	}
	else
	{
		PIT_ClearStatusFlags(PIT_1_PERIPHERAL, kPIT_Chnl_0, PIT_TFLG_TIF(1));
		PIT_StopTimer(PIT_1_PERIPHERAL, kPIT_Chnl_0);
		// si pongo un breakpoint aca funciona el codigo // si le pongo menos de 10ms al PIT, entra dos veces a este handler
		const sai_transfer_format_t * constFormat = (const sai_transfer_format_t *)getFormatData();
		CODEC_Init(&codecHandle, &boardCodecConfig);
		CODEC_SetFormat(&codecHandle, constFormat->masterClockHz, constFormat->sampleRate_Hz, constFormat->bitWidth);
	}
}


void PIT_1_3_IRQHANDLER(void){
	PIT_ClearStatusFlags(PIT_1_PERIPHERAL, kPIT_Chnl_3, PIT_TFLG_TIF(1));
	if(isFinishedPlaying()){
		resetVariables();
	}else{
		if(playing_state.audio_state == AUDIO_READY_TO_SEND){
			mini_play_music();
		}
	}
}
