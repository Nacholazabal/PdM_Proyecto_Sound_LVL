// app.c
#include "stm32f4xx_hal.h"
#include "main.h"
#include "app.h"
#include "app_isr.h"
#include "usb_commands.h"
#include "usb_cdc.h"
#include "eeprom.h"
#include "rtc.h"
#include "debug_uart.h"
#include "API_delay.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    STATE_INITIALIZING,
    STATE_IDLE,
    STATE_MONITORING,
    STATE_USB_COMMAND
} app_state_t;

static app_state_t application_state = STATE_INITIALIZING;
static delay_t     measureDelay;
static uint16_t    threshold_low;
static uint16_t    threshold_high;

// these live in app_isr.c
extern volatile uint16_t envelope;
extern uint16_t          adc_dma_buffer[ADC_BUFFER_SIZE];

extern TIM_HandleTypeDef htim2;
extern ADC_HandleTypeDef hadc1;

//─── Initialization ─────────────────────────────────────────────────────────
static void on_initializing(void)
{
	debug_uart_print("INIT: entering on_initializing()\r\n");
    usb_commands_init();
    eeprom_init();
    rtc_init();

    // load or default thresholds
    eeprom_read_thresholds(&threshold_low, &threshold_high);

    // kick off TIM2→ADC1@1kHz DMA→256 samples
    HAL_TIM_Base_Start(&htim2);
    HAL_ADC_Start_DMA(&hadc1,
                      (uint32_t*)adc_dma_buffer,
                      ADC_BUFFER_SIZE);

    // start a 1 s non‑blocking delay
    delayInit(&measureDelay, 1000);
    debug_uart_print("INIT: drivers initialized\r\n");

    application_state = STATE_IDLE;
}

//─── Idle: wait for timeout or USB ────────────────────────────────────────────
static void on_idle(void)
{
    if (delayRead(&measureDelay)) {
        application_state = STATE_MONITORING;
        return;
    }
    if (usb_commands_pending()) {
        application_state = STATE_USB_COMMAND;
        return;
    }
}

//─── Monitoring: show & log high events ──────────────────────────────────────
static void on_monitoring(void)
{
    char buf[64];
    sprintf(buf, "ENV = %u counts\r\n", envelope);
    debug_uart_print(buf);

    if (envelope < threshold_low) {
        // TODO: turn off all 6 LEDs
    }
    else if (envelope < threshold_high) {
        // TODO: light 2 LEDs proportional to (envelope - low)/(high - low)
    }
    else {
        // TODO: light all 6 LEDs
        // log timestamp
    	rtc_datetime_t dt;
    	if (rtc_get_datetime(&dt)) {
    	    eeprom_log_entry_t entry;
    	    entry.year   = dt.year;
    	    entry.month  = dt.month;
    	    entry.day    = dt.date;
    	    entry.hour   = dt.hour;
    	    entry.minute = dt.min;
    	    entry.second = dt.sec;
    	    entry.level  = envelope;
    	    eeprom_log_high_event(&entry);
    	}
    }

    // restart 1 s and go back
    delayWrite(&measureDelay, 1000);
    application_state = STATE_IDLE;
}

//─── USB Command handler ─────────────────────────────────────────────────────
static void on_usb_command(void)
{
    pending_action_t act;
    char buf[64];
    usb_command_t cmd = usb_commands_get(&act);
    switch (cmd) {
        case CMD_GET_THRESH: {
            snprintf(buf, sizeof(buf),
                     "TH_LOW=%u TH_HIGH=%u\r\n",
                     threshold_low, threshold_high);
            usb_cdc_sendString(buf);
            break;
        }
        case CMD_SET_THRESH: {
            if (eeprom_write_thresholds(act.low, act.high)) {
                threshold_low  = act.low;
                threshold_high = act.high;
                usb_cdc_sendString("Thresholds updated\r\n");
            } else {
                usb_cdc_sendString("EEPROM write error\r\n");
            }
            break;
        }
        case CMD_GET_TIME: {
            rtc_datetime_t dt;
            if (rtc_get_datetime(&dt)) {
                snprintf(buf, sizeof(buf),
                         "%02u/%02u/20%02u %02u:%02u:%02u\r\n",
                         dt.date, dt.month, dt.year,
                         dt.hour, dt.min, dt.sec);
                usb_cdc_sendString(buf);
            } else {
                usb_cdc_sendString("RTC: ND\r\n");
            }
            break;
        }
        case CMD_SET_TIME: {
            if (rtc_set_datetime(&act.dt)) {
                usb_cdc_sendString("RTC updated\r\n");
            } else {
                usb_cdc_sendString("RTC write error\r\n");
            }
            break;
        }
        case CMD_GET_LOG: {
            eeprom_log_entry_t entries[EEPROM_LOG_MAX_ENTRIES];
            uint8_t count;
            if (eeprom_read_log(entries, EEPROM_LOG_MAX_ENTRIES, &count)) {
                for (uint8_t i = 0; i < count; i++) {
                    snprintf(buf, sizeof(buf),
                             "%02u/%02u/20%02u %02u:%02u:%02u Lvl=%u\r\n",
                             entries[i].day,
                             entries[i].month,
                             entries[i].year,
                             entries[i].hour,
                             entries[i].minute,
                             entries[i].second,
                             entries[i].level);
                    usb_cdc_sendString(buf);
                }
            } else {
                usb_cdc_sendString("Log empty or error\r\n");
            }
            break;
        }
        case CMD_HELP:
        default:
            usb_commands_print_help();
            break;
    }
    application_state = STATE_IDLE;
}

//─── Main loop ───────────────────────────────────────────────────────────────
void app_entry_point(void)
{
    while (1) {
        switch (application_state) {
            case STATE_INITIALIZING:
            	on_initializing();
            	break;
            case STATE_IDLE:
            	on_idle();
            	break;
            case STATE_MONITORING:
            	on_monitoring();
            	break;
            case STATE_USB_COMMAND:
            	on_usb_command();
            	break;
        }
    }
}
