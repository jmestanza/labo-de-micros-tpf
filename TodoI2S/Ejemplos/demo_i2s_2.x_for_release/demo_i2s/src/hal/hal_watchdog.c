
#include "..\\include\\global.h"
#include "..\\include\\MKxxx.h"
#include "..\\include\\hal_config.h"


void hal_watchdog_close(void)
{
    WDOG_UNLOCK  = 0xC520;
    WDOG_UNLOCK  = 0xD928;
    WDOG_STCTRLH &= ~1UL;
}







