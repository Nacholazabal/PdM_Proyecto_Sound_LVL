#pragma once
#include <stdbool.h>

/// Initialize the BLE UART port (if needed)
bool port_ble_init(void);

/// Send a zero‑terminated string over UART1 to the BLE module
bool port_ble_sendString(const char *str);
