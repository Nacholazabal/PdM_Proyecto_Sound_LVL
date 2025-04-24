/**
 * @file port_usb_cdc.h
 * @brief Capa de abstracción de hardware para funciones USB CDC.
 *
 * Este archivo declara las funciones de bajo nivel que interactúan directamente con la interfaz USB CDC.
 * Permite que el driver de alto nivel sea independiente del hardware y de la implementación HAL.
 */

#ifndef PORT_USB_CDC_H
#define PORT_USB_CDC_H

#include <stdint.h>

/**
 * @brief Transmite datos por la interfaz USB CDC.
 *
 * Esta función encapsula la llamada a la función de transmisión de la HAL (por ejemplo, CDC_Transmit_FS).
 *
 * @param buf Puntero al buffer de datos a transmitir.
 * @param len Cantidad de bytes a transmitir.
 * @return 0 si la transmisión fue exitosa, distinto de 0 en caso de error.
 */
int port_usb_cdc_transmit(const uint8_t *buf, uint16_t len);

#endif // PORT_USB_CDC_H
