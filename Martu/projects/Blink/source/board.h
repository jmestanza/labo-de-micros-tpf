/*************************//**
  @file     board.h
  @brief    Board management
  @author   Nicolás Magliola
 **************************/

#ifndef BOARD_H
#define BOARD_H

/***************************
 * INCLUDE HEADER FILES
 **************************/

#include "gpio.h"


/***************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 **************************/

/** BOARD defines *********************/

// On Board User LEDs
#define PIN_LED_RED     PORTNUM2PIN(PB,22)
#define PIN_LED_GREEN   PORTNUM2PIN(PE,26)
#define PIN_LED_BLUE    PORTNUM2PIN(PB,21) // PTB21
#define PIN_PRUEBA 		PORTNUM2PIN(PB,9)

//Accelerometer pins
#define I2C_SDA			PORTNUM2PIN(PE,25) //PTE25
#define I2C_SCL			PORTNUM2PIN(PE,24) //PTE24

#define LED_ACTIVE      LOW
#define SW_ACTIVE       // ???
#define SW_INPUT_TYPE   // ???


/***************************
 **************************/

#endif // BOARD_H
