#ifndef BT_H
#define BT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool bt_init(void);
bool bt_send(const char* msg);
bool bt_send_raw(const uint8_t* data, size_t len);

#endif // BT_H
