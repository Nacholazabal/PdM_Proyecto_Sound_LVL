#include "port_ble.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include <string.h>

extern UART_HandleTypeDef huart1;

bool port_ble_init(void)
{
    // If you need to configure any pins manually you can do it here...
    // But HAL_Init and MX_USART1_UART_Init cover it.
    return true;
}

bool port_ble_sendString(const char *str)
{
    size_t len = strlen(str);
    return (HAL_UART_Transmit(&huart1,
                              (uint8_t*)str,
                              len,
                              HAL_MAX_DELAY) == HAL_OK);
}
