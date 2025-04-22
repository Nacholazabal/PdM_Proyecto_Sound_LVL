// debug_uart.h
#ifndef DEBUG_UART_H
#define DEBUG_UART_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

bool debug_uart_init(void);
bool debug_uart_print(const char* msg);
bool debug_uart_send_raw(const uint8_t* data, size_t len);

#endif // DEBUG_UART_H
