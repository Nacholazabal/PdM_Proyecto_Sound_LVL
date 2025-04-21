#include "usb_commands.h"
#include "usb_cdc.h"
#include "debug_uart.h"
#include <string.h>
#include <stdio.h>

void usb_commands_init(void) {
    // Ensure CDC driver is up
    usb_cdc_init();
}

bool usb_commands_pending(void) {
    return usb_cdc_isCommandPending();
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
    // SET_TIME Y M D h m s
    else if (sscanf(cmd, USB_CMD_SET_TIME " %hhu %hhu %hhu %hhu %hhu %hhu",
                    &out->dt.year, &out->dt.month, &out->dt.day,
                    &out->dt.hour, &out->dt.min, &out->dt.sec) == 6) {
        out->cmd = CMD_SET_TIME;
    }
    // GET_LOG
    else if (strcmp(cmd, USB_CMD_GET_LOG) == 0) {
        out->cmd = CMD_GET_LOG;
    }
    // HELP or unrecognized
    else {
        out->cmd = CMD_HELP;
    }

    // Consume the pending flag and buffer
    usb_cdc_clearCommand();
    return out->cmd;
}

void usb_commands_print_help(void) {
    debug_uart_print(
        "Commands:\r\n"
        "  getth                - Show thresholds\r\n"
        "  setth <low> <high>   - Set thresholds\r\n"
        "  gettime              - Read RTC\r\n"
        "  settime YY MM DD h m s - Set RTC\r\n"
        "  getlog               - Dump event log\r\n"
        "  help                 - Show this message\r\n"
    );
}
