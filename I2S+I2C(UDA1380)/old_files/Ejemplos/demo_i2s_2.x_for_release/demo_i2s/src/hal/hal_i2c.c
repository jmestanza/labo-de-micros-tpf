
#include "..\\include\\global.h"
#include "..\\include\\MKxxx.h"
#include "..\\include\\hal_config.h"



void i2c_set_tx_mode(I2C_MemMapPtr p)
{
    p->C1 |= I2C_C1_TX_MASK;

}
void i2c_set_rx_mode(I2C_MemMapPtr p)
{
    p->C1 &= ~I2C_C1_TX_MASK;
}

void i2c_set_slave_mode(I2C_MemMapPtr p)
{
    p->C1  &= ~I2C_C1_MST_MASK;
}
void i2c_set_master_mode(I2C_MemMapPtr p)
{
    p->C1  |=  I2C_C1_MST_MASK;
}

// i2c general

void i2c_give_nack(I2C_MemMapPtr p)
{
    p->C1 |= I2C_C1_TXAK_MASK;
}
void i2c_give_ack(I2C_MemMapPtr p)
{
    p->C1 &= ~I2C_C1_TXAK_MASK;
}
void i2c_repeated_start(I2C_MemMapPtr p)
{
    p->C1     |= 0x04;
}
void i2c_write_byte(I2C_MemMapPtr p, byte data)
{
    p->D = data;
}
byte i2c_read_byte(I2C_MemMapPtr p)
{
    return p->D;
}
void i2c_start(I2C_MemMapPtr p)
{
    i2c_set_master_mode(p);
    i2c_set_tx_mode(p);
}
void i2c_stop(I2C_MemMapPtr p)
{
    i2c_set_slave_mode(p);
    i2c_set_rx_mode(p);
}
void i2c_wait(I2C_MemMapPtr p)
{
    // wait flag
    while((p->S & I2C_S_IICIF_MASK)==0)
        ;
    // clear flag
    p->S |= I2C_S_IICIF_MASK;
}
bool i2c_get_ack(I2C_MemMapPtr p)
{
    if((p->S & I2C_S_RXAK_MASK) == 0)
        return true;
    else
        return false;
}

// -------------------------------------------------

void hal_i2c_init(void)
{
#if (BOARD == BOARD_K10)
    SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;
    PORTB_PCR2 = PORT_PCR_MUX(2);
    PORTB_PCR3 = PORT_PCR_MUX(2);

    I2C0_F  = 0x14; // baudrate
    I2C0_C1 = 0x80; // enable IIC
#endif

#if (BOARD == BOARD_K40) ||(BOARD == BOARD_K50)
    SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;
    PORTC_PCR10 = PORT_PCR_MUX(2);
    PORTC_PCR11 = PORT_PCR_MUX(2);

    I2C1_F  = 0x33;
    I2C1_C1 = 0x80;
#endif

#if BOARD == BOARD_K60
    SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;
    PORTD_PCR8 = PORT_PCR_MUX(2);
    PORTD_PCR9 = PORT_PCR_MUX(2);

    I2C0_F  = 0x60;
    I2C0_C1 = 0x80;
#endif
}



