/**
 * @file debug_uart.h
 * @brief Módulo de impresión por UART para depuración
 *
 * Proporciona funciones para enviar mensajes de texto o datos binarios
 * por una UART utilizada exclusivamente con fines de depuración.
 */

#ifndef DEBUG_UART_H
#define DEBUG_UART_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
typedef bool bool_t;
/**
 * @brief Inicializa el puerto UART utilizado para debug.
 *
 * Llama a la capa port específica que configura la UART.
 *
 * @return true si la UART se inicializó correctamente.
 */
bool_t debug_uart_init(void);

/**
 * @brief Envía un mensaje de texto por la UART de debug.
 *
 * @param msg Puntero al string nulo-terminado.
 * @return true si el mensaje fue enviado exitosamente.
 */
bool_t debug_uart_print(const char* msg);

/**
 * @brief Envía datos binarios por la UART de debug.
 *
 * @param data Puntero al buffer de datos.
 * @param len  Cantidad de bytes a enviar.
 * @return true si los datos fueron enviados exitosamente.
 */
bool_t debug_uart_send_raw(const uint8_t* data, size_t len);

#endif // DEBUG_UART_H
