/**
 * @file port_bt.c
 * @brief Implementación de la capa de abstracción para el módulo Bluetooth.
 *
 * Utiliza HAL UART para comunicarse con el hardware Bluetooth.
 */

#include "port_bt.h"
#include "stm32f4xx_hal.h"
#include "main.h"

// Handler UART definido en el proyecto principal (main.c o usart.c)
extern UART_HandleTypeDef huart1;

/**
 * Inicializa el periférico UART utilizado para Bluetooth.
 *
 * @return true si HAL_UART_Init devuelve HAL_OK
 */
bool port_bt_init(void) {
    return HAL_UART_Init(&huart1) == HAL_OK;
}

/**
 * Envía datos por UART hacia el módulo Bluetooth.
 *
 * @param data Buffer con los datos a enviar.
 * @param len Cantidad de bytes del buffer.
 * @return true si la transmisión fue exitosa.
 */
bool port_bt_send(const uint8_t* data, size_t len) {
    return HAL_UART_Transmit(&huart1, (uint8_t*)data, len, HAL_MAX_DELAY) == HAL_OK;
}
