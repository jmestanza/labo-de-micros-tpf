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


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
    //gpioMode(PIN_LED_BLUE, OUTPUT);
    i2s_init();
}
// senoidal de 0 a 2^24-1, 64 muestras, 750Hz ponele
//uint32_t sinWave[]={
//		0x800000,0x8c8bd3,0x98f8b8,0xa5280c,0xb0fbc5,0xbc56ba,0xc71cec,0xd133cc,
//		0xda8279,0xe2f201,0xea6d98,0xf0e2cb,0xf641ae,0xfa7d04,0xfd8a5e,0xff6236,
//		0xffffff,0xff6236,0xfd8a5e,0xfa7d04,0xf641ae,0xf0e2cb,0xea6d98,0xe2f201,
//		0xda8279,0xd133cc,0xc71cec,0xbc56ba,0xb0fbc5,0xa5280c,0x98f8b8,0x8c8bd3,
//		0x800000,0x73742c,0x670747,0x5ad7f3,0x4f043a,0x43a945,0x38e313,0x2ecc33,
//		0x257d86,0x1d0dfe,0x159267,0xf1d34,0x9be51,0x582fb,0x275a1,0x9dc9,
//		0x0,0x9dc9,0x275a1,0x582fb,0x9be51,0xf1d34,0x159267,0x1d0dfe,
//		0x257d86,0x2ecc33,0x38e313,0x43a945,0x4f043a,0x5ad7f3,0x670747,0x73742c
//};

uint32_t sinWave[]={
		0x800000,0xffffff,0x800000,0x0,0x800000
};


// deberia tener un timer con la frecuencia de muestreo 48KHz y usar i2s_send_data cada ese tiempo


#define EVENT 10 // valor optimo

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	static int i = 0;
	static int index = 0;
	// cuando voy a enviar data tengo que habilitar el modulo y cuando termine de mandar
	// lo deshabilito

	if(i%EVENT == 0){
		i2s_enable_tx();
		i2s_send_data(sinWave[index]<<8); // ignora los 8 bits mas bajos por ej-> 0x70f007xx
	}
//		else if((i-25)%EVENT == 0){
//		i2s_disable_tx(); // no se q onda esto parece que caga el frame sync
	if((i+5)%EVENT == 0){
		i2s_reset_tx_fifo();
	}

	index = (index+1)%sizeof(sinWave);
	i++;

	// notas:
	// No se cuando termina de escribir la fifo, respecto de cuando se escribio en el TDR.
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/*******************************************************************************
 ******************************************************************************/
