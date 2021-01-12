/***************************************************************************//**
  @file     board.h
  @brief    Board management
  @author   Nicolás Magliola
 ******************************************************************************/

#ifndef _BOARD_H_
#define _BOARD_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "gpio.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/***** BOARD defines **********************************************************/

// On Board User LEDs
//#define PIN_LED_BLUE    PORTNUM2PIN(PB,21) // PTB21

#define PIN_I2S_TX_BLCK	PORTNUM2PIN(PB,18)
#define PIN_I2S_TX_FS	PORTNUM2PIN(PB,19)
#define PIN_I2S_TX_D0	PORTNUM2PIN(PC,1)
#define PIN_I2S_MCLK  	PORTNUM2PIN(PC,8)
#define PIN_I2S_RX_BCLK PORTNUM2PIN(PC,9)
#define PIN_I2S_RX_FS	PORTNUM2PIN(PC,7)
#define PIN_I2S_RX_D0 	PORTNUM2PIN(PC,5)

//#define PIN_USB0_SOF_OUT PORTNUM2PIN(PC,0) ????? don't use

#define LED_ACTIVE      LOW


/*******************************************************************************
 ******************************************************************************/

#endif // _BOARD_H_
