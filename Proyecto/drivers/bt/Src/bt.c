#include "bt.h"
#include "port_bt.h"
#include <string.h>

bool bt_init(void) {
    return port_bt_init();
}

bool bt_send(const char* msg) {
    return bt_send_raw((const uint8_t*)msg, strlen(msg));
}

bool bt_send_raw(const uint8_t* data, size_t len) {
    return port_bt_send(data, len);
}
