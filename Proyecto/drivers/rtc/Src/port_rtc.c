/**
 * @file port_rtc.c
 * @brief Implementación de funciones de bajo nivel para el RTC DS1307 usando I2C.
 *
 * Utiliza funciones de la HAL para leer y escribir registros del RTC.
 */

// === Includes ===
#include <stdbool.h>
#include <stdint.h>

#include "main.h"           // handler I2C1 definido en el sistema
#include "port_rtc.h"
#include "stm32f4xx_hal.h"

#define DS1307_ADDR   (0x68 << 1) ///< Dirección I2C del DS1307 (7-bit desplazado)

extern I2C_HandleTypeDef hi2c1;

/**
 * Inicializa el puerto del RTC. No se necesita lógica adicional ya que CubeMX
 * ya inicializa el periférico I2C.
 */
bool port_rtc_init(void)
{
    return true;
}

/**
 * Lee datos desde registros del DS1307.
 *
 * Utiliza lectura de memoria con dirección de 8 bits.
 */
bool port_rtc_read(uint8_t reg_addr, uint8_t *buf, uint8_t len)
{
    return (HAL_I2C_Mem_Read(&hi2c1,
                             DS1307_ADDR,
                             reg_addr,
                             I2C_MEMADD_SIZE_8BIT,
                             buf,
                             len,
                             200) == HAL_OK);
}

/**
 * Escribe datos en registros del DS1307.
 *
 * Aunque el DS1307 no requiere retardo posterior a la escritura,
 * se introduce una pequeña demora por seguridad.
 */
bool port_rtc_write(uint8_t reg_addr, const uint8_t *buf, uint8_t len)
{
    if (HAL_I2C_Mem_Write(&hi2c1,
                          DS1307_ADDR,
                          reg_addr,
                          I2C_MEMADD_SIZE_8BIT,
                          (uint8_t*)buf,
                          len,
                          200) != HAL_OK) {
        return false;
    }

    // Breve retardo por seguridad (aunque no es estrictamente necesario para el DS1307)
    HAL_Delay(1);
    return true;
}
