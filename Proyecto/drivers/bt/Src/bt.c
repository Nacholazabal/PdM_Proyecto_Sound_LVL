/**
 * @file bt.c
 * @brief Implementación del módulo de comunicación Bluetooth.
 *
 * Estas funciones utilizan una capa de abstracción para manejar la interfaz física.
 */

#include "bt.h"
#include "port_bt.h"
#include <string.h>

/**
 * Inicializa el módulo Bluetooth llamando al port específico.
 */
bool_t bt_init(void) {
    return port_bt_init();
}

/**
 * Envía un string como mensaje Bluetooth usando la función raw.
 * Internamente convierte el string a un buffer de bytes.
 */
bool_t bt_send(const char* msg) {
    return bt_send_raw((const uint8_t*)msg, strlen(msg));
}

/**
 * Envía datos crudos por Bluetooth usando la función del port.
 */
bool_t bt_send_raw(const uint8_t* data, size_t len) {
    return port_bt_send(data, len);
}
