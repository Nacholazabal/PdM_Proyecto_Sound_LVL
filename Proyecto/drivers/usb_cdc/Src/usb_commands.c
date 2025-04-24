/**
 * @file usb_commands.c
 * @brief Implementación del parser de comandos USB CDC.
 *
 * Este módulo interpreta los comandos de texto recibidos por USB CDC,
 * los convierte a estructuras y ejecuta callbacks asociados.
 */

// === Includes ===
#include <stdio.h>
#include <string.h>

#include "debug_uart.h"
#include "rtc.h"
#include "usb_cdc.h"
#include "usb_commands.h"

void usb_commands_init(void) {
    // Asegura que el driver de CDC esté inicializado
    usb_cdc_init();
}

bool_t usb_commands_pending(void) {
    return usb_cdc_isCommandPending();
}

// Parsea los argumentos del comando 'settime'
static bool_t parse_settime_args(const char *args, rtc_datetime_t *dt) {
    unsigned int yy, MM, DD, hh, mm, ss;
    int matched = sscanf(args, "%u %u %u %u %u %u", &yy, &MM, &DD, &hh, &mm, &ss);
    if (matched != 6) {
        return false;
    }
    dt->year   = (uint8_t)yy;
    dt->month  = (uint8_t)MM;
    dt->date   = (uint8_t)DD;
    dt->hour   = (uint8_t)hh;
    dt->min    = (uint8_t)mm;
    dt->sec    = (uint8_t)ss;
    return true;
}

usb_command_t usb_commands_get(pending_action_t *out) {
    const char *cmd = usb_cdc_getCommand();

    if (strcmp(cmd, USB_CMD_GET_THRESH) == 0) {
        out->cmd = CMD_GET_THRESH;
    } else if (sscanf(cmd, USB_CMD_SET_THRESH " %hu %hu", &out->low, &out->high) == 2) {
        out->cmd = CMD_SET_THRESH;
    } else if (strcmp(cmd, USB_CMD_GET_TIME) == 0) {
        out->cmd = CMD_GET_TIME;
    } else if (strncmp(cmd, USB_CMD_SET_TIME " ", strlen(USB_CMD_SET_TIME) + 1) == 0) {
        const char *args = cmd + strlen(USB_CMD_SET_TIME) + 1;
        if (parse_settime_args(args, &out->dt)) {
            out->cmd = CMD_SET_TIME;
        } else {
            out->cmd = CMD_HELP;
        }
    } else if (strcmp(cmd, USB_CMD_GET_LOG) == 0) {
        out->cmd = CMD_GET_LOG;
    } else {
        out->cmd = CMD_HELP;
    }

    usb_cdc_clearCommand();
    return out->cmd;
}

void usb_commands_print_help(void) {
    usb_cdc_sendString(
        "Comandos disponibles:\r\n"
        "  getth                  - Mostrar umbrales actuales\r\n"
        "  setth <low> <high>     - Establecer nuevos umbrales\r\n"
        "  gettime                - Leer fecha/hora del RTC\r\n"
        "  settime YY MM DD h m s - Establecer fecha/hora RTC\r\n"
        "  getlog                 - Mostrar eventos registrados\r\n"
        "  help                   - Mostrar esta ayuda\r\n"
    );
}
