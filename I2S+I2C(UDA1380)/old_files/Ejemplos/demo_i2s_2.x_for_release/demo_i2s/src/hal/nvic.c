
#include "..\\include\\global.h"
#include "..\\include\\MKxxx.h"




void nvic_enable_irq(u8 irq_n)
{
    uint offset;

    offset = irq_n>>5;

    *(&NVICICPR0 + offset) |= 1<<(irq_n&0x1f);
    *(&NVICISER0 + offset) |= 1<<(irq_n&0x1f);
}






