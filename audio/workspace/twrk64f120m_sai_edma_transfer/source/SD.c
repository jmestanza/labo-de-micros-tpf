/*
 * SD.c
 *
 *  Created on: Feb 3, 2021
 *      Author: joa-m
 */

#include "SD.h"

const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
static const sdmmchost_detect_card_t s_sdCardDetect = {
#ifndef BOARD_SD_DETECT_TYPE
    .cdType = kSDMMCHOST_DetectCardByGpioCD,
#else
    .cdType = BOARD_SD_DETECT_TYPE,
#endif
    .cdTimeOut_ms = (~0U),
};

FATFS g_fileSystem;


FRESULT SD_init(void){
	FRESULT error;
	PRINTF("\r\nFATFS example to demonstrate how to use FATFS with SD card.\r\n");

	PRINTF("\r\nPlease insert a card into board.\r\n");

	if (sdcardWaitCardInsert() != kStatus_Success)
	{
		return -1;
	}

	if (f_mount(&g_fileSystem, driverNumberBuffer, 0U))
	{
		PRINTF("Mount volume failed.\r\n");
		return -1;
	}

#if (FF_FS_RPATH >= 2U)
	error = f_chdrive((char const *)&driverNumberBuffer[0U]);
	if (error)
	{
		PRINTF("Change drive failed.\r\n");
		return -1;
	}
#endif
	return error;
}




status_t sdcardWaitCardInsert(void)
{
    /* Save host information. */
    g_sd.host.base = SD_HOST_BASEADDR;
    g_sd.host.sourceClock_Hz = SD_HOST_CLK_FREQ;
    /* card detect type */
    g_sd.usrParam.cd = &s_sdCardDetect;
#if defined DEMO_SDCARD_POWER_CTRL_FUNCTION_EXIST
    g_sd.usrParam.pwr = &s_sdCardPwrCtrl;
#endif
    /* SD host init function */
    if (SD_HostInit(&g_sd) != kStatus_Success)
    {
        PRINTF("\r\nSD host init fail\r\n");
        return kStatus_Fail;
    }
    /* power off card */
    SD_PowerOffCard(g_sd.host.base, g_sd.usrParam.pwr);
    /* wait card insert */
    if (SD_WaitCardDetectStatus(SD_HOST_BASEADDR, &s_sdCardDetect, true) == kStatus_Success)
    {
        PRINTF("\r\nCard inserted.\r\n");
        /* power on the card */
        SD_PowerOnCard(g_sd.host.base, g_sd.usrParam.pwr);
    }
    else
    {
        PRINTF("\r\nCard detect fail.\r\n");
        return kStatus_Fail;
    }

    return kStatus_Success;
}
