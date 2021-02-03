/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "i2c_config.h"
#include "fsl_i2c.h"
#include "fsl_port.h"
#include "fsl_common.h"
#include "fsl_clock.h"
#include "pin_mux.h"



/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/



/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

i2c_master_config_t masterConfig;
i2c_master_handle_t g_m_handle;

uint32_t sourceClock          = 0;
static void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData);
volatile bool completionFlag = true;
volatile bool nakFlag        = false;


void i2cInit ()
{

	CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

	  const port_pin_config_t porte24_pin31_config = {
	    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
	    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
	    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
	    kPORT_OpenDrainEnable,                                   /* Open drain is enabled */
	    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
	    kPORT_MuxAlt5,                                           /* Pin is configured as I2C0_SCL */
	    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	  };
	  PORT_SetPinConfig(PORTE, 24u, &porte24_pin31_config); /* PORTE24 (pin 31) is configured as I2C0_SCL */
	  const port_pin_config_t porte25_pin32_config = {
	    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
	    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
	    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
	    kPORT_OpenDrainEnable,                                   /* Open drain is enabled */
	    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
	    kPORT_MuxAlt5,                                           /* Pin is configured as I2C0_SDA */
	    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	  };
	  PORT_SetPinConfig(PORTE, 25u, &porte25_pin32_config); /* PORTE25 (pin 32) is configured as I2C0_SDA */

	  I2C_MasterTransferCreateHandle(I2C0, &g_m_handle, i2c_master_callback, NULL);


	/*
	 * masterConfig.baudRate_Bps = 100000U;
	 * masterConfig.enableStopHold = false;
	 * masterConfig.glitchFilterWidth = 0U;
	 * masterConfig.enableMaster = true;
	 */
	I2C_MasterGetDefaultConfig(&masterConfig);

	masterConfig.baudRate_Bps = I2C_BAUDRATE;

	sourceClock = I2C_MASTER_CLK_FREQ;

	I2C_MasterInit(I2C0, &masterConfig, sourceClock);


}

void i2cWriteMsg(uint8_t device_address, uint8_t * data, uint8_t data_size, uint8_t register_address)
{
	i2c_master_transfer_t masterXfer;
	memset(&masterXfer, 0, sizeof(masterXfer));

	    masterXfer.slaveAddress   = device_address;
	    masterXfer.direction      = kI2C_Write;
	    masterXfer.subaddress     = register_address;
	    masterXfer.subaddressSize = 1;
	    masterXfer.data           = data;
	    masterXfer.dataSize       = data_size;
	    masterXfer.flags          = kI2C_TransferDefaultFlag;



	    if(completionFlag == true)
	    {
		    completionFlag = false;


		    I2C_MasterTransferNonBlocking(I2C0, &g_m_handle, &masterXfer);
	    }


}




void i2cReadMsg (uint8_t device_address, uint8_t * data, uint8_t data_size, uint8_t register_address)
{
	i2c_master_transfer_t masterXfer;
	 memset(&masterXfer, 0, sizeof(masterXfer));
	    masterXfer.slaveAddress   = device_address;
	    masterXfer.direction      = kI2C_Read;
	    masterXfer.subaddress     = register_address;
	    masterXfer.subaddressSize = 1;
	    masterXfer.data           = data;
	    masterXfer.dataSize       = data_size;
	    masterXfer.flags          = kI2C_TransferDefaultFlag;

	    if(completionFlag == true)
	    {
		    completionFlag = false;
		    nakFlag=false;

		    I2C_MasterTransferNonBlocking(I2C0, &g_m_handle, &masterXfer);
	    }
}


bool i2cReadMsgBlocking (uint8_t device_address, uint8_t * data, uint8_t data_size, uint8_t register_address)
{
	i2c_master_transfer_t masterXfer;
	 memset(&masterXfer, 0, sizeof(masterXfer));
	    masterXfer.slaveAddress   = device_address;
	    masterXfer.direction      = kI2C_Read;
	    masterXfer.subaddress     = register_address;
	    masterXfer.subaddressSize = 1;
	    masterXfer.data           = data;
	    masterXfer.dataSize       = data_size;
	    masterXfer.flags          = kI2C_TransferDefaultFlag;

	    if(completionFlag == true)
	    {
		    completionFlag = false;
		    nakFlag=false;

		    I2C_MasterTransferNonBlocking(I2C0, &g_m_handle, &masterXfer);
		    while ((!nakFlag) &&(!completionFlag))
		    {
		    }

		    if (completionFlag == true)
		    {
		        return true;
		    }
		    else
		    {
		    	return false;
		    }
	    }
	    else
	    {
	        return false;
	    }
}

bool i2cWriteMsgBlocking (uint8_t device_address, uint8_t * data, uint8_t data_size, uint8_t register_address )
{
	i2c_master_transfer_t masterXfer;
	memset(&masterXfer, 0, sizeof(masterXfer));

	    masterXfer.slaveAddress   = device_address;
	    masterXfer.direction      = kI2C_Write;
	    masterXfer.subaddress     = register_address;
	    masterXfer.subaddressSize = 1;
	    masterXfer.data           = data;
	    masterXfer.dataSize       = data_size;
	    masterXfer.flags          = kI2C_TransferDefaultFlag;



	    if(completionFlag == true)
	    {
		    completionFlag = false;
		    nakFlag=false;

		    I2C_MasterTransferNonBlocking(I2C0, &g_m_handle, &masterXfer);
		    while ((!nakFlag) &&(!completionFlag))
		    {
		    }

		    if (completionFlag == true)
		    {
		        return true;
		    }
		    else
		    {
		    	return false;
		    }
	    }
	    else
	    {
	        return false;
	    }
}


bool i2cgetCompletition(void)
{
	return completionFlag;
}

static void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData)
{
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success)
    {
        completionFlag = true;
    }
    /* Signal transfer success when received success status. */
    if ((status == kStatus_I2C_Nak) || (status == kStatus_I2C_Addr_Nak))
    {
        nakFlag = true;
    }
}



