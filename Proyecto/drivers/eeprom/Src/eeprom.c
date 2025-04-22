#include "eeprom.h"
#include "port_eeprom.h"
#include <string.h>
#include <stdio.h>
#include "debug_uart.h"

// ——— Initialize EEPROM and thresholds ———
bool eeprom_init(void)
{
    uint8_t sig;
    // 1) Read the stored signature byte
    if (!eeprom_read(EEPROM_SIGNATURE_ADDR, &sig, 1)) {
        return false;
    }

    // 2) If signature mismatches, first‑time setup
    if (sig != EEPROM_SIGNATURE_VALUE) {
        // ─── Write signature + default thresholds ───
        uint8_t buf[1 + 2*2];
        buf[0] = EEPROM_SIGNATURE_VALUE;
        buf[1] = (uint8_t)(DEFAULT_THRESHOLD_LOW  & 0xFF);
        buf[2] = (uint8_t)(DEFAULT_THRESHOLD_LOW >> 8);
        buf[3] = (uint8_t)(DEFAULT_THRESHOLD_HIGH & 0xFF);
        buf[4] = (uint8_t)(DEFAULT_THRESHOLD_HIGH >> 8);

        if (!eeprom_write(EEPROM_SIGNATURE_ADDR, buf, sizeof(buf))) {
            return false;
        }

        // ─── Now zero out the ring‑buffer pointers ───
        uint8_t zero = 0;
        if (!eeprom_write(EEPROM_LOG_HEAD_ADDR,  &zero, 1) ||
            !eeprom_write(EEPROM_LOG_COUNT_ADDR, &zero, 1))
        {
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
    uint8_t head, count;

    // 1) Read HEAD & COUNT
    eeprom_read(EEPROM_LOG_HEAD_ADDR,  &head,  1);
    eeprom_read(EEPROM_LOG_COUNT_ADDR, &count, 1);
    if (head  >= EEPROM_LOG_MAX_ENTRIES) head  = 0;
    if (count >  EEPROM_LOG_MAX_ENTRIES) count = 0;

    // 2) Bump COUNT (cap at max)
    if (count < EEPROM_LOG_MAX_ENTRIES) {
        count++;
    }

    // 3) Write the new entry at slot ‘head’
    uint16_t slot_addr = EEPROM_LOG_ENTRY_ADDR
                       + (uint16_t)head * EEPROM_LOG_ENTRY_SIZE;
    if (!eeprom_write(slot_addr, (uint8_t*)evt, EEPROM_LOG_ENTRY_SIZE)) {
        return false;
    }

    // 4) Advance HEAD (wrap) and store both HEAD & COUNT
    head = (head + 1) % EEPROM_LOG_MAX_ENTRIES;
    eeprom_write(EEPROM_LOG_HEAD_ADDR,  &head,  1);
    eeprom_write(EEPROM_LOG_COUNT_ADDR, &count, 1);

    debug_uart_print("LOG: wrote slot ");
    char dbg[32];
    sprintf(dbg, "%u, count=%u\r\n", head, count);
    debug_uart_print(dbg);

    return true;
}



bool eeprom_read_log(eeprom_log_entry_t *entries,
                     uint8_t max_entries,
                     uint8_t *out_count)
{
    uint8_t head, count;
    // 1) Read HEAD & COUNT
    if (!eeprom_read(EEPROM_LOG_HEAD_ADDR,  &head,  1) ||
        !eeprom_read(EEPROM_LOG_COUNT_ADDR, &count, 1))
    {
        return false;
    }
    if (head  >= EEPROM_LOG_MAX_ENTRIES) head  = 0;
    if (count >  EEPROM_LOG_MAX_ENTRIES) count = EEPROM_LOG_MAX_ENTRIES;
    if (count >  max_entries)          count = max_entries;

    // 2) Compute start index = oldest entry
    uint8_t start = (head + EEPROM_LOG_MAX_ENTRIES - count)
                  % EEPROM_LOG_MAX_ENTRIES;

    // 3) Read ‘count’ slots in order
    for (uint8_t i = 0; i < count; i++) {
        uint8_t idx = (start + i) % EEPROM_LOG_MAX_ENTRIES;
        uint16_t slot_addr = EEPROM_LOG_ENTRY_ADDR
                           + idx * EEPROM_LOG_ENTRY_SIZE;
        if (!eeprom_read(slot_addr,
                         (uint8_t*)&entries[i],
                         EEPROM_LOG_ENTRY_SIZE))
        {
            return false;
        }
    }

    *out_count = count;
    return true;
}

void eeprom_erase_log(void)
{
    uint8_t zero = 0;

    // 1) Reset HEAD pointer y COUNT
    eeprom_write(EEPROM_LOG_HEAD_ADDR,  &zero, 1);
    eeprom_write(EEPROM_LOG_COUNT_ADDR, &zero, 1);

    // 2) Limpiar todas las entradas (opcional, pero deja la EEPROM "vacía")
    uint8_t empty_entry[EEPROM_LOG_ENTRY_SIZE];
    // Usamos 0xFF para marcar celda "borrada" (o el valor que prefieras)
    memset(empty_entry, 0xFF, EEPROM_LOG_ENTRY_SIZE);

    for (uint8_t i = 0; i < EEPROM_LOG_MAX_ENTRIES; i++) {
        uint16_t slot_addr = EEPROM_LOG_ENTRY_ADDR
                           + (uint16_t)i * EEPROM_LOG_ENTRY_SIZE;
        eeprom_write(slot_addr, empty_entry, EEPROM_LOG_ENTRY_SIZE);
    }
}

/**
 * @brief Escribe en EEPROM los thresholds por defecto.
 *        No toca el ring‑buffer de logs.
 */
bool eeprom_restore_defaults(void)
{
    // Usa la función existente de escritura de thresholds:
    if (!eeprom_write_thresholds(DEFAULT_THRESHOLD_LOW,
                                 DEFAULT_THRESHOLD_HIGH))
    {
        return false;
    }
    return true;
}
