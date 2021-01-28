

#include "..\\include\\global.h" 
#include "..\\include\\hal_uart.h"
#include <stdarg.h>

#ifdef printf
#undef printf
#endif
#include <stdio.h>


static char buf[512];

void send_string(char *p_str)
{
    for(;*p_str!=NULL;p_str++)
    {
        hal_uart_shell_send_byte(*p_str);
        if(*p_str == '\n')
            hal_uart_shell_send_byte('\r');
    }
}
void q_printf(char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    vsprintf(buf, fmt, arg);
    va_end(arg);

    send_string(buf);
}
void print_buf(void *buf, uint size)
{
    int i;
    byte *p = (byte*)buf;
    for(i=0;i<size;i++)
    {
        q_printf("0x%.2x,", p[i]);
        if(((i+1)&0x3) == 0)
            q_printf("  ");
        if(((i+1)&0x7) == 0)
            q_printf("\n");
    }

    q_printf("\n");
}


