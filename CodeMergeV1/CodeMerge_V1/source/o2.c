#include "i2c_config.h"
#include "o2.h"


bool maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data)
{
	i2cWriteMsg(I2C_WRITE_ADDR, &uch_data, 1, uch_addr);
  	return i2cgetCompletition();

}

bool maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data)
{
	i2cReadMsg(I2C_READ_ADDR, puch_data, 1, uch_addr);
  	return i2cgetCompletition();
}

bool maxim_max30102_write_reg_blocking(uint8_t uch_addr, uint8_t uch_data)
{
	return i2cWriteMsgBlocking(I2C_WRITE_ADDR, &uch_data, 1, uch_addr);
}

bool maxim_max30102_read_reg_blocking(uint8_t uch_addr, uint8_t *puch_data)

{
	return i2cReadMsgBlocking(I2C_READ_ADDR, puch_data, 1, uch_addr);


}

bool maxim_max30102_init()
{

	  if(!maxim_max30102_write_reg_blocking(REG_INTR_ENABLE_1,0x40)) // INTR setting 0xc0
	    return false;
	  if(!maxim_max30102_write_reg_blocking(REG_INTR_ENABLE_2,0x00))
	    return false;
	  if(!maxim_max30102_write_reg_blocking(REG_FIFO_WR_PTR,0x00))  //FIFO_WR_PTR[4:0]
	    return false;
	  if(!maxim_max30102_write_reg_blocking(REG_OVF_COUNTER,0x00))  //OVF_COUNTER[4:0]
	    return false;
	  if(!maxim_max30102_write_reg_blocking(REG_FIFO_RD_PTR,0x00))  //FIFO_RD_PTR[4:0]
	    return false;
	  if(!maxim_max30102_write_reg_blocking(REG_FIFO_CONFIG,0x4f))  // 0fsample avg = 1, fifo rollover=false, fifo almost full = 17
	    return false;
	  if(!maxim_max30102_write_reg_blocking(REG_MODE_CONFIG,0x03))   //0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
	    return false;
	  if(!maxim_max30102_write_reg_blocking(REG_SPO2_CONFIG,0x27))  // SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (400uS)
	    return false;

	  if(!maxim_max30102_write_reg_blocking(REG_LED1_PA,0x24))   //Choose value for ~ 7mA for LED1
	    return false;
	  if(!maxim_max30102_write_reg_blocking(REG_LED2_PA,0x24))   // Choose value for ~ 7mA for LED2
	    return false;
	  if(!maxim_max30102_write_reg_blocking(REG_PILOT_PA,0x7f))   // Choose value for ~ 25mA for Pilot LED
	    return false;

	  if(!maxim_max30102_write_reg_blocking(REG_MULTI_LED_CTRL1,0x21))   // Choose value for ~ 25mA for Pilot LED
		return false;
	  if(!maxim_max30102_write_reg_blocking(REG_MULTI_LED_CTRL2,0x36))   // Choose value for ~ 25mA for Pilot LED
	    return false;
	  if(!maxim_max30102_write_reg_blocking(REG_PROX_INT_THRESH,0x20))   // Choose value for ~ 25mA for Pilot LED
		return false;
	  return true;

}


bool maxim_max30102_reset()

{
    if(!maxim_max30102_write_reg_blocking(REG_MODE_CONFIG,0x40))
        return false;
    else
        return true;
}

bool maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led)
{
  static uint32_t un_temp;
  *pun_red_led=0;
  *pun_ir_led=0;
  static uint8_t ach_i2c_data[6];

  if(i2cgetCompletition()==true){

	  maxim_max30102_read_sample(REG_FIFO_DATA, ach_i2c_data, 6);

	  un_temp=(unsigned char) ach_i2c_data[0];
	  un_temp<<=16;
	  *pun_red_led+=un_temp;
	  un_temp=(unsigned char) ach_i2c_data[1];
	  un_temp<<=8;
	  *pun_red_led+=un_temp;
	  un_temp=(unsigned char) ach_i2c_data[2];
	  *pun_red_led+=un_temp;

	  un_temp=(unsigned char) ach_i2c_data[3];
	  un_temp<<=16;
	  *pun_ir_led+=un_temp;
	  un_temp=(unsigned char) ach_i2c_data[4];
	  un_temp<<=8;
	  *pun_ir_led+=un_temp;
	  un_temp=(unsigned char) ach_i2c_data[5];
	  *pun_ir_led+=un_temp;
	  *pun_red_led&=0x03FFFF;  //Mask MSB [23:18]
	  *pun_ir_led&=0x03FFFF;  //Mask MSB [23:18]

	  return true;
  }

  return false;

}


bool maxim_max30102_read_sample(uint8_t uch_addr, uint8_t *puch_data, uint8_t size)

{
	i2cReadMsg(I2C_READ_ADDR, puch_data, size, uch_addr);

    return 0;
}






