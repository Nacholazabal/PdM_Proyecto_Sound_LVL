#ifndef PORT_BT_H
#define PORT_BT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool port_bt_init(void);
bool port_bt_send(const uint8_t* data, size_t len);

#endif // PORT_BT_H
