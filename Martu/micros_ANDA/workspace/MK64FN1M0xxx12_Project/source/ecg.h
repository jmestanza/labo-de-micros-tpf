/*
 * ecg.h
 *
 *  Created on: Jan 18, 2021
 *      Author: martinamaspero
 */

#ifndef ECG_H_
#define ECG_H_

#include "stdio.h"

#define LO_MAS PORTNUM2PIN(PB,18)
#define LO_MENOS PORTNUM2PIN(PB,18)
void ecg_init();


#endif /* ECG_H_ */
