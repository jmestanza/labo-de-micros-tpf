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
static I2S_Type * i2s_ptr = I2S0;
/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
    //gpioMode(PIN_LED_BLUE, OUTPUT);
    i2s_init();
}

#define EVENT 500

// deberia tener un timer con la frecuencia de muestreo 48KHz y usar i2s_send_data cada ese tiempo

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	static int i = 0;
	static int a = 0;
	// cuando voy a enviar data tengo que habilitar el modulo y cuando termine de mandar
	// lo deshabilito
	if(i == 0){
		i2s_send_data(1894778754); // 0x70f00782 = 0x70f007xx
	}
	if(i == 25){
		i2s_ptr->TCSR &= ~I2S_TCSR_TE_MASK; // deshabilitamos el transfer enable
	}

	if(i == 50){
		i2s_ptr->TCSR = I2S_TCSR_FR_MASK;

	  /* Set the software reset and FIFO reset to clear internal state */
//		i2s_ptr->TCSR = I2S_TCSR_SR_MASK | I2S_TCSR_FR_MASK;

		/* Clear software reset bit, this should be done by software */
//		i2s_ptr->TCSR &= ~I2S_TCSR_SR_MASK;
	}



	if(i == 75){
		i2s_ptr->TCSR |= I2S_TCSR_TE_MASK; // habilitamos el transfer enable
//		i2s_send_data(0xf0f0f0aa); // 0x70f00782 = 0x70f007xx
		i2s_send_data(0xaaf0f0f0); // 0x70f00782 = 0x70f007xx

	}

	if(i == 100){
		i2s_ptr->TCSR &= ~I2S_TCSR_TE_MASK; // deshabilitamos el transfer enable
	}

//	if(i == 50){
//		i2s_ptr->TCSR &= ~I2S_TCSR_TE_MASK; // deshabilitamos el transfer enable
//	}

//	if(i%EVENT == 0){
//		i2s_send_data(1894778754); // 0x70f00782 = 0x70f007xx
//		a = 5 ;
//
//	}else if((i-(EVENT/2))%EVENT == 0){
//		i2s_send_data(0); // 0x70f00782 = 0x70f007xx
//	}

	i++;

// dependiendo de cuanto deje durar EVENT,
// si lo hago mas chico aparecen menos frames...?
// si lo dejo durar mas aparecen mas frames????

// no se porque aparecen tantos frames si lo unico que hice fue escribir 1 dato.
// lo cual deberia ser exactamente 1 frame...

// es como si quedara informacion en el TDR (Transfer Data register)


//  toma los 24 bits mas significativos!!!
// el problema es que es medio random cuando se vacia la FIFO

}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/*******************************************************************************
 ******************************************************************************/
