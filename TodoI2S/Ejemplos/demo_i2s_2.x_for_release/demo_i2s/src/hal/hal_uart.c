
#include "..\\include\\global.h"
#include "..\\include\\MKxxx.h"
#include "..\\include\\hal_config.h"


static void _uart_init(UART_MemMapPtr uart_base_ptr, int sysclk_khz, int baud)
{
    u16 ubd;
    u8 temp;

    UART_C2_REG(uart_base_ptr) &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
    UART_C1_REG(uart_base_ptr) = 0;
    
    ubd = (u16)((sysclk_khz*1000)/(baud * 16));
    temp = UART_BDH_REG(uart_base_ptr) & ~(UART_BDH_SBR(0x1F));
    
    UART_BDH_REG(uart_base_ptr) = temp |  UART_BDH_SBR(((ubd & 0x1F00) >> 8));
    UART_BDL_REG(uart_base_ptr) = (u8)(ubd & UART_BDL_SBR_MASK);
    UART_C2_REG(uart_base_ptr) |= (UART_C2_TE_MASK | UART_C2_RE_MASK );
}

static bool _byte_arrive(UART_MemMapPtr uart_base_ptr)
{
    return UART_S1_REG(uart_base_ptr) & UART_S1_RDRF_MASK;
}
static char _get_byte(UART_MemMapPtr uart_base_ptr)
{
    while (!(UART_S1_REG(uart_base_ptr) & UART_S1_RDRF_MASK));
    return UART_D_REG(uart_base_ptr);
}
static void _send_byte(UART_MemMapPtr uart_base_ptr, char ch)
{
    while(!(UART_S1_REG(uart_base_ptr) & UART_S1_TDRE_MASK));
    UART_D_REG(uart_base_ptr) = (u8)ch;
}

// ----------------------------------------------------------------
#if BOARD == BOARD_K10
void hal_uart_init(void)
{
    // ser1
    SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
    PORTB_PCR16 = PORT_PCR_MUX(0x3);
    PORTB_PCR17 = PORT_PCR_MUX(0x3);
    _uart_init(UART0_BASE_PTR, SYS_CLK_KHZ, CONSOLE_BAUDRATE);
}
#define UART_CONSOLE UART0_BASE_PTR

#elif (BOARD == BOARD_K60)||(BOARD == BOARD_K50)||(BOARD == BOARD_K40)

void hal_uart_init(void)
{
    // ser1
    SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;
    PORTC_PCR16 = PORT_PCR_MUX(0x3);
    PORTC_PCR17 = PORT_PCR_MUX(0x3);
    _uart_init(UART3_BASE_PTR, BUS_CLK_KHZ, CONSOLE_BAUDRATE);
}
#define UART_CONSOLE UART3_BASE_PTR

#endif


void hal_uart_shell_send_byte(char ch)
{
    _send_byte(UART_CONSOLE, ch);
}
bool hal_uart_shell_byte_available(void)
{
    return _byte_arrive(UART_CONSOLE);
}
char hal_uart_shell_recv_byte(void)
{
    return _get_byte(UART_CONSOLE);
}


