#include "usb_commands.h"
#include "usb_cdc.h"
#include "debug_uart.h"
#include <string.h>
#include <stdio.h>
#include "rtc.h"

void usb_commands_init(void) {
    // Ensure CDC driver is up
    usb_cdc_init();
}

bool usb_commands_pending(void) {
    return usb_cdc_isCommandPending();
}

static bool parse_settime_args(const char *args, rtc_datetime_t *dt) {
    unsigned int yy, MM, DD, hh, mm, ss;
    int matched = sscanf(args,
                         "%u %u %u %u %u %u",
                         &yy, &MM, &DD, &hh, &mm, &ss);
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

    // GET_THRESH
    if (strcmp(cmd, USB_CMD_GET_THRESH) == 0) {
        out->cmd = CMD_GET_THRESH;
    }
    // SET_THRESH L H
    else if (sscanf(cmd, USB_CMD_SET_THRESH " %hu %hu",
                    &out->low, &out->high) == 2) {
        out->cmd = CMD_SET_THRESH;
    }
    // GET_TIME
    else if (strcmp(cmd, USB_CMD_GET_TIME) == 0) {
        out->cmd = CMD_GET_TIME;
    }
    // SET_TIME …delegate to helper…
    else if (strncmp(cmd, USB_CMD_SET_TIME " ", strlen(USB_CMD_SET_TIME) + 1) == 0)
    {
        const char *args = cmd + strlen(USB_CMD_SET_TIME) + 1;
        if (parse_settime_args(args, &out->dt)) {
            out->cmd = CMD_SET_TIME;
        } else {
            out->cmd = CMD_HELP;
        }
    }
    // GET_LOG
    else if (strcmp(cmd, USB_CMD_GET_LOG) == 0) {
        out->cmd = CMD_GET_LOG;
    }
    // HELP or unrecognized
    else {
        out->cmd = CMD_HELP;
    }

    usb_cdc_clearCommand();
    return out->cmd;
}

void usb_commands_print_help(void) {
    usb_cdc_sendString(
        "Commands:\r\n"
        "  getth                - Show thresholds\r\n"
        "  setth <low> <high>   - Set thresholds\r\n"
        "  gettime              - Read RTC\r\n"
        "  settime YY MM DD h m s - Set RTC\r\n"
        "  getlog               - Dump event log\r\n"
        "  help                 - Show this message\r\n"
    );
}
