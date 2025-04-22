#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t sec;    // 0–59
    uint8_t min;    // 0–59
    uint8_t hour;   // 0–23
    uint8_t day;    // 1–7 (weekday, optional)
    uint8_t date;   // 1–31
    uint8_t month;  // 1–12
    uint8_t year;   // 0–99 (-> 20YY)
} rtc_datetime_t;

/// Call once at startup.
/// Returns true if I²C OK; sets a flag if no valid time yet.
bool rtc_init(void);

/// Has the time ever been set? (checks CH bit on DS1307)
bool rtc_is_set(void);

/// Read current time/date. Returns false if RTC not set.
bool rtc_get_datetime(rtc_datetime_t *dt);

/// Write time/date. This _starts_ the oscillator (clears CH bit)
bool rtc_set_datetime(const rtc_datetime_t *dt);
