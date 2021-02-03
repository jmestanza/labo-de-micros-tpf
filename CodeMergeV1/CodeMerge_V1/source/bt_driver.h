/*
 * bt_driver.h
 *
 *  Created on: 22 dic. 2020
 *      Author: Marcelo
 */

#ifndef BT_DRIVER_H_
#define BT_DRIVER_H_

/*
 * General defines
 */
#define DEVICE_CON 1
#define DEVICE_NOT_CON 0
#define BUFFER_SIZE 16
#define TIMEOUT_LIMIT 300 // 3 Seg
/*
 * Function declarations
 */

/*
 * bt_init: init uart and timer drivers to use
 */
void bt_init(void);

_Bool bt_getState(void);

void bt_callback(void);

void bt_tim_callback(void);

#endif /* BT_DRIVER_H_ */
