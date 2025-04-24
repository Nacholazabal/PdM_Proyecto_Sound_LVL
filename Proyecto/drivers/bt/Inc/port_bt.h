/**
 * @file port_bt.h
 * @brief Capa de abstracción para comunicación Bluetooth
 *
 * Define las funciones necesarias para inicializar y enviar datos a través del hardware UART
 * utilizado para comunicación Bluetooth.
 */

#ifndef PORT_BT_H
#define PORT_BT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Inicializa la interfaz UART para el módulo Bluetooth.
 *
 * Esta función debe ser llamada una vez al inicio del sistema.
 *
 * @return true si la inicialización fue exitosa.
 */
bool port_bt_init(void);

/**
 * @brief Envía datos por la interfaz Bluetooth (UART).
 *
 * @param data Puntero al buffer de datos.
 * @param len Longitud del buffer.
 * @return true si la transmisión fue exitosa.
 */
bool port_bt_send(const uint8_t* data, size_t len);

#endif // PORT_BT_H
