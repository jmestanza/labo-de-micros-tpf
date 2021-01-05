

#include "..\\include\\global.h"
#include "..\\include\\MKxxx.h"
#include "..\\include\\hal_i2c_ex.h"
#include "..\\include\\hal_config.h"

#define I2C_ADDR_SGTL5000 (0x0a<<1)

#if (BOARD == BOARD_K60) || (BOARD == BOARD_K10)
    #define I2C_BASE I2C0_BASE_PTR
#endif
#if (BOARD == BOARD_K50) || (BOARD == BOARD_K40)
    #define I2C_BASE I2C1_BASE_PTR
#endif

// this delay is very important, it may cause w-r operation failure.
static void pause(void)
{
    int n;
    for(n=0; n<400; n++)
        asm("nop");
}
static u16 _read(u16 addr)
{
    char result[2];

    i2c_start(I2C_BASE);

    i2c_write_byte(I2C_BASE,I2C_ADDR_SGTL5000 | I2C_WRITE);
    i2c_wait(I2C_BASE);
    i2c_get_ack(I2C_BASE);

    i2c_write_byte(I2C_BASE, addr>>8);
    i2c_wait(I2C_BASE);
    i2c_get_ack(I2C_BASE);

    i2c_write_byte(I2C_BASE, addr&0xff);
    i2c_wait(I2C_BASE);
    i2c_get_ack(I2C_BASE);
    
    i2c_repeated_start(I2C_BASE);
    i2c_write_byte(I2C_BASE, I2C_ADDR_SGTL5000 | I2C_READ);
    i2c_wait(I2C_BASE);
    i2c_get_ack(I2C_BASE);

    i2c_set_rx_mode(I2C_BASE);

    i2c_give_ack(I2C_BASE);
    result[0] = i2c_read_byte(I2C_BASE);
    i2c_wait(I2C_BASE);

    i2c_give_nack(I2C_BASE);
    result[0] = i2c_read_byte(I2C_BASE);
    i2c_wait(I2C_BASE);

    i2c_stop(I2C_BASE);
    result[1] = i2c_read_byte(I2C_BASE);
    pause();

    return (result[0]<<8) | result[1];
}

static void _write(u16 addr,u16 data)
{
    i2c_start(I2C_BASE);

    i2c_write_byte(I2C_BASE, I2C_ADDR_SGTL5000|I2C_WRITE);
    i2c_wait(I2C_BASE);
    i2c_get_ack(I2C_BASE);

    i2c_write_byte(I2C_BASE, addr>>8);
    i2c_wait(I2C_BASE);
    i2c_get_ack(I2C_BASE);

    i2c_write_byte(I2C_BASE, addr&0xff);
    i2c_wait(I2C_BASE);
    i2c_get_ack(I2C_BASE);

    i2c_write_byte(I2C_BASE, data>>8);
    i2c_wait(I2C_BASE);
    i2c_get_ack(I2C_BASE);

    i2c_write_byte(I2C_BASE, data&0xff);
    i2c_wait(I2C_BASE);
    i2c_get_ack(I2C_BASE);

    i2c_stop(I2C_BASE);
    pause();
}

void hw_audio_sgtl5000_test_reg(void)
{
    printf("get sgtl5000 id: %x \n", _read(0x0000));
/*
    printf("get sgtl5000 id: %x \n", _read(0x0038));
    _write(0x0038,0x1234);
    printf("get sgtl5000 0x0038: %x \n", _read(0x0038));
// */
}




/* * Register values. */
#define SGTL5000_CHIP_ID                0x0000
#define SGTL5000_CHIP_DIG_POWER         0x0002
#define SGTL5000_CHIP_CLK_CTRL          0x0004
#define SGTL5000_CHIP_I2S_CTRL          0x0006
#define SGTL5000_CHIP_SSS_CTRL          0x000a
#define SGTL5000_CHIP_ADCDAC_CTRL       0x000e
#define SGTL5000_CHIP_DAC_VOL           0x0010
#define SGTL5000_CHIP_PAD_STRENGTH      0x0014
#define SGTL5000_CHIP_ANA_ADC_CTRL      0x0020
#define SGTL5000_CHIP_ANA_HP_CTRL       0x0022
#define SGTL5000_CHIP_ANA_CTRL          0x0024
#define SGTL5000_CHIP_LINREG_CTRL       0x0026
#define SGTL5000_CHIP_REF_CTRL          0x0028
#define SGTL5000_CHIP_MIC_CTRL          0x002a
#define SGTL5000_CHIP_LINE_OUT_CTRL     0x002c
#define SGTL5000_CHIP_LINE_OUT_VOL      0x002e
#define SGTL5000_CHIP_ANA_POWER         0x0030
#define SGTL5000_CHIP_PLL_CTRL          0x0032
#define SGTL5000_CHIP_CLK_TOP_CTRL      0x0034
#define SGTL5000_CHIP_ANA_STATUS        0x0036
#define SGTL5000_CHIP_SHORT_CTRL        0x003c
#define SGTL5000_CHIP_ANA_TEST2         0x003a
#define SGTL5000_CHIP_DAP_BASS_ENHANCE          0x0104
#define SGTL5000_CHIP_DAP_BASS_ENHANCE_CTRL     0x0106
    
#define SGTL5000_DAP_AUDIO_EQ                   0x0108
#define SGTL5000_DAP_AUDIO_EQ_BASS_BAND0        0x0116
#define SGTL5000_DAP_AUDIO_EQ_BASS_BAND1        0x0118
#define SGTL5000_DAP_AUDIO_EQ_BASS_BAND2        0x011A
#define SGTL5000_DAP_AUDIO_EQ_BASS_BAND3        0x011C
#define SGTL5000_DAP_AUDIO_EQ_BASS_BAND4        0x011E

#define SGTL5000_CHIP_DAP_AVC_CTRL              0x0124
#define SGTL5000_CHIP_DAP_MAIN_CHAN             0x0120
#define SGTL5000_CHIP_DAP_CTRL                  0x0100

void hw_audio_sgtl5000_init(void)
{
    //# Configuration for 1.8V, 1.8V, 1.8V VDDD, VDDA, VDDIO
    //# Set VDDD regulator
    _write(SGTL5000_CHIP_LINREG_CTRL, 0x000c);

    // Enable internal oscillator for charge pump
    _write(SGTL5000_CHIP_CLK_TOP_CTRL, 0x0800);

    //# Set VAG level, ref bias to -50%
    //_write(SGTL5000_CHIP_REF_CTRL, 0x004E);

    _write(SGTL5000_CHIP_REF_CTRL, 0x01EE);
        
    //# Set LINREG level to 1V
    _write(SGTL5000_CHIP_LINREG_CTRL, 0x000C);
    
    //# Set LINEOUT voltage/current
    _write(SGTL5000_CHIP_LINE_OUT_CTRL, 0x0F04);

    //# Set LO volume
    //_write(SGTL5000_CHIP_LINE_OUT_VOL, 0x0F0F);
    _write(SGTL5000_CHIP_LINE_OUT_VOL, 0x0000);


    //# Power up inputs/outputs  
    //# Power up CPUMP, Power up VAG, Turn off VDDD regulators (using ext supplies)
    //_write(SGTL5000_CHIP_ANA_POWER, 0x4AFF);
    // by gj, adc for mic set to mono mode
    _write(SGTL5000_CHIP_ANA_POWER, 0x4ABF);

    //# Power up digital blocks
    _write(SGTL5000_CHIP_DIG_POWER, 0x01FF);

    //# Set clocks for 48kHz, 256*Fs
    //_write(SGTL5000_CHIP_CLK_CTRL,0x0008);
    
    // 44.1 kHz
    // _write(SGTL5000_CHIP_CLK_CTRL,0x0004);
    // 32 k
    _write(SGTL5000_CHIP_CLK_CTRL,0x0000);

    //# Set Master, 24-bit I2S
    _write(SGTL5000_CHIP_I2S_CTRL, 0x0010);

    //# Set I/O configuration
    // adc in to i2s out (i.e. input to Kinetis tower card)
    // i2s in to dac (i.e.output from Kinetis tower card)

    // I2S_IN->DAC 
    //_write(SGTL5000_CHIP_SSS_CTRL, 0x0010);

    // I2S_IN->DAP->DAC->LINEOUT, HP
    _write(SGTL5000_CHIP_DAP_CTRL, 0x0001); // DAP_EN
    // i2s->dap->dac
    _write(SGTL5000_CHIP_SSS_CTRL, 0x0070); 
    // by gj, adc->dap->dac, ok
    //_write(SGTL5000_CHIP_SSS_CTRL, 0x0030); 
    // by gj adc-->dac
    //_write(SGTL5000_CHIP_SSS_CTRL, 0x0000); 
    // open micphone
    _write(SGTL5000_CHIP_MIC_CTRL, 2<<8);


    // i2s in-> dac
    //_write(SGTL5000_CHIP_SSS_CTRL, 0x0050);

    //# Set Unmute DAC, ADC
    //_write(SGTL5000_CHIP_ADCDAC_CTRL, 0x0230);
    _write(SGTL5000_CHIP_ADCDAC_CTRL, 0x0000);        
    _write(SGTL5000_CHIP_DAC_VOL, 0x0000);

    _write(SGTL5000_CHIP_LINE_OUT_VOL, 0x0F0F); // Lineout Volume Adjustment

    // Set Line Out Analog Ground to 1.55V
    _write(SGTL5000_CHIP_LINE_OUT_CTRL, 0x0F1E); 

    //For Input from Line IN
    //_write(SGTL5000_CHIP_ANA_CTRL ,0x0004);

    // dac->hp, micphone->adc, hp unmute, adc unmute
    _write(SGTL5000_CHIP_ANA_CTRL ,0x0000);
    _write(SGTL5000_CHIP_DAP_MAIN_CHAN,0xFFFF);

    // Turn ON AVC - Very Important for gain level
    _write(SGTL5000_CHIP_DAP_AVC_CTRL,0x7101); // Try AVC enable with max. GAIN
    _write(SGTL5000_CHIP_DAP_BASS_ENHANCE,0x0021); // Enable Bass Boost
    _write(SGTL5000_CHIP_DAP_BASS_ENHANCE_CTRL,0x0000);
}

#define SIN_TEST
/*
    about how to show sinewave

    sample rate is set to 32k
*/
#ifdef SIN_TEST
#include "math.h"
#endif
void hw_audio_sgtl5000_play(s32 *sin_table, s32 *sin_table2, uint buf_size)
{
    int i = 0;

    while(1)
    {
        // clear flag
        if(I2S0_TCSR&(1<<18))
            I2S0_TCSR |= (1<<18);
            
        //continue;
        if( (I2S0_TCSR&(1<<16)) != 0)
        {
            #ifdef SIN_TEST
                I2S0_TDR0 = sin_table[i];
                I2S0_TDR0 = sin_table2[i];
                i++;
                if(i >= buf_size)
                    i = 0;
            #else
                I2S0_TX0 = I2S0_RX0; // &0xffff00;
                I2S0_TX0 = I2S0_RX0; // &0xffff00;
            #endif
        }
// */
    }
}



