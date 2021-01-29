/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "board.h"
#include "gpio.h"
#include "i2s.h"
#include "DMA.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
// 4294967295 = 0x FF FF FF FF
// 2694881440 = 0x A0 A0 A0 A0

static uint32_t data[] = {4294967295, 0 , 2694881440, 0 };

//uint32_t data[16] __attribute__((aligned(4))) =
//{
//		0, 2, 4, 6, // 4 samples
//		8,10,12,14,
//		1,3,5,7,
//		9,11,13,15
//};

//uint32_t data[16] __attribute__((aligned(4))) =
//{
//		16, 2, 4, 6, // 4 samples
//		8,10,12,14,
//		1,3,5,7,
//		9,11,13,15
//};

//uint32_t data[32] __attribute__((aligned(1024))) =
//{
//		1, 2, 3, 4, // 4 samples
//		5, 6, 7, 8,
//		9, 10, 11, 12,
//		13,14,15, 16,
//		17,18,19,20,
//		21,22,23,24,
//		25,26,27,28,
//		29,30,31,32
//};

uint32_t dst[50] __attribute__((aligned(1024)));
			 //__attribute__((aligned(4)));


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void process(void){ // this function is called when DMA has transfered one block (half of the total)

}



/* Función que se llama 1 vez, al comienzo del programa */


void App_Init (void)
{
    //gpioMode(PIN_LED_BLUE, OUTPUT);
    i2s_init();
    DMA0_Config(process); // 4 samples , 4 bytes (uint32_t)
    DMA0_ConfigClassic(7, 4, data, dst); // mod 32
    DMA0_EnableRequest();
}



/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
//	DMA0_GenerateRequest();
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/*******************************************************************************
 ******************************************************************************/