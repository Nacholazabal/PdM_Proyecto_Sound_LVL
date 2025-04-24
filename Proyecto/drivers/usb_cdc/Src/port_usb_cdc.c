/**
 * @file port_usb_cdc.c
 * @brief Implementación de la capa de abstracción de hardware para USB CDC.
 *
 * Implementa funciones que encapsulan las llamadas a la capa de HAL/Middleware de STM32,
 * como CDC_Transmit_FS, para mantener independencia del hardware.
 */

// === Includes ===
#include <stdint.h>

#include "port_usb_cdc.h"
#include "usbd_cdc_if.h"  // Proporciona CDC_Transmit_FS

/**
 * @brief Transmite datos por USB CDC utilizando la función de HAL.
 */
int port_usb_cdc_transmit(const uint8_t *buf, uint16_t len)
{
    return CDC_Transmit_FS((uint8_t*)buf, len);
}

