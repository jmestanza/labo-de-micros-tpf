

#ifndef __HAL_UART__
#define __HAL_UART__


void hal_uart_init(void);
void hal_uart_shell_send_byte(char ch);
char hal_uart_shell_recv_byte(void);
bool hal_uart_shell_byte_available(void);

#endif

