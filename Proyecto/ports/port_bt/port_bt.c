#include "port_bt.h"
#include "stm32f4xx_hal.h"
#include "main.h"

extern UART_HandleTypeDef huart1;  // Asegurate que está definido en tu main.c o en usart.c

bool port_bt_init(void) {
    return HAL_UART_Init(&huart1) == HAL_OK;
}

bool port_bt_send(const uint8_t* data, size_t len) {
    return HAL_UART_Transmit(&huart1, (uint8_t*)data, len, HAL_MAX_DELAY) == HAL_OK;
}
