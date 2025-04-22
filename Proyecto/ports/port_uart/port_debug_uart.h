#ifndef PORT_DEBUG_UART_H
#define PORT_DEBUG_UART_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

bool port_debug_uart_init(void);
bool port_debug_uart_send(const uint8_t* data, size_t len);

#endif // PORT_UART_DEBUG_H
