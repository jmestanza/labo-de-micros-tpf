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
#include "fsl_sai.h"
#include "fsl_clock.h"
#include "fsl_i2c.h"
#include "fsl_uart.h"
#include "fsl_pit.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
/* Definitions for BOARD_InitPeripherals functional group */
/* Definition of peripheral ID */
#define SAI_1_PERIPHERAL I2S0
/* SAI_1 interrupt vector ID (number). */
#define SAI_1_SERIAL_TX_IRQN I2S0_Tx_IRQn
/* SAI_1 interrupt handler identifier. */
#define SAI_1_SERIAL_TX_IRQHANDLER I2S0_Tx_IRQHandler
/* Master clock source frequency used for counting the master clock divider in the Tx SetFormat type functions, not available on all devices. */
#define SAI_1_TX_MCLK_SOURCE_CLOCK_HZ 20971520UL
/* Bit clock source frequency used for counting the bit clock divider in the Tx SetFormat type functions. */
#define SAI_1_TX_BCLK_SOURCE_CLOCK_HZ 12288000UL
/* BOARD_InitPeripherals defines for I2C0 */
/* Definition of peripheral ID */
#define I2C_1_PERIPHERAL I2C0
/* Definition of the clock source */
#define I2C_1_CLOCK_SOURCE I2C0_CLK_SRC
/* Definition of the clock source frequency */
#define I2C_1_CLK_FREQ CLOCK_GetFreq(I2C_1_CLOCK_SOURCE)
/* Definition of peripheral ID */
#define UART_1_PERIPHERAL UART0
/* Definition of the clock source frequency */
#define UART_1_CLOCK_SOURCE CLOCK_GetFreq(UART0_CLK_SRC)
/* BOARD_InitPeripherals defines for PIT */
/* Definition of peripheral ID. */
#define PIT_1_PERIPHERAL PIT
/* Definition of clock source. */
#define PIT_1_CLOCK_SOURCE kCLOCK_BusClk
/* Definition of clock source frequency. */
#define PIT_1_CLK_FREQ CLOCK_GetFreq(PIT_1_CLOCK_SOURCE)
/* Definition of ticks count for channel 0. */
#define PIT_1_0_TICKS USEC_TO_COUNT(20U, PIT_1_CLK_FREQ) - 1U
/* PIT_1 interrupt vector ID (number). */
#define PIT_1_0_IRQN PIT0_IRQn
/* PIT_1 interrupt handler identifier. */
#define PIT_1_0_IRQHANDLER PIT0_IRQHandler

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

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
extern const sai_config_t SAI_1_tx_config;
extern sai_transfer_format_t SAI_1_tx_format;
extern const i2c_master_config_t I2C_1_config;
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
