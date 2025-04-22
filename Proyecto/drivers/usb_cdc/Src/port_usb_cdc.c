/**
 * @file port_usb_cdc.c
 * @brief Hardware abstraction layer for USB CDC functions.
 *
 * This module implements the port-level functions by wrapping the low-level USB CDC functions provided
 * by the STM32Cube HAL middleware (such as CDC_Transmit_FS). It isolates the high-level driver from direct calls
 * to HAL, making the code more portable.
 */

#include "port_usb_cdc.h"
#include "usbd_cdc_if.h"  // Provides CDC_Transmit_FS and potentially other USB CDC functions

/**
 * @brief Transmits data over USB CDC.
 *
 * Calls the HAL function CDC_Transmit_FS to send data.
 */
int port_usb_cdc_transmit(const uint8_t *buf, uint16_t len)
{
    return CDC_Transmit_FS((uint8_t*)buf, len);
}

/**
 * @brief Registers a receive callback for USB CDC.
 *
 * This example implementation does not wrap a dynamic registration because the USB middleware's
 * receive callback (CDC_Receive_FS) is typically fixed. In a more advanced design, you might store the
 * callback pointer and call it from CDC_Receive_FS.
 *
 * For now, this function is a placeholder.
 */
void port_usb_cdc_register_receive_callback(void (*callback)(uint8_t*, uint32_t))
{
    // In a full implementation, store the callback pointer and invoke it from the USB middleware's receive function.
    (void)callback; // Suppress unused parameter warning
}
