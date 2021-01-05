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


// deberia tener un timer con la frecuencia de muestreo 48KHz y usar i2s_send_data cada ese tiempo

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	static int i = 0;
	// cuando voy a enviar data tengo que habilitar el modulo y cuando termine de mandar
	// lo deshabilito
	if(i == 0){
		i2s_enable_tx();
		i2s_send_data(0x70f00782); //  0x70f007xx
	}
	if(i == 25){
		i2s_disable_tx();
	}
	if(i == 50){
		i2s_reset_tx_fifo();
	}
	if(i == 75){
		i2s_enable_tx();
		i2s_send_data(0xaaf0f0f0); // 0xaaf0f0f0 = 0xaaf0f0xx , ignora los ultimos 8 bits
	}
	if(i == 100){
		i2s_disable_tx(); // deshabilitamos el transfer enable
	}

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
