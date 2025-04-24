/**
 * @file usb_cdc.c
 * @brief Implementación del módulo de alto nivel USB CDC.
 *
 * Este módulo implementa el manejo de comandos USB CDC recibidos y su transmisión.
 * Utiliza la capa port para la comunicación real con el hardware USB.
 */

#include <string.h>

#include "debug_uart.h"
#include "port_usb_cdc.h"
#include "usb_cdc.h"

// Bandera que indica si hay un comando pendiente por procesar
static volatile bool command_pending = false;
// Buffer de comando recibido (terminado en null)
static char usb_cmd_buffer[USB_CMD_BUFFER_SIZE] = { 0 };

void usb_cdc_init(void) {
    command_pending = false;
    memset(usb_cmd_buffer, 0, USB_CMD_BUFFER_SIZE);
}

void usb_cdc_receiveCallback(uint8_t *Buf, uint32_t Len) {
    debug_uart_print("usb_cdc_receiveCallback()\r\n");
    debug_uart_print((char*) Buf);
    debug_uart_print("\r\n");

    // Elimina caracteres de nueva línea o retorno de carro al final
    while (Len > 0 && (Buf[Len - 1] == '\r' || Buf[Len - 1] == '\n')) {
        Len--;
    }

    // Si no hay otro comando pendiente y cabe en el buffer, lo copiamos
    if (!command_pending && Len < USB_CMD_BUFFER_SIZE) {
        memcpy(usb_cmd_buffer, Buf, Len);

        // Garantiza que el buffer quede null-terminado para usarlo como string
        usb_cmd_buffer[Len < USB_CMD_BUFFER_SIZE ? Len : USB_CMD_BUFFER_SIZE - 1] = '\0';

        command_pending = true;
    }
    // Si ya había un comando pendiente, se descarta la nueva entrada
}

bool usb_cdc_isCommandPending(void) {
    return command_pending;
}

const char* usb_cdc_getCommand(void) {
    return usb_cmd_buffer;
}

void usb_cdc_clearCommand(void) {
    command_pending = false;
    memset(usb_cmd_buffer, 0, USB_CMD_BUFFER_SIZE);
}

uint8_t usb_cdc_sendString(const char *str) {
    uint16_t len = (uint16_t) strlen(str);
    return port_usb_cdc_transmit((const uint8_t*) str, len);
}
