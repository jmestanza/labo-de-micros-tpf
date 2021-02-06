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
#define TIMEOUT_LIMIT 15 // 3 Seg
#define MAX_PENALTY 3


/*
 * Function declarations
 */

/*
 * bt_init: init uart and timer drivers to use
 */
void bt_init(void (*callback)(char*));

_Bool bt_getState(void);

void bt_setBpmGFX(uint16_t bpmGFX);
void bt_setBpmValue(uint16_t bpmValue);

void bt_callback(void);

void bt_tim_callback(void);

#endif /* BT_DRIVER_H_ */
