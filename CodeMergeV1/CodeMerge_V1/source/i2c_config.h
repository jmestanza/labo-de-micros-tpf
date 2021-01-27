#ifndef I2C_H_
#define I2C_H_


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef void (* pfunc) (void);


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define I2C_MASTER_BASEADDR 		I2C0
#define I2C_MASTER_CLK_SRC          I2C0_CLK_SRC
#define I2C_MASTER_CLK_FREQ         CLOCK_GetFreq(I2C0_CLK_SRC)
#define I2C_MASTER_IRQ              I2C0_IRQn
#define I2C_MASTER_IRQHandler       I2C0_IRQHandler
#define I2C_BAUDRATE               100000U



/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initialize i2c driver
 * Initialize I2C0 (SCL -> PTE24 ; SDA -> PTE25)
*/
void i2cInit (void);

/**
 * @brief Read a received message. Non-Blocking
*/
//void i2cReadMsg(uint8_t device_address, uint8_t * data, uint8_t data_size, uint8_t register_address);

void i2cReadMsg (uint8_t device_address, uint8_t * data, uint8_t data_size, uint8_t register_address);
/**
 * @brief Write a message to be transmitted. Non-Blocking
*/
void i2cWriteMsg(uint8_t device_address, uint8_t * data, uint8_t data_size, uint8_t register_address);

bool i2cReadMsgBlocking (uint8_t device_address, uint8_t * data, uint8_t data_size, uint8_t register_address);

bool i2cWriteMsgBlocking (uint8_t device_address, uint8_t * data, uint8_t data_size, uint8_t register_address );

bool i2cgetCompletition(void);


/*******************************************************************************
 ******************************************************************************/

#endif /* I2C_H_ */
