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

#define EVENT 500

// deberia tener un timer con la frecuencia de muestreo 48KHz y usar i2s_send_data cada ese tiempo

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	static int i = 0;
	static int a = 0;
	// cuando voy a enviar data tengo que habilitar el modulo y cuando termine de mandar
	// lo deshabilito

	if(i%EVENT == 0){
		i2s_send_data(1894778754); // 0x70f00782 = 0x70f007xx
		a = 5 ;

	}else if((i-(EVENT/2))%EVENT == 0){
		i2s_send_data(0); // 0x70f00782 = 0x70f007xx
	}

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
