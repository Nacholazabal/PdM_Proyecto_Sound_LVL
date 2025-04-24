#include "rtc.h"
#include "port_rtc.h"

// === Funciones auxiliares ===

// Convierte un número codificado en BCD (ej: 0x25 = 37) a binario decimal (37)
// BCD codifica los dígitos decimales por separado en nibbles: 0x25 = 0x2*10 + 0x5 = 25
static uint8_t bcd2bin(uint8_t v) {
    return (v & 0x0F) + ((v >> 4) * 10);
}

// Convierte un número decimal (ej: 37) a formato BCD: 0x25
static uint8_t bin2bcd(uint8_t v) {
    return ((v / 10) << 4) | (v % 10);
}

// Bandera que indica si el RTC fue correctamente configurado y activado
static bool time_set = false;

// === Funciones públicas ===

bool rtc_init(void)
{
    port_rtc_init();

    uint8_t sec_reg;
    // Leemos el registro de segundos (reg 0x00), donde el bit 7 (CH) indica si el reloj está detenido
    if (!port_rtc_read(0x00, &sec_reg, 1)) {
        return false;
    }

    // Si el bit CH está en 1, el reloj no está corriendo → time_set = false
    time_set = !(sec_reg & 0x80);
    return true;
}

bool rtc_is_set(void)
{
    return time_set;
}

bool rtc_get_datetime(rtc_datetime_t *dt)
{
    if (!time_set) return false;

    uint8_t buf[7];
    // Leemos los 7 registros de tiempo desde 0x00
    if (!port_rtc_read(0x00, buf, 7)) {
        return false;
    }

    // Convertimos los campos BCD a binario
    dt->sec   = bcd2bin(buf[0] & 0x7F);  // bit 7 = CH, lo descartamos
    dt->min   = bcd2bin(buf[1]);
    dt->hour  = bcd2bin(buf[2] & 0x3F);  // 24h, ignoramos bit 6 AM/PM
    dt->day   = bcd2bin(buf[3]);
    dt->date  = bcd2bin(buf[4]);
    dt->month = bcd2bin(buf[5]);
    dt->year  = bcd2bin(buf[6]);
    return true;
}

bool rtc_set_datetime(const rtc_datetime_t *dt)
{
    uint8_t buf[8];

    // Codificamos los campos a BCD
    buf[0] = bin2bcd(dt->sec) & 0x7F;  // bit 7 (CH) en 0 para arrancar reloj
    buf[1] = bin2bcd(dt->min);
    buf[2] = bin2bcd(dt->hour);
    buf[3] = bin2bcd(dt->day);
    buf[4] = bin2bcd(dt->date);
    buf[5] = bin2bcd(dt->month);
    buf[6] = bin2bcd(dt->year);

    // Escribimos los 7 registros en el RTC comenzando desde la dirección 0x00
    if (!port_rtc_write(0x00, buf, 7)) {
        return false;
    }

    time_set = true;
    return true;
}
