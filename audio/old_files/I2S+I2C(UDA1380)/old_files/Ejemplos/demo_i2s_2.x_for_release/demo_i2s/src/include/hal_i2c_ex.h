#ifndef __HAL_I2C_EX__
#define __HAL_I2C_EX__

void i2c_set_tx_mode(I2C_MemMapPtr p);
void i2c_set_rx_mode(I2C_MemMapPtr p);
void i2c_set_slave_mode(I2C_MemMapPtr p);
void i2c_set_master_mode(I2C_MemMapPtr p);
void i2c_give_nack(I2C_MemMapPtr p);
void i2c_give_ack(I2C_MemMapPtr p);
void i2c_repeated_start(I2C_MemMapPtr p);
void i2c_write_byte(I2C_MemMapPtr p, byte data);
byte i2c_read_byte(I2C_MemMapPtr p);
void i2c_start(I2C_MemMapPtr p);
void i2c_stop(I2C_MemMapPtr p);
bool i2c_wait(I2C_MemMapPtr p);
bool i2c_get_ack(I2C_MemMapPtr p);
void hal_i2c_init(void);

#define I2C_READ  1
#define I2C_WRITE 0


#endif

