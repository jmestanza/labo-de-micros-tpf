/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "fsl_common.h"
#include "fsl_dspi.h"
#include "fsl_uart.h"
#include "fsl_clock.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_pit.h"
#include "fsl_i2c.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
/* Definitions for BOARD_InitPeripherals functional group */
/* BOARD_InitPeripherals defines for SPI0 */
/* Definition of peripheral ID */
#define SPI_0_PERIPHERAL SPI0
/* Definition of the clock source */
#define SPI_0_CLOCK_SOURCE DSPI0_CLK_SRC
/* Definition of the clock source frequency */
#define SPI_0_CLK_FREQ CLOCK_GetFreq(SPI_0_CLOCK_SOURCE)
/* Definition of peripheral ID */
#define UART_1_PERIPHERAL UART0
/* Definition of the clock source frequency */
#define UART_1_CLOCK_SOURCE CLOCK_GetFreq(UART0_CLK_SRC)
/* Alias for GPIOC peripheral */
#define GPIO_1_GPIO GPIOC
/* BOARD_InitPeripherals defines for PIT */
/* Definition of peripheral ID. */
#define PIT_1_PERIPHERAL PIT
/* Definition of clock source. */
#define PIT_1_CLOCK_SOURCE kCLOCK_BusClk
/* Definition of clock source frequency. */
#define PIT_1_CLK_FREQ CLOCK_GetFreq(PIT_1_CLOCK_SOURCE)
/* Definition of ticks count for channel 0. */
#define PIT_1_0_TICKS USEC_TO_COUNT(150000U, PIT_1_CLK_FREQ) - 1U
/* Definition of ticks count for channel 1. */
#define PIT_1_1_TICKS USEC_TO_COUNT(12500U, PIT_1_CLK_FREQ) - 1U
/* PIT_1 interrupt vector ID (number). */
#define PIT_1_0_IRQN PIT0_IRQn
/* PIT_1 interrupt handler identifier. */
#define PIT_1_0_IRQHANDLER PIT0_IRQHandler
/* PIT_1 interrupt vector ID (number). */
#define PIT_1_1_IRQN PIT1_IRQn
/* PIT_1 interrupt handler identifier. */
#define PIT_1_1_IRQHANDLER PIT1_IRQHandler

/* Definitions for BOARD_InitBUTTONsPeripheral functional group */
/* Alias for GPIOC peripheral */
#define BOARD_SW2_GPIO GPIOC
/* Alias for PORTC */
#define BOARD_SW2_PORT PORTC
/* SW2 interrupt vector ID (number). */
#define BOARD_SW2_IRQN PORTC_IRQn
/* SW2 interrupt handler identifier. */
#define BOARD_SW2_IRQHANDLER PORTC_IRQHandler
/* Alias for GPIOA peripheral */
#define BOARD_SW3_GPIO GPIOA
/* Alias for PORTA */
#define BOARD_SW3_PORT PORTA
/* SW3 interrupt vector ID (number). */
#define BOARD_SW3_IRQN PORTA_IRQn
/* SW3 interrupt handler identifier. */
#define BOARD_SW3_IRQHANDLER PORTA_IRQHandler

/* Definitions for BOARD_InitLEDsPeripheral functional group */
/* Alias for GPIOE peripheral */
#define BOARD_LEDRGB_GREEN_GPIO GPIOE
/* Alias for GPIOB peripheral */
#define BOARD_LEDRGB_RED_BLUE_GPIO GPIOB

/* Definitions for BOARD_InitDEBUG_UARTPeripheral functional group */
/* Definition of peripheral ID */
#define BOARD_DEBUG_UART_PERIPHERAL UART0
/* Definition of the clock source frequency */
#define BOARD_DEBUG_UART_CLOCK_SOURCE CLOCK_GetFreq(UART0_CLK_SRC)

/* Definitions for BOARD_InitACCELPeripheral functional group */
/* BOARD_InitACCELPeripheral defines for I2C0 */
/* Definition of peripheral ID */
#define BOARD_ACCEL_I2C_PERIPHERAL I2C0
/* Definition of the clock source */
#define BOARD_ACCEL_I2C_CLOCK_SOURCE I2C0_CLK_SRC
/* Definition of the clock source frequency */
#define BOARD_ACCEL_I2C_CLK_FREQ CLOCK_GetFreq(BOARD_ACCEL_I2C_CLOCK_SOURCE)

/* Definitions for BOARD_InitSDHCPeripheral functional group */
/* Alias for GPIOE peripheral */
#define BOARD_SDHC_DETECT_GPIO GPIOE

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
extern const dspi_master_config_t SPI_0_config;
extern const uart_config_t UART_1_config;
extern const pit_config_t PIT_1_config;
extern const uart_config_t BOARD_DEBUG_UART_config;
extern const i2c_master_config_t BOARD_ACCEL_I2C_config;

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/
void BOARD_InitPeripherals(void);
void BOARD_InitBUTTONsPeripheral(void);
void BOARD_InitLEDsPeripheral(void);
void BOARD_InitDEBUG_UARTPeripheral(void);
void BOARD_InitACCELPeripheral(void);
void BOARD_InitSDHCPeripheral(void);

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void);

#if defined(__cplusplus)
}
#endif

#endif /* _PERIPHERALS_H_ */
