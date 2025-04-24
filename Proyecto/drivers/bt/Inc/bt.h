/**
 * @file bt.h
 * @brief Módulo de comunicación Bluetooth
 *
 * Proporciona funciones para inicializar y enviar datos a través del módulo Bluetooth.
 * Estas funciones dependen de una capa de portabilidad definida en port_bt.
 */

#ifndef BT_H
#define BT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef bool bool_t;
/**
 * @brief Inicializa el módulo Bluetooth.
 *
 * Esta función inicializa el hardware o interfaz correspondiente para la comunicación Bluetooth.
 *
 * @return true si la inicialización fue exitosa, false en caso contrario.
 */
bool_t bt_init(void);

/**
 * @brief Envía un mensaje de texto por Bluetooth.
 *
 * Esta función convierte un string a bytes y lo envía usando la función raw.
 *
 * @param msg Puntero al string nulo-terminado a enviar.
 * @return true si el mensaje fue enviado correctamente.
 */
bool_t bt_send(const char* msg);

/**
 * @brief Envía datos crudos (binarios) por Bluetooth.
 *
 * @param data Puntero al buffer de datos.
 * @param len Longitud del buffer.
 * @return true si los datos fueron enviados correctamente.
 */
bool_t bt_send_raw(const uint8_t* data, size_t len);

#endif // BT_H
