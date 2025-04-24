/**
 * @file debug_uart.c
 * @brief Implementación del módulo de depuración por UART.
 *
 * Encapsula la inicialización y el envío de datos a través de una UART
 * configurada para propósitos de logging o debug.
 */

#include <string.h>

#include "debug_uart.h"
#include "port_debug_uart.h"

/**
 * Inicializa la UART usada para debug.
 *
 * La configuración específica del hardware se hace en port_debug_uart.
 */
bool_t debug_uart_init(void) {
    return port_debug_uart_init();
}

/**
 * Envía un string de texto a través de la UART de debug.
 *
 * Internamente convierte el string a un buffer de bytes y llama a la función binaria.
 */
bool_t debug_uart_print(const char* msg) {
    return debug_uart_send_raw((const uint8_t*)msg, strlen(msg));
}

/**
 * Envía datos crudos (bytes) a través de la UART de debug.
 */
bool_t debug_uart_send_raw(const uint8_t* data, size_t len) {
    return port_debug_uart_send(data, len);
}
