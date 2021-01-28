

#include "..\\include\\global.h"
#include "..\\include\\hal_i2c.h"
#include "..\\include\\hal_i2s.h"
#include "..\\include\\hal_audio.h"
#include "..\\include\\MKxxx.h"
#include "math.h"

#define BUF_N_SAMPLES 160
static s32 sin_table[BUF_N_SAMPLES];
static s32 sin_table2[BUF_N_SAMPLES];

void sin_table_init(void)
{
    int i;
    
    for(i=0;i<BUF_N_SAMPLES;i++)
    {
        sin_table[i]  = (s32)(6000*sin(2*3.1415926*1000*((float)i/(float)32000)));
        sin_table2[i] = (s32)(6000*sin(2*3.1415926*200*((float)i/(float)32000)));
    }
}
void app_audio(void)
{
    sin_table_init();
    hal_i2c_init();
    hal_i2s_init();
    hal_audio_init();
    hal_i2s_enable();
    hal_audio_play(sin_table, sin_table2, BUF_N_SAMPLES);
}

void app_isr_dma0(void)
{
    hal_fill_tx_buf(sin_table, sin_table2, BUF_N_SAMPLES);
}

