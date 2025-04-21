#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "rtc.h"      // for rtc_datetime_t

// Command text
#define USB_CMD_GET_THRESH  "getth"
#define USB_CMD_SET_THRESH  "setth"
#define USB_CMD_GET_TIME    "gettime"
#define USB_CMD_SET_TIME    "settime"
#define USB_CMD_GET_LOG     "getlog"
#define USB_CMD_HELP        "help"

// Parsed command IDs
typedef enum {
    CMD_GET_THRESH,
    CMD_SET_THRESH,
    CMD_GET_TIME,
    CMD_SET_TIME,
    CMD_GET_LOG,
    CMD_HELP
} usb_command_t;

// Carries params for SET_THRESH and SET_TIME
typedef struct {
    usb_command_t   cmd;
    uint16_t        low;
    uint16_t        high;
    rtc_datetime_t  dt;
} pending_action_t;

// Public API
void            usb_commands_init(void);
bool            usb_commands_pending(void);
usb_command_t   usb_commands_get(pending_action_t *action);
void            usb_commands_print_help(void);
