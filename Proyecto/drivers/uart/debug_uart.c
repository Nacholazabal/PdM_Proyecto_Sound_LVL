#include "debug_uart.h"
#include "port_debug_uart.h"
#include <string.h>

bool debug_uart_init(void) {
    return port_debug_uart_init();
}

bool debug_uart_print(const char* msg) {
    return debug_uart_send_raw((const uint8_t*)msg, strlen(msg));
}

bool debug_uart_send_raw(const uint8_t* data, size_t len) {
    return port_debug_uart_send(data, len);
}
