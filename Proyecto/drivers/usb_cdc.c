/**
 * @file usb_cdc.c
 * @brief High-level USB CDC driver implementation.
 *
 * This module implements the logic for buffering and processing USB CDC commands.
 * It uses the port_usb_cdc layer to perform actual data transmission.
 * Commands are stored in a static buffer and a flag indicates a pending command.
 */

#include "usb_cdc.h"
#include "port_usb_cdc.h"
#include <string.h>
#include "debug_uart.h"

static volatile bool command_pending = false; ///< Flag indicating a received command is pending
static char usb_cmd_buffer[USB_CMD_BUFFER_SIZE] = { 0 };

/**
 * @brief Initializes the USB CDC driver.
 */
void usb_cdc_init(void) {
	command_pending = false;
	memset(usb_cmd_buffer, 0, USB_CMD_BUFFER_SIZE);
}

/**
 * @brief Processes received USB data.
 *
 * This function should be called by the low-level USB receive callback.
 * It copies data into the command buffer and marks a command as pending.
 *
 * @param Buf Pointer to the received data buffer.
 * @param Len Number of bytes received.
 */
void usb_cdc_receiveCallback(uint8_t *Buf, uint32_t Len) {
	debug_uart_print("usb_cdc_receiveCallback()\r\n");
	debug_uart_print((char*) Buf);
	debug_uart_print("\r\n");
	while (Len > 0 && (Buf[Len - 1] == '\r' || Buf[Len - 1] == '\n')) {
		Len--;
	}
	if (!command_pending && Len < USB_CMD_BUFFER_SIZE) {
		memcpy(usb_cmd_buffer, Buf, Len);
		// Null-terminate the string safely.
		usb_cmd_buffer[Len < USB_CMD_BUFFER_SIZE ? Len : USB_CMD_BUFFER_SIZE - 1] =
				'\0';
		command_pending = true;
	}
	// If a command is already pending, new data is discarded.
}

/**
 * @brief Checks if there is a pending USB CDC command.
 *
 * @return true if there is a pending command, false otherwise.
 */
bool usb_cdc_isCommandPending(void) {
	return command_pending;
}

/**
 * @brief Retrieves the buffered USB CDC command.
 *
 * @return Pointer to the null-terminated command string.
 */
const char* usb_cdc_getCommand(void) {
	return usb_cmd_buffer;
}

/**
 * @brief Clears the current USB CDC command.
 */
void usb_cdc_clearCommand(void) {
	command_pending = false;
	memset(usb_cmd_buffer, 0, USB_CMD_BUFFER_SIZE);
}

/**
 * @brief Sends a string over USB CDC.
 *
 * This function calls the port layer function to actually transmit the data.
 *
 * @param str The null-terminated string to send.
 * @return 0 on success, non-zero on error.
 */
int usb_cdc_sendString(const char *str) {
	uint16_t len = (uint16_t) strlen(str);
	return port_usb_cdc_transmit((const uint8_t*) str, len);
}

