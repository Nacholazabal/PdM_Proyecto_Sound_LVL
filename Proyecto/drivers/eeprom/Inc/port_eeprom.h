/**
 * @file port_eeprom.h
 * @brief Capa de abstracción de hardware para acceso a EEPROM externa por I2C.
 *
 * Define funciones para inicializar, leer y escribir datos en una EEPROM externa
 * utilizando la interfaz I2C (por ejemplo, un AT24C32).
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * Inicializa el periférico I2C usado para la EEPROM.
 *
 * @return true si la inicialización fue exitosa.
 */
bool port_eeprom_init(void);

/**
 * Lee una cantidad de bytes desde una dirección específica de la EEPROM.
 *
 * @param mem_addr Dirección de inicio en memoria EEPROM (16 bits).
 * @param data     Puntero al buffer donde guardar los datos leídos.
 * @param len      Cantidad de bytes a leer.
 * @return true si la lectura fue exitosa.
 */
bool eeprom_read(uint16_t mem_addr, uint8_t *data, uint16_t len);

/**
 * Escribe datos en la EEPROM a partir de una dirección dada.
 *
 * @param mem_addr Dirección de inicio en memoria EEPROM (16 bits).
 * @param data     Puntero al buffer de datos a escribir.
 * @param len      Cantidad de bytes a escribir.
 * @return true si la escritura fue exitosa.
 */
bool eeprom_write(uint16_t mem_addr, const uint8_t *data, uint16_t len);
