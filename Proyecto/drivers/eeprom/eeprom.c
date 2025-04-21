#include "eeprom.h"
#include "port_eeprom.h"
#include <string.h>


// ——— Initialize EEPROM and thresholds ———
bool eeprom_init(void)
{
    uint8_t sig;
    // Read the stored signature byte
    if (!eeprom_read(EEPROM_SIGNATURE_ADDR, &sig, 1)) {
        return false;
    }

    // If signature mismatches, first‑time setup:
    if (sig != EEPROM_SIGNATURE_VALUE) {
        // Buffer: [ signature ][ LOW (2) ][ HIGH (2) ]
        uint8_t buf[1 + 2*2];
        buf[0] = EEPROM_SIGNATURE_VALUE;

        // Pack default LOW
        buf[1] = (uint8_t)(DEFAULT_THRESHOLD_LOW & 0xFF);
        buf[2] = (uint8_t)(DEFAULT_THRESHOLD_LOW >> 8);
        // Pack default HIGH
        buf[3] = (uint8_t)(DEFAULT_THRESHOLD_HIGH & 0xFF);
        buf[4] = (uint8_t)(DEFAULT_THRESHOLD_HIGH >> 8);

        // Write signature + defaults
        if (!eeprom_write(EEPROM_SIGNATURE_ADDR, buf, sizeof(buf))) {
            return false;
        }
    }
    return true;
}

// ——— Read LOW & HIGH thresholds ———
bool eeprom_read_thresholds(uint16_t *low, uint16_t *high)
{
    uint8_t buf[2*2];
    if (!eeprom_read(EEPROM_THRESH_ADDR, buf, sizeof(buf))) {
        return false;
    }
    // Little‑endian unpack
    *low  = (uint16_t)buf[0] | ((uint16_t)buf[1] << 8);
    *high = (uint16_t)buf[2] | ((uint16_t)buf[3] << 8);
    return true;
}

// ——— Write new LOW & HIGH thresholds ———
bool eeprom_write_thresholds(uint16_t low, uint16_t high)
{
    uint8_t buf[2*2];
    buf[0] = (uint8_t)(low & 0xFF);
    buf[1] = (uint8_t)(low >> 8);
    buf[2] = (uint8_t)(high & 0xFF);
    buf[3] = (uint8_t)(high >> 8);
    return eeprom_write(EEPROM_THRESH_ADDR, buf, sizeof(buf));
}

bool eeprom_log_high_event(const eeprom_log_entry_t *evt)
{
    // 1) read existing log into RAM (up to EEPROM_LOG_MAX_ENTRIES)
    eeprom_log_entry_t tmp[EEPROM_LOG_MAX_ENTRIES];
    uint8_t count = 0;
    eeprom_read_log(tmp, EEPROM_LOG_MAX_ENTRIES, &count);

    // 2) shift everything down
    if (count == EEPROM_LOG_MAX_ENTRIES) {
        memmove(&tmp[1], &tmp[0], (EEPROM_LOG_MAX_ENTRIES-1)*sizeof(*tmp));
        count = EEPROM_LOG_MAX_ENTRIES;
    } else {
        count++;
        memmove(&tmp[1], &tmp[0], (count-1)*sizeof(*tmp));
    }

    // 3) insert new event at tmp[0]
    tmp[0] = *evt;

    // 4) write back `count` entries to EEPROM
    return eeprom_write(EEPROM_LOG_BASE_ADDR,
                              (uint8_t*)tmp,
                              count * EEPROM_LOG_ENTRY_SIZE);
    return true;
}

bool eeprom_read_log(eeprom_log_entry_t *entries, uint8_t max_entries, uint8_t *out_count)
{
    // calculate how many bytes are valid: max_entries * size
    uint16_t to_read = max_entries * EEPROM_LOG_ENTRY_SIZE;
    uint8_t buf[to_read];
    if (!eeprom_read(EEPROM_LOG_BASE_ADDR, buf, to_read))
        return false;

    // unpack; stop if you hit a sentinel (e.g. year==0) or max_entries
    uint8_t count = 0;
    for (uint8_t i = 0; i < max_entries; i++) {
        uint8_t *b = &buf[i * EEPROM_LOG_ENTRY_SIZE];
        entries[i].year   = b[0];
        entries[i].month  = b[1];
        entries[i].day    = b[2];
        entries[i].hour   = b[3];
        entries[i].minute = b[4];
        entries[i].second = b[5];
        entries[i].level  = b[6];
        // you can check if year==0 to break early
        count++;
    }
    *out_count = count;
    return true;
}
