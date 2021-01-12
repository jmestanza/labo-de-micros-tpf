



#include "..\\include\\global.h"
#include "..\\include\\MKxxx.h"
#include "..\\include\\hal_config.h"
#include "..\\include\\hal_clock.h"
#include "..\\include\\hal_dma.h"
#include "..\\include\\hal_i2c.h"
#include "..\\include\\hal_i2s.h"
#include "..\\include\\hal_int.h"
#include "..\\include\\hal_uart.h"
#include "..\\include\\hal_watchdog.h"


static void show_board_info(void)
{
    printf("\n..........................");
#if BOARD == BOARD_K60
    printf("BOARD_TWR_K60\n");
#elif BOARD == BOARD_K50
    printf("BOARD_TWR_K50\n");
#elif BOARD == BOARD_K40
    printf("BOARD_TWR_K40\n");
#elif BOARD == BOARD_K10
    printf("BOARD_TWR_K10\n");
#endif
}

void hal_init(void)
{
    hal_watchdog_close();
    hal_clock_init();

    hal_uart_init();
    show_board_info();

    hal_relocation();
    hal_int_enable();
}





