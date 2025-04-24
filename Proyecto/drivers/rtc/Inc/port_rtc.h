/**
 * @file port_rtc.h
 * @brief Capa de abstracción para comunicación con el RTC DS1307 por I2C.
 *
 * Este módulo define funciones para inicializar, leer y escribir registros del RTC DS1307.
 */
#ifndef PORT_RTC_H
#define PORT_RTC_H

#include <stdbool.h>
#include <stdint.h>

/**
 * Inicializa la interfaz I2C usada por el RTC.
 *
 * Debe llamarse una vez luego de que el periférico I2C1 esté inicializado.
 *
 * @return true si la inicialización fue exitosa.
 */
bool port_rtc_init(void);

/**
 * Lee bytes desde registros del DS1307 a partir de una dirección.
 *
 * @param reg_addr Dirección del registro inicial del RTC.
 * @param buf      Buffer donde se guardarán los datos leídos.
 * @param len      Cantidad de bytes a leer.
 *
 * @return true si la lectura fue exitosa.
 */
bool port_rtc_read(uint8_t reg_addr, uint8_t *buf, uint8_t len);

/**
 * Escribe datos en registros del DS1307 a partir de una dirección.
 *
 * @param reg_addr Dirección del registro inicial del RTC.
 * @param buf      Buffer con los datos a escribir.
 * @param len      Cantidad de bytes a escribir.
 *
 * @return true si la escritura fue exitosa.
 */
bool port_rtc_write(uint8_t reg_addr, const uint8_t *buf, uint8_t len);

#endif // PORT_RTC
