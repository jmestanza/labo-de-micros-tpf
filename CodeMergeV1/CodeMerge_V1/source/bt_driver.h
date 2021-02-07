/*
 * bt_driver.h
 *
 *  Created on: 22 dic. 2020
 *      Author: Marcelo
 */

#ifndef BT_DRIVER_H_
#define BT_DRIVER_H_

#include <stdint.h>
/*
 * General defines
 */
#define DEVICE_CON 1
#define DEVICE_NOT_CON 0
#define BUFFER_SIZE_BT 16
#define TIMEOUT_LIMIT 120 // 3 Seg
#define MAX_PENALTY 3

/*
 * Function declarations
 */

void bt_init(void (*callback)(char*), uint8_t plotQty);

void bt_setBpmPlot(uint16_t *bpmGFX);
void bt_setBpmValue(uint16_t bpmValue);
void bt_setSpo2Plot(uint16_t *spo2GFX);
void bt_setSpo2Value(float spo2Value);
void bt_setTempValue(float tempValue);

_Bool bt_isConnected(void);

void bt_callback(void);

void bt_tim_callback(void);

#endif /* BT_DRIVER_H_ */
