



#include "..\\include\\global.h"
#include "..\\include\\MKxxx.h"
#include "..\\include\\hal_uart.h"
#include "..\\include\\hal_config.h"
#include "..\\include\\nvic.h"

#define DMA_SIZE_8_BIT  0
#define DMA_SIZE_16_BIT 1
#define DMA_SIZE_32_BIT 2

#define DMA_MUX_SRC_ADC0 40
#define DMA_MUX_SRC_ADC1 41

#define SRAM_SADDRESS    0x1FFFE000
#define SRAM_DADDRESS    0x1FFFF000

void dma_sw_trig(int channel)
{
    static int i = 0;
    DMA_SSRT = channel;

    printf("dma_sw_trig %d\n",i++);
}
void sw_delay(void)
{
    int i;
    for(i=0;i<100;i++);
}
int dma_wait(int channel)
{
    int r = 0;
    while((( DMA_CSR(channel) & DMA_CSR_DONE_MASK) == 0) && ( DMA_ES == 0));
            
    if (DMA_ES)
        r = DMA_ES;
    return r;
}

#if TEST_ENABLE
void mem_src_init(void)
{
    char *p;
    int i = 0;

    p = (char*)SRAM_SADDRESS;
    for(i=0;i<128;i++)
        *p++ = i;


    p = (char*)SRAM_DADDRESS;
    for(i=0;i<128;i++)
        *p++ = 0;
}

void dma_sw_trgger_test (void)
{
    int channel = 0;

    SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

    DMA_SADDR(channel) = SRAM_SADDRESS;
    DMA_DADDR(channel) = SRAM_DADDRESS;

    // 每次传送4个字节 无内层循环
    DMA_NBYTES_MLNO(channel) = 4;
    DMA_ATTR(channel) = DMA_ATTR_SSIZE(DMA_SIZE_16_BIT) | DMA_ATTR_DSIZE(DMA_SIZE_16_BIT);

    // 源、目地址每次操作后的增量，这是个有符号数，可以递增，也可以递减
    DMA_SOFF(channel) = 2;
    DMA_DOFF(channel) = 2;

    // 主循环完成后的地址增量
    DMA_SLAST(channel)     =  0;
    DMA_DLAST_SGA(channel) = (uint)(-8);

    // CITER的值会在运行中进行递减，耗尽后，会重新装载BITER的值
    DMA_CITER_ELINKNO(channel) = 2;
    DMA_BITER_ELINKNO(channel) = 2;


    nvic_enable_irq(IRQ_DMA0);
    // major耗尽后产生中断
    DMA_CSR(channel) = DMA_CSR_INTMAJOR_MASK;    
    // enable interrupt on this channel
    DMA_SERQ = DMA_SERQ_SERQ(channel);

    mem_src_init();
    printf("mem_src_init\n");
    print_buf((char*)SRAM_SADDRESS, 16);
    print_buf((char*)SRAM_DADDRESS, 16);

    dma_sw_trig(channel);
    dma_sw_trig(channel);
    dma_wait(channel);

    printf("trig 1\n");
    print_buf((char*)SRAM_SADDRESS, 16);
    print_buf((char*)SRAM_DADDRESS, 16);

    dma_sw_trig(channel);
    dma_sw_trig(channel);
    dma_wait(channel);
    printf("trig 2\n");
    print_buf((char*)SRAM_SADDRESS, 16);
    print_buf((char*)SRAM_DADDRESS, 16);

    dma_sw_trig(channel);
    dma_sw_trig(channel);
    dma_wait(channel);
    printf("trig 3\n");
    print_buf((char*)SRAM_SADDRESS, 16);
    print_buf((char*)SRAM_DADDRESS, 16);
}
#endif

#if BOARD == BOARD_K10
static void _init_for_adc1 (void)
{
    int channel = 0;

    SIM_SCGC6     |= SIM_SCGC6_DMAMUX_MASK;
    DMAMUX_CHCFG0  = DMAMUX_CHCFG_ENBL_MASK | 
                     DMAMUX_CHCFG_SOURCE(DMA_MUX_SRC_ADC1);
    nvic_enable_irq(IRQ_DMA0);

    DMA_SADDR(channel) = (u32)(&(ADC1_RA));
    DMA_DADDR(channel) = SRAM_DADDRESS;

    // 每次传送4个字节 无内层循环
    DMA_NBYTES_MLNO(channel) = 4;
    DMA_ATTR(channel) = DMA_ATTR_SSIZE(DMA_SIZE_32_BIT) | DMA_ATTR_DSIZE(DMA_SIZE_32_BIT);

    // 源、目地址每次操作后的增量，这是个有符号数，可以递增，也可以递减
    // 这里的设置要和DMA_ATTR匹配
    DMA_SOFF(channel) = 0;
    DMA_DOFF(channel) = 4;

    // 主循环完成后的地址增量
    DMA_SLAST(channel)     =  0;
    DMA_DLAST_SGA(channel) = (uint)(-8);

    // CITER的值会在运行中进行递减，耗尽后，会重新装载BITER的值
    DMA_CITER_ELINKNO(channel) = 2;
    DMA_BITER_ELINKNO(channel) = 2;

    // major耗尽后产生中断
    DMA_CSR(channel) = DMA_CSR_INTMAJOR_MASK;

    // enable dma
    DMA_SERQ = DMA_SERQ_SERQ(channel);
}
#endif

#if (BOARD == BOARD_K50) || (BOARD == BOARD_K60)


typedef struct {
    int channel;
	u32 saddr,daddr;     // initial address
	s16 soff,doff;       // offset after each read/write
	u16 attr;	         // modulo and port size
	u32 nbytes;          // each minor transmission count
	s32 slast,dlast_sga; // offset after major loop end
	u16 citer,biter;	 // major loop, CITER decreases, then reloaded from BITER	
}DMA_TCD;
static void _dma_init(DMA_TCD *tcd)
{
    DMA_SADDR(tcd->channel)           = tcd->saddr;
    DMA_DADDR(tcd->channel)           = tcd->daddr;
    DMA_NBYTES_MLOFFYES(tcd->channel) = tcd->nbytes;
    DMA_ATTR(tcd->channel)            = tcd->attr;
    DMA_SOFF(tcd->channel)            = tcd->soff;
    DMA_DOFF(tcd->channel)            = tcd->doff;
    DMA_SLAST(tcd->channel)           = tcd->slast;
    DMA_DLAST_SGA(tcd->channel)       = tcd->dlast_sga;
    DMA_CITER_ELINKNO(tcd->channel)   = tcd->citer;
    DMA_BITER_ELINKNO(tcd->channel)   = tcd->biter;

    printf("%x,%x,%x,%x,\n%d,%d,%d,%d,%d,%d\n",
    tcd->saddr,tcd->daddr,tcd->nbytes,tcd->attr,
    tcd->soff,tcd->doff,tcd->slast,tcd->dlast_sga,
    tcd->citer,tcd->biter);
}
#endif

void hal_dma_init(void)
{
#if BOARD == BOARD_K10
    _init_for_adc1();
#endif

#if (BOARD == BOARD_K50) || (BOARD == BOARD_K60)
/*
    _init_for_i2s(0, (uint)buf_i2s_r_rx, (uint)(&I2S0_RX0), false);
    _init_for_i2s(1, (uint)buf_i2s_l_rx, (uint)(&I2S0_RX1), false);
    _init_for_i2s(2, (uint)buf_i2s_r_tx, (uint)(&I2S0_TX0), true);
    _init_for_i2s(3, (uint)buf_i2s_l_tx, (uint)(&I2S0_TX1), true);
*/
    // when major loop end and half end, generate interrupt
    DMA_CSR(0) = DMA_CSR_INTHALF_MASK | DMA_CSR_INTMAJOR_MASK;
    nvic_enable_irq(IRQ_DMA0);
#endif
}

#define DMA_SRC_I2S_R 14
#define DMA_SRC_I2S_T 15
void hal_dma_init_for_i2s(uint buf_rx, uint buf_tx, uint block_n_sample, uint sample_n_byte)
{
    uint size_bit;
    DMA_TCD tcd;

    switch(sample_n_byte)
    {
        case 1:  size_bit = DMA_SIZE_8_BIT;  break;
        case 2:  size_bit = DMA_SIZE_16_BIT; break;
        case 4:  size_bit = DMA_SIZE_32_BIT; break;
        default: size_bit = DMA_SIZE_32_BIT; break;
    }

    SIM_SCGC6    |= SIM_SCGC6_DMAMUX_MASK;
    DMAMUX_CHCFG0 = DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(DMA_SRC_I2S_T);

    DMA_CR       |= DMA_CR_EMLM_MASK;
    DMA_CSR(0)    = DMA_CSR_INTHALF_MASK | DMA_CSR_INTMAJOR_MASK;
    nvic_enable_irq(IRQ_DMA0);

    tcd.channel   = 0;
    tcd.nbytes    = DMA_NBYTES_MLOFFYES_SMLOE_MASK | 
                    DMA_NBYTES_MLOFFYES_MLOFF(sample_n_byte - block_n_sample*2*sample_n_byte*2) | 
                    DMA_NBYTES_MLOFFYES_NBYTES(sample_n_byte*2);
    tcd.attr      = DMA_ATTR_SSIZE(size_bit) | DMA_ATTR_DSIZE(size_bit);

    tcd.saddr     = buf_tx;
    tcd.soff      = block_n_sample*2*sample_n_byte;
    tcd.slast     = -(block_n_sample*2*sample_n_byte*3 - sample_n_byte);

    tcd.daddr     = (uint)(&I2S0_TDR0);
    tcd.doff      = 0;
    tcd.dlast_sga = 0;
    
    tcd.citer     = block_n_sample*2;
    tcd.biter     = block_n_sample*2;
    _dma_init(&tcd);

    // enable DMA channel
    DMA_SERQ = DMA_SERQ_SERQ(0); 
}
void hal_dma_clear_int(uint channel)
{
    channel &= 0xf;
    DMA_CINT = channel;
}
void check_dma_buffer(void)
{
    while(1)
    {
        while(!(DMA_TCD0_CSR & DMA_CSR_DONE_MASK))
            ;
        DMA_TCD0_CSR &= ~DMA_CSR_DONE_MASK;
        print_buf((char*)SRAM_DADDRESS, 16);
    }
}



