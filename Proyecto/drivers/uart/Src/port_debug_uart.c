/**
 * @file port_debug_uart.c
 * @brief Implementación de la capa de abstracción de UART de depuración.
 *
 * Utiliza funciones HAL para enviar datos mediante el periférico UART definido.
 */

// === Includes ===
#include <stddef.h>
#include <stdint.h>

#include "main.h"
#include "port_debug_uart.h"
#include "stm32f4xx_hal.h"

// Handler definido en main.c
extern UART_HandleTypeDef huart2;

/**
 * Inicializa el periférico UART2 para debug.
 *
 * La configuración de baudrate, pines y DMA debe hacerse con STM32CubeMX.
 */
bool_t port_debug_uart_init(void) {
    return HAL_UART_Init(&huart2) == HAL_OK;
}

/**
 * Envía datos por UART2 con timeout bloqueante.
 *
 * Esta función es bloqueante: espera hasta que se transmitan todos los datos.
 */
bool_t port_debug_uart_send(const uint8_t* data, size_t len) {
    return HAL_UART_Transmit(&huart2, (uint8_t*)data, len, HAL_MAX_DELAY) == HAL_OK;
}
