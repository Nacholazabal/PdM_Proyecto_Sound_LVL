#pragma once
#include <stdint.h>
#include <stdbool.h>

// EEPROM layout
#define EEPROM_SIGNATURE_ADDR      0x0000
#define EEPROM_SIGNATURE_VALUE     0xA5u

#define EEPROM_THRESH_ADDR         0x0001  // 2 bytes: LOW threshold
#define EEPROM_HIGH_THRESH_ADDR    0x0003  // 2 bytes: HIGH threshold

// Default thresholds (named instead of “magic”)
#define DEFAULT_THRESHOLD_LOW      10u
#define DEFAULT_THRESHOLD_HIGH     20u
// after your other #defines

// ─── Log ring buffer definitions ───────────────────────────────
#define EEPROM_LOG_HEAD_ADDR     0x000F  // HEAD pointer
#define EEPROM_LOG_COUNT_ADDR    0x0010  // COUNT of valid entries
#define EEPROM_LOG_ENTRY_ADDR    0x0020  // first log slot

#define EEPROM_LOG_MAX_ENTRIES   10
#define EEPROM_LOG_ENTRY_SIZE    sizeof(eeprom_log_entry_t)

typedef struct {
    uint8_t  year, month, day;
    uint8_t  hour, minute, second;
    uint16_t level;
} eeprom_log_entry_t;

// Initialize EEPROM: if signature missing, write signature + default thresholds
bool eeprom_init(void);

// Read back LOW and HIGH thresholds
bool eeprom_read_thresholds(uint16_t *low, uint16_t *high);

// Write new LOW and HIGH thresholds
bool eeprom_write_thresholds(uint16_t low, uint16_t high);

// (Log functions remain unchanged…)
bool eeprom_log_high_event(const eeprom_log_entry_t *evt);
bool eeprom_read_log(eeprom_log_entry_t *entries,
                     uint8_t max_entries,
                     uint8_t *out_count);
