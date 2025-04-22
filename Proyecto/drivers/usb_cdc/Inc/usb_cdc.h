/**
 * @file usb_cdc.h
 * @brief High-level USB CDC driver interface.
 *
 * This module provides an abstraction for USB CDC communication.
 * It manages the buffering of received commands and the transmission of data.
 * The implementation relies on the port layer (port_usb_cdc) for the actual USB transfers.
 */

#ifndef USB_CDC_H
#define USB_CDC_H

#include <stdint.h>
#include <stdbool.h>

/** Maximum size of the received USB command buffer */
#define USB_CMD_BUFFER_SIZE 128

/**
 * @brief Initializes the USB CDC driver.
 *
 * This function resets the internal buffer and clears the command pending flag.
 * USB hardware initialization is assumed to be handled by CubeMX.
 */
void usb_cdc_init(void);

/**
 * @brief USB CDC receive callback.
 *
 * This function should be called by the USB middleware's receive callback.
 * It stores the incoming data into an internal buffer and sets a flag indicating a pending command.
 *
 * @param Buf Pointer to the received data.
 * @param Len Length in bytes of the received data.
 */
void usb_cdc_receiveCallback(uint8_t* Buf, uint32_t Len);

/**
 * @brief Checks whether a USB CDC command is pending.
 *
 * @return true if a command has been received and not yet processed, false otherwise.
 */
bool usb_cdc_isCommandPending(void);

/**
 * @brief Retrieves the pending USB CDC command.
 *
 * @return Pointer to a null-terminated string containing the received command.
 */
const char* usb_cdc_getCommand(void);

/**
 * @brief Clears the pending USB CDC command flag and buffer.
 *
 * This should be called after the command has been processed.
 */
void usb_cdc_clearCommand(void);

/**
 * @brief Sends a null-terminated string over USB CDC.
 *
 * This function wraps the lower-level port function to transmit data.
 *
 * @param str Pointer to the string to be sent.
 * @return 0 on success, non-zero on error.
 */
int usb_cdc_sendString(const char* str);

#endif // USB_CDC_H
