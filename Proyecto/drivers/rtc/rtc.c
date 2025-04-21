#include "rtc.h"
#include "port_rtc.h"

// BCD ↔ BIN helpers
static uint8_t  bcd2bin(uint8_t v) { return (v & 0x0F) + ((v >> 4) * 10); }
static uint8_t  bin2bcd(uint8_t v) { return ((v / 10) << 4) | (v % 10); }

static bool     time_set = false;

bool rtc_init(void)
{
    port_rtc_init();

    uint8_t sec_reg;
    if (!port_rtc_read(0x00, &sec_reg, 1)) {
        return false;
    }
    // CH bit = bit7 of seconds register. If set → clock halted (not init’d)
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
    if (!port_rtc_read(0x00, buf, 7)) {
        return false;
    }
    // decode BCD fields
    dt->sec   = bcd2bin(buf[0] & 0x7F);
    dt->min   = bcd2bin(buf[1]);
    dt->hour  = bcd2bin(buf[2] & 0x3F);
    dt->day   = bcd2bin(buf[3]);
    dt->date  = bcd2bin(buf[4]);
    dt->month = bcd2bin(buf[5]);
    dt->year  = bcd2bin(buf[6]);
    return true;
}

bool rtc_set_datetime(const rtc_datetime_t *dt)
{
    uint8_t buf[8];
    // seconds with CH=0 to start oscillator
    buf[0] = bin2bcd(dt->sec) & 0x7F;
    buf[1] = bin2bcd(dt->min);
    buf[2] = bin2bcd(dt->hour);
    buf[3] = bin2bcd(dt->day);
    buf[4] = bin2bcd(dt->date);
    buf[5] = bin2bcd(dt->month);
    buf[6] = bin2bcd(dt->year);
    // write starting at reg 0x00
    if (!port_rtc_write(0x00, buf, 7)) {
        return false;
    }
    time_set = true;
    return true;
}
