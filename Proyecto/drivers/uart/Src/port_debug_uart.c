#include "port_debug_uart.h"
#include "stm32f4xx_hal.h"
#include "main.h"

extern UART_HandleTypeDef huart2;

bool port_debug_uart_init(void) {
    return HAL_UART_Init(&huart2) == HAL_OK;
}

bool port_debug_uart_send(const uint8_t* data, size_t len) {
    return HAL_UART_Transmit(&huart2, (uint8_t*)data, len, HAL_MAX_DELAY) == HAL_OK;
}
