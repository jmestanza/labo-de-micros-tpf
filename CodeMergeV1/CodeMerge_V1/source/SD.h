/*
 * SD.h
 *
 *  Created on: Feb 3, 2021
 *      Author: joa-m
 */

#ifndef SD_H_
#define SD_H_


#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"
#include "fsl_debug_console.h"

FRESULT SD_init(void);
status_t sdcardWaitCardInsert(void);


#endif /* SD_H_ */
