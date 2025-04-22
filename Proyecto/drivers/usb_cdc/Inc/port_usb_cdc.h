/**
 * @file port_usb_cdc.h
 * @brief Hardware abstraction layer for USB CDC functions.
 *
 * This file declares the port-level functions that directly interact with the USB CDC middleware.
 * It encapsulates the lower-level HAL functions so that the high-level driver remains hardware independent.
 */

#ifndef PORT_USB_CDC_H
#define PORT_USB_CDC_H

#include <stdint.h>

/**
 * @brief Transmits data over USB CDC.
 *
 * This function wraps the underlying USB CDC transmit function (e.g., CDC_Transmit_FS).
 *
 * @param buf Pointer to the data buffer to transmit.
 * @param len Length in bytes of the data to transmit.
 * @return 0 if successful, non-zero otherwise.
 */
int port_usb_cdc_transmit(const uint8_t *buf, uint16_t len);

/**
 * @brief Registers a receive callback.
 *
 * This function allows the high-level USB CDC driver to be notified when data is received.
 * The callback will be invoked from within the USB middleware receive routine.
 *
 * @param callback A pointer to a function that accepts a buffer and its length.
 */
void port_usb_cdc_register_receive_callback(void (*callback)(uint8_t*, uint32_t));

#endif // PORT_USB_CDC_H
