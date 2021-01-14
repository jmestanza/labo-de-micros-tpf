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
#define BUFFER_SIZE (1600U)
#define BUFFER_NUM (2U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_I2C_ReleaseBus(void);
static void callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle) = {0};
edma_handle_t dmaHandle = {0};
codec_handle_t codecHandle = {0};
extern codec_config_t boardCodecConfig;
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t buffer[BUFFER_NUM*BUFFER_SIZE], 4);
volatile bool isFinished = false;
volatile uint32_t finishIndex = 0U;
volatile uint32_t emptyBlock = BUFFER_NUM;
/*******************************************************************************
 * Code
 ******************************************************************************/

static void i2c_release_bus_delay(void)
{
    uint32_t i = 0;
    for (i = 0; i < I2C_RELEASE_BUS_COUNT; i++)
    {
        __NOP();
    }
}

void BOARD_I2C_ReleaseBus(void)
{
    uint8_t i = 0;
    gpio_pin_config_t pin_config;
    port_pin_config_t i2c_pin_config = {0};

    /* Config pin mux as gpio */
    i2c_pin_config.pullSelect = kPORT_PullUp;
    i2c_pin_config.mux = kPORT_MuxAsGpio;

    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic = 1U;
    CLOCK_EnableClock(kCLOCK_PortE);
    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SCL_PIN, &i2c_pin_config);
    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SDA_PIN, &i2c_pin_config);

    GPIO_PinInit(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, &pin_config);
    GPIO_PinInit(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA high */
    for (i = 0; i < 9; i++)
    {
        GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
        i2c_release_bus_delay();

        GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
        i2c_release_bus_delay();

        GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
    i2c_release_bus_delay();
}


static void callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
//	if()
//    if(kStatus_SAI_RxError == status) // estaba esto pero
    if(kStatus_SAI_TxError == status)   // me parece que deberia ser con Tx
    {
    	PRINTF("Hubo un error en el SAI");
//    	SAI_ErrorIRQHandler();
    }
    else
    {
    	// se completo una transferencia, a partir de aca hay un t para que salga por la fifo, hay que avisar que hay que hacer delay
        finishIndex++;
        emptyBlock++;
        /* Judge whether the music array is completely transfered. */
        if(MUSIC_LEN/BUFFER_SIZE == finishIndex)
        {
            isFinished = true; // hay que resetear los indices
        }
    }
}

/*!
 * @brief Main function
 */


uint32_t cpy_index = 0U, tx_index=0U;

sai_transfer_t xfer;

sai_config_t config;

uint32_t delayCycle = 5000000U;

edma_config_t dmaConfig = {0};

uint32_t mclkSourceClockHz = 0U;
sai_transfer_format_t format;



void PIT_1_0_IRQHANDLER(void){
	PIT_ClearStatusFlags(PIT_1_PERIPHERAL, kPIT_Chnl_0, PIT_TFLG_TIF(1));
	// si pongo un breakpoint aca funciona el codigo // si le pongo menos de 10ms al PIT, entra dos veces a este handler
    CODEC_Init(&codecHandle, &boardCodecConfig);
    CODEC_SetFormat(&codecHandle, format.masterClockHz, format.sampleRate_Hz, format.bitWidth);
    PIT_StopTimer(PIT_1_PERIPHERAL, kPIT_Chnl_0);
}

void PIT_1_1_IRQHANDLER(void){
	PIT_ClearStatusFlags(PIT_1_PERIPHERAL, kPIT_Chnl_1, PIT_TFLG_TIF(1));

	if(!isFinished)
	{
		if((emptyBlock > 0U) && (cpy_index < MUSIC_LEN/BUFFER_SIZE))
		{
			 /* Fill in the buffers. */
			 memcpy((uint8_t *)&buffer[BUFFER_SIZE*(cpy_index%BUFFER_NUM)],(uint8_t *)&music[cpy_index*BUFFER_SIZE],sizeof(uint8_t)*BUFFER_SIZE);
			 emptyBlock--;
			 cpy_index++;
		}
		if(emptyBlock < BUFFER_NUM)
		{

			/*  xfer structure */
			xfer.data = (uint8_t *)&buffer[BUFFER_SIZE*(tx_index%BUFFER_NUM)];
			xfer.dataSize = BUFFER_SIZE;
			/* Wait for available queue. */
			if(kStatus_Success == SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer))
			{
				tx_index++;
			}
		}
	}else{ // termino!
	    PIT_StopTimer(PIT_1_PERIPHERAL, kPIT_Chnl_1);

	    isFinished = false;
	    finishIndex = 0U;
	    emptyBlock = BUFFER_NUM;
	    cpy_index = 0U;
	    tx_index=0U;
	}
}

void play_music(void){
    isFinished = false;
    finishIndex = 0U;
    emptyBlock = BUFFER_NUM;
    cpy_index = 0U;
    tx_index=0U;
    delayCycle = 5000000U;

    while(!isFinished)
    {
        if((emptyBlock > 0U) && (cpy_index < MUSIC_LEN/BUFFER_SIZE))
        {
             /* Fill in the buffers. */
             memcpy((uint8_t *)&buffer[BUFFER_SIZE*(cpy_index%BUFFER_NUM)],(uint8_t *)&music[cpy_index*BUFFER_SIZE],sizeof(uint8_t)*BUFFER_SIZE);
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
            }else if(kStatus_SAI_QueueFull == status){
				// clear the queue
				__ASM("nop");
			}

        }
    }
}
I2S_Type * sai_base = I2S0;


void I2S0_Tx_IRQHandler(){ // ACA DEBERIA LLEGAR POR ERROR DE LA FIFO SOLAMENTE
	if (sai_base->TCSR & I2S_TCSR_FEF_MASK){
		//	void SAI_ErrorIRQHandler(void){
		/* Clear the FIFO error flag */
		SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);

		/* Reset FIFO */
		SAI_TxSoftwareReset(DEMO_SAI, kSAI_ResetTypeFIFO);

//				SAI_TxSoftwareReset(base, type);
//				SAI_TxInit(DEMO_SAI, &config);
//				SAI_TxReset(sai_base);
//				SAI_TxInit(DEMO_SAI, &config);
//				SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &txHandle, callback, NULL, &dmaHandle);
//				mclkSourceClockHz = DEMO_SAI_CLK_FREQ;
//				SAI_TransferTxSetFormatEDMA(DEMO_SAI, &txHandle, &format, mclkSourceClockHz, format.masterClockHz);


		/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
		  exception return operation might vector to incorrect interrupt */
		#if defined __CORTEX_M && (__CORTEX_M == 4U)
			__DSB();
		#endif

	}
}

void deinit_DMA(void){
//    EDMA_Init(EXAMPLE_DMA, &dmaConfig);
//    EDMA_CreateHandle(&dmaHandle, EXAMPLE_DMA, EXAMPLE_CHANNEL);
//    EDMA_TcdReset(edma_tcd_t *tcd);
    EDMA_ResetChannel(EXAMPLE_DMA, EXAMPLE_CHANNEL); // esto usa TcdReset
    EDMA_EnableAutoStopRequest(EXAMPLE_DMA, EXAMPLE_CHANNEL, 0); // disable
	EDMA_DisableChannelRequest(EXAMPLE_DMA, EXAMPLE_CHANNEL);
    EDMA_Deinit(EXAMPLE_DMA);

//	EDMA_DisableChannelInterrupts(EXAMPLE_DMA, EXAMPLE_CHANNEL, uint32_t mask);
    //    EDMA_GetErrorStatusFlags(DMA_Type *base);
    //	EDMA_ClearChannelStatusFlags(DMA_Type *base, uint32_t channel, uint32_t mask);

//    EDMA_DisableChannelInterrupts(DMA_Type *base, uint32_t channel, uint32_t mask);
//    EDMA_DisableChannelRequest(DMA_Type *base, uint32_t channel);
//    EDMA_GetErrorStatusFlags(DMA_Type *base);
//	EDMA_ClearChannelStatusFlags(DMA_Type *base, uint32_t channel, uint32_t mask);

}

void init_I2S_and_DMA(void){
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(EXAMPLE_DMA, &dmaConfig);
    EDMA_CreateHandle(&dmaHandle, EXAMPLE_DMA, EXAMPLE_CHANNEL);

    DMAMUX_Init(DMAMUX0);
    DMAMUX_SetSource(DMAMUX0, EXAMPLE_CHANNEL, EXAMPLE_SAI_TX_SOURCE);
    DMAMUX_EnableChannel(DMAMUX0, EXAMPLE_CHANNEL);

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

/* If need to handle audio error, enable sai interrupt */

    EnableIRQ(I2S0_Tx_IRQn);
    SAI_TxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);

    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &txHandle, callback, NULL, &dmaHandle);

    mclkSourceClockHz = DEMO_SAI_CLK_FREQ;
    SAI_TransferTxSetFormatEDMA(DEMO_SAI, &txHandle, &format, mclkSourceClockHz, format.masterClockHz);
}


void PIT_1_2_IRQHANDLER(void){
	PIT_ClearStatusFlags(PIT_1_PERIPHERAL, kPIT_Chnl_2, PIT_TFLG_TIF(1));
	SAI_TxSoftwareReset(I2S0, kSAI_ResetAll);
	init_I2S_and_DMA();
	play_music();
	PIT_StopTimer(PIT_1_PERIPHERAL, kPIT_Chnl_2);
}

void get_delay(uint32_t max){
	delayCycle = max;
 	while (delayCycle)
	{
		__ASM("nop");
		delayCycle--;
	}
}

int main(void)
{
	PRINTF("len del arreglo music = %d \n",sizeof(music));
	PRINTF("MUSIC_LEN (el define)= %d \n",MUSIC_LEN);

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitBootPeripherals();
    BOARD_I2C_ReleaseBus(); // probe sin esto y anda pero lo dejo por las dudas
    BOARD_I2C_ConfigurePins();
    BOARD_InitDebugConsole();
    BOARD_Codec_I2C_Init();

    PRINTF("SAI example started!\n\r");

    init_I2S_and_DMA();

    PIT_StartTimer(PIT_1_PERIPHERAL, kPIT_Chnl_0);

//    get_delay(5000000U);

    play_music(); // el primero por la inicializacion no se escucha bien me parece

    play_music();

//	get_delay(500000U);
    deinit_DMA();
//    PIT_StartTimer(PIT_1_PERIPHERAL, kPIT_Chnl_2);
//    get_delay(5000000U);

//    get_delay(10000000U); // este ya no se escucha

//    get_delay(50000000U); // este ya no se escucha
//    play_music();

//    play_music();


    /* Once transfer finish, disable SAI instance. */
//    SAI_TransferAbortSendEDMA(DEMO_SAI, &txHandle);
//    SAI_Deinit(DEMO_SAI);

//    PRINTF("\n\r SAI EDMA example finished!\n\r ");
//    PRINTF("SAI example finished!");

    while (1)
    {
    }
}


#if defined(SAI_ErrorIRQHandler)
void SAI_ErrorIRQHandler(void)
{
    /* Clear the FIFO error flag */
    SAI_TxClearStatusFlags(DEMO_SAI, kSAI_FIFOErrorFlag);

    /* Reset FIFO */
    SAI_TxSoftwareReset(DEMO_SAI, kSAI_ResetTypeFIFO);
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
#endif
