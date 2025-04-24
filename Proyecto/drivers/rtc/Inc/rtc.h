#ifndef RTC_H
#define RTC_H
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Estructura que representa fecha y hora del RTC.
 *
 * Los campos están en formato binario, no BCD.
 */
typedef struct {
    uint8_t sec;    ///< Segundos (0–59)
    uint8_t min;    ///< Minutos (0–59)
    uint8_t hour;   ///< Horas (0–23)
    uint8_t day;    ///< Día de la semana (1–7, opcional)
    uint8_t date;   ///< Día del mes (1–31)
    uint8_t month;  ///< Mes (1–12)
    uint8_t year;   ///< Año (0–99, se asume 20YY)
} rtc_datetime_t;

/**
 * @brief Inicializa el módulo RTC.
 *
 * Llama a la capa de port y verifica si el reloj está corriendo (CH=0).
 *
 * @return true si I²C funciona y se pudo leer el RTC.
 */
bool rtc_init(void);

/**
 * @brief Verifica si el RTC tiene una hora válida cargada.
 *
 * Revisa el bit CH del registro de segundos.
 *
 * @return true si el reloj está corriendo.
 */
bool rtc_is_set(void);

/**
 * @brief Lee la fecha y hora actuales desde el RTC.
 *
 * @param[out] dt Estructura donde se guardará la fecha y hora.
 * @return true si la lectura fue exitosa.
 */
bool rtc_get_datetime(rtc_datetime_t *dt);

/**
 * @brief Escribe fecha y hora en el RTC.
 *
 * También pone en marcha el oscilador (CH=0).
 *
 * @param[in] dt Fecha y hora a establecer.
 * @return true si se escribió correctamente.
 */
bool rtc_set_datetime(const rtc_datetime_t *dt);

#endif // RTC_H
