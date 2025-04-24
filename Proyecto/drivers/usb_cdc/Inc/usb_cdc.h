/**
 * @file usb_cdc.h
 * @brief Módulo de alto nivel para comunicación USB CDC.
 *
 * Este módulo proporciona una abstracción para la comunicación USB CDC,
 * gestionando el buffer de comandos recibidos y la transmisión de datos.
 * La implementación depende de la capa port (port_usb_cdc) para la transferencia real.
 */

#ifndef USB_CDC_H
#define USB_CDC_H

#include <stdint.h>
#include <stdbool.h>

/** Tamaño máximo del buffer de comando USB recibido */
#define USB_CMD_BUFFER_SIZE 128

/**
 * @brief Inicializa el driver USB CDC.
 *
 * Esta función limpia el buffer interno y reinicia la bandera de comando pendiente.
 * Se asume que la inicialización del hardware USB ya fue hecha por CubeMX.
 */
void usb_cdc_init(void);

/**
 * @brief Callback de recepción de datos USB CDC.
 *
 * Esta función debe ser llamada desde la función de recepción de la capa USB.
 * Almacena los datos recibidos en un buffer interno y marca el comando como pendiente.
 *
 * @param Buf Puntero a los datos recibidos.
 * @param Len Longitud en bytes de los datos.
 */
void usb_cdc_receiveCallback(uint8_t* Buf, uint32_t Len);

/**
 * @brief Indica si hay un comando USB pendiente de ser procesado.
 *
 * @return true si se recibió un comando, false si no.
 */
bool usb_cdc_isCommandPending(void);

/**
 * @brief Devuelve el comando pendiente almacenado.
 *
 * @return Puntero a un string terminado en null con el comando recibido.
 */
const char* usb_cdc_getCommand(void);

/**
 * @brief Limpia el comando pendiente y el buffer.
 *
 * Debe llamarse luego de procesar el comando.
 */
void usb_cdc_clearCommand(void);

/**
 * @brief Envía un string por USB CDC.
 *
 * Esta función utiliza la función de transmisión de la capa port.
 *
 * @param str Puntero al string null-terminado.
 * @return 0 si fue exitoso, distinto de 0 si hubo error.
 */
uint8_t usb_cdc_sendString(const char* str);

#endif // USB_CDC_H
