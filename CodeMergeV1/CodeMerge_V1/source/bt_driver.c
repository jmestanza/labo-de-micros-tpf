/*
 * bt_driver.c
 *
 *  Created on: 22 dic. 2020
 *      Author: Marcelo
 */
#include "bt_driver.h"
#include "board.h"
#include "peripherals.h"
/*
 *  - Esto basicamente son una interrupcion por UART y un TIM
 * Cuando se conecta un dispositivo, este envia un "Device Connected"
 * el sistema devuelve un "Ack"
 * Esto activa el timer periodico con el que se enviara la informacion de
 * las 3 variables, luego de cada set enviado se espera un "ACK"
 * Si se desconecta, en el siguiente ciclo de envio el sistema no recibiria el "ACK"
 * por lo que se pasa a estado de "Device not Connected"
 */

/*
 * General variables
 */
bool state = DEVICE_NOT_CON;

char bufferUART[BUFFER_SIZE];
char *buffer_ptr = bufferUART; // Porque no puedo comparar directamente con string sino

uint8_t data2send[] = "0512_060_367_9915"; // Mensaje de prueba

uint16_t aux = 0;
uint16_t cont = 0;
bool txOnGoing  = false;

const char *dev_command = "NewDevice";
const char *dev_ack = "DeviceAck";

uint8_t rxIndex = 0;
bool is_devAck = false;
uint16_t timeOut = 0;

/*
 * Function declarations
 */
void clear_buffer(void);

/*
 * Function definitions
 */

void bt_init(void)
{
	/*
	 * Deberia inicializar uart y timer x200ms
	 * O no, y dejar las interrupciones en main con los callbacks
	 */
	data2send[8] = '\n';
}

void bt_callback()
{
    uint8_t data;

    /* If new data arrived. */
    if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(UART_2_PERIPHERAL))
    {
        data = UART_ReadByte(UART_2_PERIPHERAL);

        /* If \n is not received, add data to buffer. */
        if (data != '_') // Se me ocurrio usar '_' como terminador especial
        {
            bufferUART[rxIndex] = data;
            rxIndex++;
            if(rxIndex == BUFFER_SIZE)
            {
            	clear_buffer();
                LED_BLUE_TOGGLE();
                LED_GREEN_TOGGLE();
            }
        }
        else
        {
        	bufferUART[rxIndex] = '\0';
        	if(*buffer_ptr == *dev_command)
        	{
        		clear_buffer();
        		FTM_StartTimer(FTM_1_PERIPHERAL, kFTM_SystemClock);
        		is_devAck = true;
        		LED_BLUE_OFF();
        		LED_GREEN_TOGGLE();
        		state = DEVICE_CON;
        	}
        	else if(*bufferUART == *dev_ack)
        	{
        		clear_buffer();
        		is_devAck = true;
        	}
        }
    }

    /* If sending data. */
    if(((kUART_TxDataRegEmptyFlag) & UART_GetStatusFlags(UART_2_PERIPHERAL)) && (txOnGoing))
    {
    	UART_WriteByte(UART_2_PERIPHERAL, data2send[cont]);
    	if(cont == 8) // Esto deberia ser el size de los datos
    	{
    		UART_DisableInterrupts(UART_2_PERIPHERAL, kUART_TxDataRegEmptyInterruptEnable);
    		cont = 0;
    		txOnGoing = false;
    	}
    	else
    	{
    		cont++;
    	}
    }

}

void bt_tim_callback(void)
{
	/*
	 * Se llama cada 100ms cuando hay un dispositivo conectado
	 */
	if(is_devAck)
	{
		/* Se envia byte a byte mediante interrupcion */
		UART_EnableInterrupts(UART_2_PERIPHERAL, kUART_TxDataRegEmptyInterruptEnable);
		txOnGoing = true;
		timeOut = 0;
		is_devAck = false;
	}
	else
	{
		/*
		 * Son 3 segundos de espera de Ack, sino toma como que se desconecto y detiene el timer
		 */
		timeOut++;
		if(timeOut == TIMEOUT_LIMIT)
		{
			FTM_StopTimer(FTM_1_PERIPHERAL);
			LED_GREEN_OFF();
			timeOut = 0;
			state = DEVICE_NOT_CON;
		}
	}

	if(!(aux % 10))
	{
		LED_GREEN_TOGGLE(); // Para saber que esta andando
	}
	aux++;
}

_Bool bt_getState(void)
{
	return state;
}

void clear_buffer(void)
{
	uint8_t i = 0;
	for(i=0;i<BUFFER_SIZE;i++)
	{
		bufferUART[i] = '\0';
	}
	rxIndex = 0;
}
