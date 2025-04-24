//======================================================================================================================
// app.c - Main application logic for sound level detector
//======================================================================================================================

// C Standard Libraries
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// STM32 HAL
#include "stm32f4xx_hal.h"

// Project Headers
#include "API_delay.h"
#include "app.h"
#include "app_isr.h"
#include "bt.h"
#include "button.h"
#include "debug_uart.h"
#include "eeprom.h"
#include "main.h"
#include "rtc.h"
#include "usb_cdc.h"
#include "usb_commands.h"

//----------------------------------------------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------------------------------------------
#define INIT_DELAY_MS         1000U   ///< Delay inicial de 1 segundo para la medición

//----------------------------------------------------------------------------------------------------------------------
// Tipos de datos privados
//----------------------------------------------------------------------------------------------------------------------
typedef enum {
    STATE_INITIALIZING,     ///< Estado inicial: se configuran periféricos y se carga configuración
    STATE_IDLE,             ///< Estado de espera: sin medición activa
    STATE_MONITORING,       ///< Estado de monitoreo de nivel de sonido
    STATE_USB_COMMAND       ///< Estado de procesamiento de comandos USB
} app_state_t;

//----------------------------------------------------------------------------------------------------------------------
// Variables privadas
//----------------------------------------------------------------------------------------------------------------------
static app_state_t application_state = STATE_INITIALIZING; ///< Estado actual de la aplicación
static delay_t     measureDelay;                            ///< Delay no bloqueante para el muestreo
static uint16_t    threshold_low;                           ///< Umbral inferior de nivel de sonido
static uint16_t    threshold_high;                          ///< Umbral superior de nivel de sonido

// Variables compartidas con interrupciones (declaradas en app_isr.c)
extern volatile uint16_t envelope;                          ///< Nivel de sonido actual (calculado en DMA)
extern uint16_t          adc_dma_buffer[ADC_BUFFER_SIZE];   ///< Buffer DMA del ADC

// Periféricos usados
extern TIM_HandleTypeDef htim2;
extern ADC_HandleTypeDef hadc1;

//----------------------------------------------------------------------------------------------------------------------
// Funciones privadas
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
// Manejo del estado de inicialización
//----------------------------------------------------------------------------------------------------------------------

/**
 * @brief Inicializa los periféricos y carga configuración desde EEPROM.
 *
 * Inicia ADC con DMA, TIM2, carga umbrales de EEPROM y prepara drivers para operación.
 */
static void on_initializing(void)
{
    debug_uart_print("INIT: entering on_initializing()\r\n");
    usb_commands_init();
    eeprom_init();
    rtc_init();
    bt_init();

    // Leer umbrales desde EEPROM o usar valores por defecto si falla
    eeprom_read_thresholds(&threshold_low, &threshold_high);

    // Iniciar ADC1 con DMA a 1 kHz
    HAL_TIM_Base_Start(&htim2);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_dma_buffer, ADC_BUFFER_SIZE);

    // Iniciar delay no bloqueante de 1 segundo
    delayInit(&measureDelay, INIT_DELAY_MS);
    debug_uart_print("INIT: drivers initialized\r\n");
    application_state = STATE_IDLE;
}

//----------------------------------------------------------------------------------------------------------------------
// Manejo del estado IDLE
//----------------------------------------------------------------------------------------------------------------------

/**
 * @brief Estado de espera. Gestiona botones y transición por timeout o USB.
 */
static void on_idle(void)
{
    button_update();

    if (button_was_long_pressed()) {
        eeprom_erase_log();
        usb_cdc_sendString("EEPROM logs erased.\r\n");
    }
    else if (button_was_pressed()) {
        eeprom_restore_defaults();
        usb_cdc_sendString("Thresholds restored to defaults.\r\n");
    }

    if (delayRead(&measureDelay)) {
        application_state = STATE_MONITORING;
        return;
    }
    if (usb_commands_pending()) {
        application_state = STATE_USB_COMMAND;
        return;
    }
}

//----------------------------------------------------------------------------------------------------------------------
// Manejo del estado MONITORING
//----------------------------------------------------------------------------------------------------------------------

/**
 * @brief Clasifica nivel de sonido, reporta y loguea si es alto.
 */
static void on_monitoring(void)
{
    char buf[80];
    debug_uart_print("=== ENTER MONITORING ===\r\n");
    sprintf(buf, "DBG: Envelope = %u\r\n", envelope);
    debug_uart_print(buf);

    if (envelope <= threshold_low) {
        debug_uart_print("DBG: Classification → LOW NOISE\r\n");
        bt_send("LOW NOISE\r\n");
    } else if (envelope < threshold_high) {
        debug_uart_print("DBG: Classification → MEDIUM NOISE\r\n");
        bt_send("MEDIUM NOISE\r\n");
    } else {
        debug_uart_print("DBG: Classification → HIGH NOISE\r\n");
        bt_send("HIGH NOISE\r\n");

        rtc_datetime_t dt;
        if (rtc_get_datetime(&dt)) {
            sprintf(buf,
                    "DBG: Logging @ %02u/%02u/20%02u %02u:%02u:%02u, lvl=%u\r\n",
                    dt.day, dt.month, dt.year,
                    dt.hour, dt.min, dt.sec,
                    envelope);
            debug_uart_print(buf);

            eeprom_log_entry_t entry = {
                .year   = dt.year,
                .month  = dt.month,
                .day    = dt.day,
                .hour   = dt.hour,
                .minute = dt.min,
                .second = dt.sec,
                .level  = (uint16_t)envelope
            };
            eeprom_log_high_event(&entry);
        } else {
            debug_uart_print("DBG: RTC read failed, skipping log\r\n");
        }
    }

    debug_uart_print("=== EXIT MONITORING ===\r\n");
    delayWrite(&measureDelay, INIT_DELAY_MS);
    application_state = STATE_IDLE;
}

//----------------------------------------------------------------------------------------------------------------------
// Manejo del estado USB_COMMAND
//----------------------------------------------------------------------------------------------------------------------

/**
 * @brief Procesa comandos entrantes desde la interfaz USB CDC.
 */

static void on_usb_command(void)
{
    pending_action_t act;
    char buf[64];
    usb_command_t cmd = usb_commands_get(&act);
    switch (cmd) {
        case CMD_GET_THRESH: {
            snprintf(buf, sizeof(buf), "TH_LOW=%u TH_HIGH=%u\r\n", threshold_low, threshold_high);
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

//----------------------------------------------------------------------------------------------------------------------
// Funciones publicas
//----------------------------------------------------------------------------------------------------------------------

/**
 * @brief Ejecuta el ciclo de aplicación según el estado actual.
 *
 * Lazo principal que evalúa el estado de la aplicación y despacha a los handlers correspondientes.
 * Esta función bloquea y debe ser llamada una sola vez desde main().
 */
void appUpdate(void)
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
