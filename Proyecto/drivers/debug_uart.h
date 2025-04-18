/**
 * @file debug_uart.h
 * @brief Debug UART interface.
 *
 * This module provides a simple interface for sending debug messages over UART.
 * It wraps the HAL_UART_Transmit function.
 */

#ifndef DEBUG_UART_H
#define DEBUG_UART_H

#include <stdint.h>

/**
 * @brief Initializes the UART debug module.
 *
 * Typically, initialization of UART2 is handled by CubeMX.
 * This function is provided for consistency in the driver structure.
 */
void debug_uart_init(void);

/**
 * @brief Sends a null-terminated string over UART for debugging.
 *
 * @param msg Pointer to the null-terminated string to transmit.
 * @return 0 on success, non-zero on failure.
 */
int debug_uart_print(const char *msg);

#endif // DEBUG_UART_H
