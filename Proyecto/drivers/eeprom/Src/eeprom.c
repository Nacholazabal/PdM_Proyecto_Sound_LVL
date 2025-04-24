#include <stdio.h>
#include <string.h>

#include "debug_uart.h"
#include "eeprom.h"
#include "port_eeprom.h"

// ─── Inicialización de EEPROM ─────────────────────────────────────────────
bool_t eeprom_init(void)
{
    uint8_t sig;

    // 1) Leemos la firma almacenada
    if (!eeprom_read(EEPROM_SIGNATURE_ADDR, &sig, 1)) {
        return false;
    }

    // 2) Si la firma no coincide, es la primera vez → inicializamos
    if (sig != EEPROM_SIGNATURE_VALUE) {
        // ─── Escribimos firma + thresholds por defecto ───
        uint8_t buf[1 + 2*2];
        buf[0] = EEPROM_SIGNATURE_VALUE;
        buf[1] = (uint8_t)(DEFAULT_THRESHOLD_LOW  & 0xFF);
        buf[2] = (uint8_t)(DEFAULT_THRESHOLD_LOW >> 8);
        buf[3] = (uint8_t)(DEFAULT_THRESHOLD_HIGH & 0xFF);
        buf[4] = (uint8_t)(DEFAULT_THRESHOLD_HIGH >> 8);

        if (!eeprom_write(EEPROM_SIGNATURE_ADDR, buf, sizeof(buf))) {
            return false;
        }

        // ─── Inicializamos los punteros del buffer circular ───
        uint8_t zero = 0;
        if (!eeprom_write(EEPROM_LOG_HEAD_ADDR,  &zero, 1) ||
            !eeprom_write(EEPROM_LOG_COUNT_ADDR, &zero, 1))
        {
            return false;
        }
    }

    return true;
}

// ─── Leer thresholds LOW y HIGH ───────────────────────────────────────────
bool_t eeprom_read_thresholds(uint16_t *low, uint16_t *high)
{
    uint8_t buf[2*2];
    if (!eeprom_read(EEPROM_THRESH_ADDR, buf, sizeof(buf))) {
        return false;
    }
    // Desempaquetado little endian
    *low  = (uint16_t)buf[0] | ((uint16_t)buf[1] << 8);
    *high = (uint16_t)buf[2] | ((uint16_t)buf[3] << 8);
    return true;
}

// ─── Escribir nuevos thresholds ───────────────────────────────────────────
bool_t eeprom_write_thresholds(uint16_t low, uint16_t high)
{
    uint8_t buf[2*2];
    buf[0] = (uint8_t)(low & 0xFF);
    buf[1] = (uint8_t)(low >> 8);
    buf[2] = (uint8_t)(high & 0xFF);
    buf[3] = (uint8_t)(high >> 8);
    return eeprom_write(EEPROM_THRESH_ADDR, buf, sizeof(buf));
}

// ─── Guardar nuevo evento de ruido alto ───────────────────────────────────
bool_t eeprom_log_high_event(const eeprom_log_entry_t *evt)
{
    uint8_t head, count;

    // 1) Leer HEAD y COUNT actuales del buffer
    eeprom_read(EEPROM_LOG_HEAD_ADDR,  &head,  1);
    eeprom_read(EEPROM_LOG_COUNT_ADDR, &count, 1);

    // Validar límites por si están corruptos
    if (head  >= EEPROM_LOG_MAX_ENTRIES) head  = 0;
    if (count >  EEPROM_LOG_MAX_ENTRIES) count = 0;

    // 2) Si no está lleno, incrementamos count
    if (count < EEPROM_LOG_MAX_ENTRIES) {
        count++;
    }

    // 3) Calculamos la dirección para guardar el evento
    uint16_t slot_addr = EEPROM_LOG_ENTRY_ADDR
                       + (uint16_t)head * EEPROM_LOG_ENTRY_SIZE;

    if (!eeprom_write(slot_addr, (uint8_t*)evt, EEPROM_LOG_ENTRY_SIZE)) {
        return false;
    }

    // 4) Avanzar HEAD circularmente y guardar ambos punteros
    head = (head + 1) % EEPROM_LOG_MAX_ENTRIES;
    eeprom_write(EEPROM_LOG_HEAD_ADDR,  &head,  1);
    eeprom_write(EEPROM_LOG_COUNT_ADDR, &count, 1);

    // Mensaje de debug
    debug_uart_print("LOG: wrote slot ");
    char dbg[32];
    sprintf(dbg, "%u, count=%u\r\n", head, count);
    debug_uart_print(dbg);

    return true;
}

// ─── Leer los últimos eventos registrados ────────────────────────────────
bool_t eeprom_read_log(eeprom_log_entry_t *entries,
                     uint8_t max_entries,
                     uint8_t *out_count)
{
    uint8_t head, count;

    // 1) Leer HEAD y COUNT actuales
    if (!eeprom_read(EEPROM_LOG_HEAD_ADDR,  &head,  1) ||
        !eeprom_read(EEPROM_LOG_COUNT_ADDR, &count, 1))
    {
        return false;
    }

    // Validaciones de límites
    if (head  >= EEPROM_LOG_MAX_ENTRIES) head  = 0;
    if (count >  EEPROM_LOG_MAX_ENTRIES) count = EEPROM_LOG_MAX_ENTRIES;
    if (count >  max_entries)             count = max_entries;

    // 2) Calculamos el índice del evento más antiguo
    uint8_t start = (head + EEPROM_LOG_MAX_ENTRIES - count)
                  % EEPROM_LOG_MAX_ENTRIES;

    // 3) Leemos los eventos en orden circular
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

// ─── Borrar todo el log de eventos ───────────────────────────────────────
void eeprom_erase_log(void)
{
    uint8_t zero = 0;

    // 1) Resetear HEAD y COUNT
    eeprom_write(EEPROM_LOG_HEAD_ADDR,  &zero, 1);
    eeprom_write(EEPROM_LOG_COUNT_ADDR, &zero, 1);

    // 2) Limpiar todas las entradas (opcional, valor 0xFF = vacío)
    uint8_t empty_entry[EEPROM_LOG_ENTRY_SIZE];
    memset(empty_entry, 0xFF, EEPROM_LOG_ENTRY_SIZE);

    for (uint8_t i = 0; i < EEPROM_LOG_MAX_ENTRIES; i++) {
        uint16_t slot_addr = EEPROM_LOG_ENTRY_ADDR
                           + (uint16_t)i * EEPROM_LOG_ENTRY_SIZE;
        eeprom_write(slot_addr, empty_entry, EEPROM_LOG_ENTRY_SIZE);
    }
}

// ─── Restaurar los thresholds por defecto ────────────────────────────────
bool_t eeprom_restore_defaults(void)
{
    return eeprom_write_thresholds(DEFAULT_THRESHOLD_LOW,
                                   DEFAULT_THRESHOLD_HIGH);
}
