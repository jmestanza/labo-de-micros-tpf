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

char bufferUART[BUFFER_SIZE_BT];
char *buffer_ptr = bufferUART; // Porque no puedo comparar directamente con string sino

uint8_t data2send[] = "0512_060_367_9915X"; // Mensaje de prueba

uint8_t penalty = 0;
uint16_t aux = 0;
uint16_t cont = 0;
bool txOnGoing  = false;

const char *dev_command = "NewDevice";
const char *dev_ack = "DeviceAck";

const char *cmd_ecg_oor = "MP3.ECG..";
const char *cmd_spo2_oor = "MP3.SPO..";
const char *cmd_temp_oor = "MP3.TEM..";

uint8_t rxIndex = 0;
bool is_devAck = false;
uint16_t timeOut = 0;

void (*sound_callback)(char*);
/*
 * Function declarations
 */
void clear_buffer(void);

/*
 * Function definitions
 */

void bt_init(void (*callback)(char*))
{
	data2send[17] = '\n';
	sound_callback = callback;
}

void bt_callback()
{
    uint8_t data;

    /* If new data arrived. */
    if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(UART_2_PERIPHERAL))
    {
        data = UART_ReadByte(UART_2_PERIPHERAL);

        /* If '_' is not received, add data to buffer. */
        if (data != '_') // Se me ocurrio usar '_' como terminador especial
        {
        	bufferUART[rxIndex] = data;
            rxIndex++;
            if(rxIndex >= BUFFER_SIZE_BT)
            {
            	clear_buffer();
            }
        }
        else
        {
        	bufferUART[rxIndex] = '\0';
        	if(!strcmp(&bufferUART[rxIndex-9],dev_command))
        	{
        		clear_buffer();
        		is_devAck = true;
        		LED_RED_OFF();
        		state = DEVICE_CON; // Enables PIT entering
        	}
        	else if(!strcmp(&bufferUART[rxIndex-9],dev_ack))
        	{
        		clear_buffer();
        		is_devAck = true;
        	}
        	else if(!strcmp(&bufferUART[rxIndex-9],cmd_ecg_oor))
        	{
        		sound_callback(&bufferUART[rxIndex-9]);
        		clear_buffer();
        	}
        	else if(!strcmp(&bufferUART[rxIndex-9],cmd_spo2_oor))
        	{
        		sound_callback(&bufferUART[rxIndex-9]);
        		clear_buffer();
        	}
        	else if(!strcmp(&bufferUART[rxIndex-9],cmd_temp_oor))
        	{
        		sound_callback(&bufferUART[rxIndex-9]);
        		clear_buffer();
        	}
        	else
			{
        		clear_buffer();
			}
        }
    }

//    /* If sending data. */
//    if(((kUART_TxDataRegEmptyFlag) & UART_GetStatusFlags(UART_2_PERIPHERAL)) && (txOnGoing))
//    {
//    	UART_WriteByte(UART_2_PERIPHERAL, data2send[cont]);
//    	if(cont == 17) // Esto deberia ser el size de los datos
//    	{
//    		UART_DisableInterrupts(UART_2_PERIPHERAL, kUART_TxDataRegEmptyInterruptEnable);
//    		cont = 0;
//    		clear_buffer();
//    		txOnGoing = false;
//    	}
//    	else
//    	{
//    		cont++;
//    	}
//    }

}

void bt_tim_callback(void)
{
	/*
	 * Se llama cada 200ms cuando hay un dispositivo conectado
	 */
	if(is_devAck)
	{
		/* Se envia byte a byte mediante interrupcion */
//		UART_EnableInterrupts(UART_2_PERIPHERAL, kUART_TxDataRegEmptyInterruptEnable);
		//txOnGoing = true;
		timeOut = 0;
		is_devAck = false;
		penalty = 0;
		//clear_buffer();
		UART_WriteBlocking(UART_2_PERIPHERAL, data2send, 17);
		//txOnGoing = false;
	}
	else
	{
		/*
		 * Son 3 segundos de espera de Ack, sino toma como que se desconecto y detiene el timer
		 */
		timeOut++;
		if(timeOut == TIMEOUT_LIMIT)
		{
			penalty++;
			if(penalty == MAX_PENALTY) // Max tries reached
			{
				LED_GREEN_OFF();
				LED_RED_ON();
				state = DEVICE_NOT_CON; // Disables PIT entering
				penalty = 0;
			}
			else // Try new send
			{
				UART_WriteBlocking(UART_2_PERIPHERAL, data2send, 17);
			}
			clear_buffer();
			timeOut = 0;
		}
	}

	if(!(aux % 5))
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
	for(i=0;i<BUFFER_SIZE_BT;i++)
	{
		bufferUART[i] = '\0';
	}
	rxIndex = 0;
}
