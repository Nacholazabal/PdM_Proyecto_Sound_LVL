#include "port_eeprom.h"
#include "main.h"          // for extern I2C handle
#include "stm32f4xx_hal.h"

#define EEPROM_I2C_ADDR    (0x50 << 1)   // AT24C32 base address

extern I2C_HandleTypeDef hi2c1;          // from main.h

bool port_eeprom_init(void)
{
    // nothing to do: CubeMX already calls HAL_I2C_Init(&hi2c1)
    return true;
}

bool eeprom_read(uint16_t mem_addr, uint8_t *data, uint16_t len)
{
    return (HAL_I2C_Mem_Read(&hi2c1,
                             EEPROM_I2C_ADDR,
                             mem_addr,
                             I2C_MEMADD_SIZE_16BIT,
                             data,
                             len,
                             500) == HAL_OK);
}

bool eeprom_write(uint16_t mem_addr, const uint8_t *data, uint16_t len)
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
    // wait for the internal write cycle (~5 ms max)
    HAL_Delay(6);
    return true;
}
