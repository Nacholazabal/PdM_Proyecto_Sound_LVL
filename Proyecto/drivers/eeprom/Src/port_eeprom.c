/**
 * @file port_eeprom.c
 * @brief Implementación de funciones de acceso a EEPROM externa vía I2C.
 *
 * Utiliza funciones HAL para leer y escribir datos a través de I2C en una EEPROM externa.
 */

#include "port_eeprom.h"
#include "main.h"          // handler I2C externo
#include "stm32f4xx_hal.h"

#define EEPROM_I2C_ADDR    (0x50 << 1)   ///< Dirección base del dispositivo AT24C32

extern I2C_HandleTypeDef hi2c1;

/**
 * Inicializa el periférico I2C para la EEPROM.
 *
 * En este caso, no se hace nada porque CubeMX ya inicializa I2C en main().
 */
bool_t port_eeprom_init(void)
{
    return true;
}

/**
 * Lee datos desde la EEPROM por I2C.
 */
bool_t eeprom_read(uint16_t mem_addr, uint8_t *data, uint16_t len)
{
    return (HAL_I2C_Mem_Read(&hi2c1,
                             EEPROM_I2C_ADDR,
                             mem_addr,
                             I2C_MEMADD_SIZE_16BIT,
                             data,
                             len,
                             500) == HAL_OK);
}

/**
 * Escribe datos a la EEPROM por I2C.
 *
 * Luego de escribir, espera al menos 6 ms para completar el ciclo interno de escritura.
 */
bool_t eeprom_write(uint16_t mem_addr, const uint8_t *data, uint16_t len)
{
    if (HAL_I2C_Mem_Write(&hi2c1,
                          EEPROM_I2C_ADDR,
                          mem_addr,
                          I2C_MEMADD_SIZE_16BIT,
                          (uint8_t*)data,
                          len,
                          500) != HAL_OK)
    {
        return false;
    }
    // Esperamos el ciclo de escritura interna de la EEPROM (~5 ms)
    HAL_Delay(6);
    return true;
}
