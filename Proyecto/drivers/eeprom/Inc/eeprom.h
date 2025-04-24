#pragma once
#include <stdint.h>
#include <stdbool.h>

// ─── Layout de la EEPROM ──────────────────────────────────────────────
#define EEPROM_SIGNATURE_ADDR      0x0000  ///< Dirección de la firma de validación
#define EEPROM_SIGNATURE_VALUE     0xA5u   ///< Valor esperado de la firma

#define EEPROM_THRESH_ADDR         0x0001  ///< Dirección para el umbral bajo (LOW)
#define EEPROM_HIGH_THRESH_ADDR    0x0003  ///< Dirección para el umbral alto (HIGH)

// ─── Valores por defecto para los umbrales ────────────────────────────
#define DEFAULT_THRESHOLD_LOW      10u     ///< Umbral bajo por defecto
#define DEFAULT_THRESHOLD_HIGH     20u     ///< Umbral alto por defecto

// ─── Definiciones del buffer circular de logs ─────────────────────────
#define EEPROM_LOG_HEAD_ADDR     0x000F    ///< Dirección del puntero HEAD
#define EEPROM_LOG_COUNT_ADDR    0x0010    ///< Dirección del contador de entradas válidas
#define EEPROM_LOG_ENTRY_ADDR    0x0020    ///< Dirección del primer slot de log

#define EEPROM_LOG_MAX_ENTRIES   10        ///< Máximo de entradas a almacenar
#define EEPROM_LOG_ENTRY_SIZE    sizeof(eeprom_log_entry_t) ///< Tamaño de cada entrada

typedef bool bool_t;
// ─── Estructura de log en EEPROM ──────────────────────────────────────
typedef struct {
    uint8_t  year, month, day;
    uint8_t  hour, minute, second;
    uint16_t level; ///< Nivel de ruido registrado
} eeprom_log_entry_t;

/**
 * @brief Inicializa la EEPROM.
 *
 * Si no se detecta firma, se escribe la firma y se graban los umbrales por defecto.
 *
 * @return true si todo fue correcto, false en caso de error.
 */
bool_t eeprom_init(void);

/**
 * @brief Lee los umbrales LOW y HIGH desde la EEPROM.
 *
 * @param[out] low  Puntero donde se guardará el umbral bajo.
 * @param[out] high Puntero donde se guardará el umbral alto.
 *
 * @return true si la lectura fue exitosa.
 */
bool_t eeprom_read_thresholds(uint16_t *low, uint16_t *high);

/**
 * @brief Escribe nuevos valores de umbral bajo y alto en EEPROM.
 *
 * @param low  Nuevo valor para el umbral bajo.
 * @param high Nuevo valor para el umbral alto.
 *
 * @return true si ambas escrituras fueron exitosas.
 */
bool_t eeprom_write_thresholds(uint16_t low, uint16_t high);

/**
 * @brief Guarda una nueva entrada en el log de eventos de alto ruido.
 *
 * @param evt Puntero a la entrada de log a guardar.
 *
 * @return true si se escribió correctamente.
 */
bool_t eeprom_log_high_event(const eeprom_log_entry_t *evt);

/**
 * @brief Lee las últimas entradas del log de eventos de ruido alto.
 *
 * @param[out] entries     Arreglo donde se guardarán las entradas leídas.
 * @param      max_entries Tamaño máximo del arreglo.
 * @param[out] out_count   Cantidad real de entradas devueltas.
 *
 * @return true si se leyeron correctamente.
 */
bool_t eeprom_read_log(eeprom_log_entry_t *entries, uint8_t max_entries, uint8_t *out_count);

/**
 * @brief Borra el contenido del log de eventos.
 */
void eeprom_erase_log(void);

/**
 * @brief Restaura los umbrales a valores por defecto.
 *
 * Marca también la EEPROM como no inicializada para que eeprom_init() reescriba los valores.
 *
 * @return true si todas las escrituras fueron exitosas, false en error.
 */
bool_t eeprom_restore_defaults(void);
