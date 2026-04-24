#ifndef UART_H
#define UART_H

void uart_init(void);
void uart_print(const char *s);
void uart2_write_char(char c) ;
void uart2_write_str(const char *s) ;
void uart2_write_uint(unsigned int v) ;
#endif
