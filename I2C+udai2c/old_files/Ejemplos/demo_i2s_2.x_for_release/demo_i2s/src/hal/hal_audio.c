


#include "..\\include\\global.h"
#include "..\\include\\hw_audio_sgtl5000.h"
#include "..\\include\\hal_dma.h"

#define I2S_BLOCK_N_SAMPLES  64 // each block has N samples
//#define I2S_BLOCK_N_SAMPLES  8 // each block has N samples
#define I2S_SAMPLE_N_BYTE 4    // each sample has N bytes

typedef struct 
{
    byte buf_i2s_r_rx[I2S_BLOCK_N_SAMPLES*I2S_SAMPLE_N_BYTE*2];
    byte buf_i2s_l_rx[I2S_BLOCK_N_SAMPLES*I2S_SAMPLE_N_BYTE*2];
    byte buf_i2s_r_tx[I2S_BLOCK_N_SAMPLES*I2S_SAMPLE_N_BYTE*2];
    byte buf_i2s_l_tx[I2S_BLOCK_N_SAMPLES*I2S_SAMPLE_N_BYTE*2];
}I2S_BUF;

static I2S_BUF i2s_buf;


void hal_fill_tx_buf(s32 *p_r, s32 *p_l, uint buf_n_sample)
{
    static int index = 0;
    static int data_index = 0;
    int i;
    s32 *p_r_tx;
    s32 *p_l_tx;
    
    if(index == 0)
    {
        p_r_tx = (int*)i2s_buf.buf_i2s_r_tx;
        p_l_tx = (int*)i2s_buf.buf_i2s_l_tx;
    }
    else
    {
        p_r_tx = (int*)(i2s_buf.buf_i2s_r_tx+I2S_BLOCK_N_SAMPLES*I2S_SAMPLE_N_BYTE);
        p_l_tx = (int*)(i2s_buf.buf_i2s_l_tx+I2S_BLOCK_N_SAMPLES*I2S_SAMPLE_N_BYTE);
    }
    
    for(i=0;i<I2S_BLOCK_N_SAMPLES;i++)
    {
        *p_r_tx++ = p_r[data_index];
        *p_l_tx++ = p_l[data_index];

        data_index++;

        if(data_index >= buf_n_sample)
            data_index = 0;
    }

    index ^= 1;
}

void hal_audio_init(void)
{
    hw_audio_sgtl5000_init();
    hw_audio_sgtl5000_test_reg();
    hal_dma_init_for_i2s((uint)(i2s_buf.buf_i2s_r_rx), (uint)(i2s_buf.buf_i2s_r_tx), I2S_BLOCK_N_SAMPLES, I2S_SAMPLE_N_BYTE);    
}

void hal_audio_play(s32 *sin_table, s32 *sin_table2, uint buf_size)
{
    while(1)
    {
        ;
    }
}


