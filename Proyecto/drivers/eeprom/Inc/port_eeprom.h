#pragma once
#include <stdint.h>
#include <stdbool.h>

/// Call once at boot (I²C1 is already CubeMX‑initialized)
bool port_eeprom_init(void);

/// Read `len` bytes from EEPROM at 16‑bit word address `mem_addr`
/// Returns true on success
bool eeprom_read(uint16_t mem_addr, uint8_t *data, uint16_t len);

/// Write `len` bytes to EEPROM at 16‑bit word address `mem_addr`
/// Takes care of the internal write cycle delay
bool eeprom_write(uint16_t mem_addr, const uint8_t *data, uint16_t len);
