#pragma once
#include <stdint.h>
#include <stdbool.h>

/// Must call once after I2C1 is up
bool port_rtc_init(void);

/// Read raw bytes starting at DS1307 register `reg_addr`
bool port_rtc_read(uint8_t reg_addr, uint8_t *buf, uint8_t len);

/// Write raw bytes starting at DS1307 register `reg_addr`
bool port_rtc_write(uint8_t reg_addr, const uint8_t *buf, uint8_t len);
