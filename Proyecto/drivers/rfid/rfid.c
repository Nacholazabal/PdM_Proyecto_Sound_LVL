#include "rfid.h"
#include "port_rfid.h"
#include <string.h>

static uint8_t registered_cards[RFID_MAX_CARDS][RFID_UID_MAX_LEN];
static size_t  registered_count = 0;

bool rfid_init(void) {
    return port_rfid_init();
}

bool rfid_read_uid(uint8_t *uid, size_t *uid_len) {
    return port_rfid_read_uid(uid, uid_len);
}

bool rfid_register_card(const uint8_t *uid, size_t uid_len) {
    if (uid_len > RFID_UID_MAX_LEN || registered_count >= RFID_MAX_CARDS) {
        return false;
    }
    memcpy(registered_cards[registered_count++], uid, uid_len);
    return true;
}

bool rfid_authenticate(const uint8_t *uid, size_t uid_len) {
    for (size_t i = 0; i < registered_count; i++) {
        if (uid_len == RFID_UID_MAX_LEN &&
            memcmp(registered_cards[i], uid, uid_len) == 0) {
            return true;
        }
    }
    return false;
}
