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
 * Private defines
 */
#define BPM_PLT_POS 0 // De 0 a 99
#define BPM_VAL_POS 12*(2 + 1) // Son 12 valores con dos digitos mas el "_"
#define SPO_PLT_POS BPM_VAL_POS + (3 + 1) // Los tres dígitos mas el "_"
#define SPO_VAL_POS SPO_PLT_POS + 12*(2 + 1) // Son 12 valores con dos digitos mas el "_"
#define TEM_VAL_POS SPO_VAL_POS + (4 + 1) // Los 4 digitos mas el "_"

// PlotBpm_ + DataBpm_ + PlotSpo2_ + DataSpo2_ + DataTemp_
#define DATA_SIZE 12*3 + 4 + 12*3 + 5 + 3 + 1
/*
 * General variables
 */
_Bool state = DEVICE_NOT_CON;

char bufferUART[BUFFER_SIZE_BT];
char *buffer_ptr = bufferUART; // Porque no puedo comparar directamente con string sino
				//     0123456789ABCDEFGH
uint8_t data2send[DATA_SIZE]; // Mensaje de prueba

uint8_t penalty = 0;
uint8_t points = 0;
uint16_t aux = 0;
uint16_t cont = 0;

const char *dev_command = "NewDevice";
const char *dev_ack = "DeviceAck";

const char *cmd_ecg_oor = "MP3.ECG..";
const char *cmd_spo2_oor = "MP3.SPO..";
const char *cmd_temp_oor = "MP3.TEM..";
const char *cmd_dedo = "MP3.FIN..";

uint8_t rxIndex = 0;
_Bool is_devAck = false;
uint16_t timeOut = 0;

void (*sound_callback)(char*);
/*
 * Function declarations
 */
void clear_buffer(void);

/*
 * Function definitions
 */

void bt_init(void (*callback)(char*), uint8_t plotQty)
{
	points = plotQty;
	sound_callback = callback;
	for(uint16_t i=0;i<DATA_SIZE;i++)
	{
		data2send[i] = '_';
	}
	data2send[DATA_SIZE-1] = '\n';
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
        	else if(!strcmp(&bufferUART[rxIndex-9],cmd_dedo))
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
		UART_WriteBlocking(UART_2_PERIPHERAL, data2send, DATA_SIZE);
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

				UART_WriteBlocking(UART_2_PERIPHERAL, data2send, DATA_SIZE);
			}
			clear_buffer();
			timeOut = 0;
		}
	}

	if(!(aux % 20) && (state == DEVICE_CON))
	{
		LED_GREEN_TOGGLE(); // Para saber que esta andando
	}
	aux++;
}

_Bool bt_isConnected(void)
{
	return state;
}

void clear_buffer(void)
{
	memset(bufferUART,'\0',BUFFER_SIZE_BT);
//	uint8_t i = 0;
//	for(i=0;i<BUFFER_SIZE_BT;i++)
//	{
//		bufferUART[i] = '\0';
//	}
	rxIndex = 0;
}

/*
 * Parsing functions
 */
void bt_setBpmPlot(uint16_t *bpmGFX)
{
	for(uint16_t u=0;u<points;u++)
	{
		data2send[BPM_PLT_POS +3*u] = bpmGFX[u]/10 + '0';
		data2send[BPM_PLT_POS+1 +3*u] = bpmGFX[u]%10 + '0';
	}
}

void bt_setBpmValue(uint16_t bpmValue)
{
	data2send[BPM_VAL_POS] = bpmValue/100 + '0';
	uint16_t aux;
	aux = bpmValue%100;
	data2send[BPM_VAL_POS+1] = aux/10 + '0';
	data2send[BPM_VAL_POS+2] = aux%10 + '0';
}

void bt_setSpo2Plot(uint16_t *spo2GFX)
{
	for(uint16_t u=0;u<points;u++)
	{
		data2send[SPO_PLT_POS +3*u] = spo2GFX[u]/10 + '0';
		data2send[SPO_PLT_POS+1 +3*u] = spo2GFX[u]%10 + '0';
	}
}

void bt_setSpo2Value(float spo2Value)
{
	data2send[SPO_VAL_POS] = spo2Value/10 + '0';
	data2send[SPO_VAL_POS+1] = ((uint16_t)(spo2Value))%10 + '0';
	data2send[SPO_VAL_POS+2] = ((uint16_t)(spo2Value*10))%10 + '0';
	data2send[SPO_VAL_POS+3] = ((uint16_t)(spo2Value*100))%10 + '0';
}

void bt_setTempValue(float tempValue)
{
	data2send[TEM_VAL_POS] = tempValue/10 + '0';
	data2send[TEM_VAL_POS+1] = ((uint16_t)(tempValue))%10 + '0';
	data2send[TEM_VAL_POS+2] = ((uint16_t)(tempValue*10))%10 + '0';
}
