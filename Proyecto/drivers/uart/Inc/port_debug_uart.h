/**
 * @file port_debug_uart.h
 * @brief Capa de abstracción para UART de depuración.
 *
 * Este módulo permite inicializar y enviar datos a través de una UART
 * destinada exclusivamente para mensajes de debug.
 */

#ifndef PORT_DEBUG_UART_H
#define PORT_DEBUG_UART_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
typedef bool bool_t;
/**
 * @brief Inicializa el periférico UART utilizado para debug.
 *
 * Esta función depende de la configuración realizada por STM32CubeMX
 * y asume que el handler correspondiente ya está inicializado.
 *
 * @return true si la UART se inicializó correctamente.
 */
bool_t port_debug_uart_init(void);

/**
 * @brief Envía datos binarios por la UART de debug.
 *
 * @param data Puntero al buffer de datos.
 * @param len  Longitud del buffer en bytes.
 * @return true si los datos fueron enviados correctamente.
 */
bool_t port_debug_uart_send(const uint8_t* data, size_t len);

#endif // PORT_DEBUG_UART_H
