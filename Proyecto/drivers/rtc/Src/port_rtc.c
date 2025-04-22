#include "port_rtc.h"
#include "main.h"           // for extern hi2c1
#include "stm32f4xx_hal.h"

#define DS1307_ADDR   (0x68 << 1)

extern I2C_HandleTypeDef hi2c1;

bool port_rtc_init(void)
{
    // nothing extra to do: CubeMX already init’s hi2c1
    return true;
}

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
    // DS1307 has no internal write‑cycle delay, but a small delay is safe
    HAL_Delay(1);
    return true;
}
