


#ifndef __HAL_CONFIG__
#define __HAL_CONFIG__


// define board
#define BOARD_K10 0
#define BOARD_K40 1
#define BOARD_K50 2
#define BOARD_K60 3
#define BOARD BOARD_K60

#define SYS_CLK_KHZ 48000
#define BUS_CLK_KHZ (SYS_CLK_KHZ/2)
#define VEC_RELOAD 0x1fff0000

#define CONSOLE_BAUDRATE 115200

#endif






