

#ifndef __HAL_DMA__
#define __HAL_DMA__


void dma_sw_trgger_test(void);
void hal_dma_init(void);
void hal_dma_init_for_i2s(uint buf_rx, uint buf_tx, uint block_n_sample, uint sample_n_byte);
void hal_dma_channel_init(void);
void hal_dma_clear_int(uint channel);

#endif


