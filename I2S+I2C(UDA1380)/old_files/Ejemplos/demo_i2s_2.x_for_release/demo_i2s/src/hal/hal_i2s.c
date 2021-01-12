

#include "..\\include\\global.h"
#include "..\\include\\MKxxx.h"
#include "..\\include\\hal_config.h"


static void _set_clock_112896(void)
{
    SIM_SCGC6 &= ~(SIM_SCGC6_I2S_MASK);

    // output = input¡Á[(I2SFRAC+1) / (I2SDIV+1) ] = (48* (4/17))
    // SIM_CLKDIV2 |= SIM_CLKDIV2_I2SDIV(16) | SIM_CLKDIV2_I2SFRAC(3);
    I2S0_MDR = I2S_MDR_FRACT(3) | I2S_MDR_DIVIDE(16);
    SIM_SCGC6 |= SIM_SCGC6_I2S_MASK;
}
static void _set_clock_122800(void)
{
    // output = input ¡Á[(I2SFRAC+1) / (I2SDIV+1) ] = (48M* (32/125))
    // SIM_CLKDIV2 |= SIM_CLKDIV2_I2SDIV(124) | SIM_CLKDIV2_I2SFRAC(31);
    I2S0_MDR = I2S_MDR_FRACT(31) | I2S_MDR_DIVIDE(124);
}
static void _i2s_init(void)
{
    #define I2S_CONFIG_WORDS_IN_A_FRAME 2
    #define I2S_CONFIG_BITS_IN_A_WORD   24

    I2S0_TCR1 = 4;// 6;    // water mark
    I2S0_TCR2 |= (0<<30) | // master mode(Async mode)
                 (1<<26) | // MSEL = MCLK
                 (1<<25) | // CLK = drive on falling edge
                 (1<<24) ; // CLK = OUTPUT

    I2S0_TCR3 = (1<<16); // enable channel 0

    I2S0_TCR4 = ((I2S_CONFIG_WORDS_IN_A_FRAME-1)<<16)  | // words in a frame
                ((I2S_CONFIG_BITS_IN_A_WORD  -1)<<8)   | // bits in a word
                 (1<<4)                                | // MSB
                 (1<<3)                                | // one bit early
                 (1<<1)                                | // frame active low
                 (1<<0)                                ; // frame = output

    I2S0_TCR5 = ((I2S_CONFIG_BITS_IN_A_WORD-1) <<24) | // word N width
                ((I2S_CONFIG_BITS_IN_A_WORD-1) <<16) | // word 0 width
                 (0x17<<8);                            // right adjust, where the first bit starts

    I2S0_TMR = 0;

    // enable TX
    I2S0_TCSR = (0<<31) | // enable tx
                (1<<28) | // enable bit clock
                (1<<0);   // enable DMA request
}

static void _i2s_set_rate(int smprate)
{
    unsigned char div;
    SIM_SCGC6 |= SIM_SCGC6_I2S_MASK;

    // Select MCLK input source
    I2S0_MCR = (1<<30)| // MCLK = output
               (0<<24); // MCLK SRC = core clock = 48M

    if((smprate == 11025)||(smprate == 22050)||(smprate == 44100))
        _set_clock_112896();

    if((smprate == 8000) || (smprate == 12000) || (smprate == 16000) ||
       (smprate == 24000)|| (smprate == 32000) || (smprate == 48000) )
        _set_clock_122800();

    switch(smprate)
    {
        case 32000: div=3;  break; // 12.288M/(32K*48) = 8, 8 = (DIV+1)*2, DIV = 3
    }   

    I2S0_TCR2 = div;
}

static void _i2s_io_init(void)
{
    #if (BOARD == BOARD_K60) || (BOARD == BOARD_K50)
    PORTE_PCR6  &= PORT_PCR_MUX_MASK;
    PORTE_PCR7  &= PORT_PCR_MUX_MASK;
    PORTE_PCR10 &= PORT_PCR_MUX_MASK;
    PORTE_PCR11 &= PORT_PCR_MUX_MASK;
    PORTE_PCR12 &= PORT_PCR_MUX_MASK;

    PORTE_PCR6  |= PORT_PCR_MUX(0x04);
    PORTE_PCR7  |= PORT_PCR_MUX(0x04);
    PORTE_PCR10 |= PORT_PCR_MUX(0x04);
    PORTE_PCR11 |= PORT_PCR_MUX(0x04);
    PORTE_PCR12 |= PORT_PCR_MUX(0x04);
    #endif

    #if BOARD == BOARD_K40
    PORTA_PCR12 &= PORT_PCR_MUX_MASK;
    PORTA_PCR13 &= PORT_PCR_MUX_MASK;
    PORTA_PCR14 &= PORT_PCR_MUX_MASK;
    PORTA_PCR15 &= PORT_PCR_MUX_MASK;
    PORTA_PCR16 &= PORT_PCR_MUX_MASK;
    PORTA_PCR5  &= PORT_PCR_MUX_MASK;
    PORTA_PCR17 &= PORT_PCR_MUX_MASK;

    PORTA_PCR12 |= PORT_PCR_MUX(0x06); // tx
    PORTA_PCR13 |= PORT_PCR_MUX(0x06); // tx fs
    PORTA_PCR14 |= PORT_PCR_MUX(0x06); // tx clock
    PORTA_PCR15 |= PORT_PCR_MUX(0x06); // rx
    PORTA_PCR16 |= PORT_PCR_MUX(0x06); // rx fs
    PORTA_PCR5  |= PORT_PCR_MUX(0x06); // rx clock
    PORTA_PCR17 |= PORT_PCR_MUX(0x06); // mclk
    #endif

    #if BOARD == BOARD_K10
    PORTB_PCR18 |= PORT_PCR_MUX(0x06);
    PORTB_PCR19 |= PORT_PCR_MUX(0x06);
    PORTC_PCR0  |= PORT_PCR_MUX(0x06);
    PORTC_PCR8  |= PORT_PCR_MUX(0x06);
    PORTC_PCR11 |= PORT_PCR_MUX(0x06);
    #endif
}

void hal_i2s_init(void)
{    
    _i2s_io_init();
    _i2s_set_rate(32000);
    _i2s_init();
}

void hal_i2s_enable(void)
{
    I2S0_TCSR |= 1u<<31;
}


